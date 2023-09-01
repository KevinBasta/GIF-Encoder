
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "hashmap.h"
#include "bitarray.h"
#include "typesGIF.h"
#include "GIFCodeTable.h"
#include "printUtility.h"
#include "bitUtility.h"

/**
 * @brief Allows for increasing currentCodeSize after the
 * current code (where the new code table value was added)
 * is written to the codeStream
 * @param increaseCodeSizeFlag  - Indicates that a potential 
 * code size increase is required
 * @param entriesEqualCodeSize  - Indicates the amount of 
 * new code table values that have matched with 2^(currentCodeSize) -1
 * @param currentCodeSize       - Bit width of codeStream entries being
 * written to imageData
 */
static void checkCondidtionsForCodeSizeIncrease(bool *increaseCodeSizeFlag, u32 *entriesEqualCodeSize, u8 *currentCodeSize) {    
    if (*increaseCodeSizeFlag == true) {
        if (*entriesEqualCodeSize % 2 == 0) {
            (*currentCodeSize)++;
        }

        if (*entriesEqualCodeSize == 0) {
            *increaseCodeSizeFlag = false;
        } else {
            (*entriesEqualCodeSize)--;
        }
    }
}

/**
 * @brief Adds codeStream value to imageData to be written to file
 * @param imageData 
 * @param value 
 * @param currentCodeSize 
 * @param hasNewCodeTableEntry 
 * @param newCodeTableEntry 
 * @param increaseCodeSizeFlag 
 * @param entriesEqualCodeSize 
 */
static void addCodeStreamValueToImageData(bitarray *imageData, 
                                   u32 value,
                                   u32 currentCodeSize,
                                   bool hasNewCodeTableEntry,
                                   u32 newCodeTableEntry,
                                   bool *increaseCodeSizeFlag,
                                   u32 *entriesEqualCodeSize) {    
    u32 occupiedBits = getOccupiedBits(value);
    
    bitarrayAppendPackedNormalizedRight(imageData, value, occupiedBits, currentCodeSize);
    if (hasNewCodeTableEntry == true) {
        if (pow(2, currentCodeSize) - 1 == newCodeTableEntry) {
            *increaseCodeSizeFlag = true;
            *entriesEqualCodeSize += 1;
            //printf("CODE SIZE FLAG INCREASE SET\n");
        }
    }
}

/**
 * @brief Compress an indexStream's data using the LZW algorithm
 * and pack the generated codeStream using flexible code sizes
 * @param clrTable      - Color table to use
 * @param indexStream   - A frame described in terms of color table
 * @param imageData     - The outputted compressed data generated
 * by this function.
 * @return OPERATION_SUCCESS or OPERATION_FAILED
 */
STATUS_CODE createLZWImageData(colorTable *clrTable, array *indexStream, bitarray *imageData) {
    STATUS_CODE status;
    
    codeTable *codeTable = initCodeTable(clrTable);
    array *indexBuffer = arrayInit(indexStream->size);
    array *codeStream = arrayInit(indexStream->size);

    // Put the clear code in the code stream
    char *clearCodeValue = hashmapSearch(codeTable->map, "cc");
    CHECK_NULL_RETURN(clearCodeValue);

    status = arrayAppend(codeStream, atoi(clearCodeValue));
    CHECKSTATUS(status);

    // Add first element of index stream to index buffer
    status = arrayAppend(indexBuffer, arrayGetItem(indexStream, 0));
    CHECKSTATUS(status);


    // Setting up imageData for flexible code sizes
    u8 currentCodeSize = getLWZMinCodeSize(clrTable->size - 1);
    bitarrayAppend(imageData, currentCodeSize);
    bitarrayBookMark(imageData); // Mark to put number of bytes of data
    bitarrayAppend(imageData, 0);
    currentCodeSize += 1;

    u8 numberOfBytesOfDataInSubBlock = 0;
    u32 startByte = imageData->currentIndex;

    bool increaseCodeSizeFlag = false;
    int entriesEqualCodeSize = 0;
    
    addCodeStreamValueToImageData(imageData, codeStream->items[0], currentCodeSize, false, 0, &increaseCodeSizeFlag, &entriesEqualCodeSize);

    for (size_t i = 1; i < indexStream->size; i++) {
        // Add next index stream entry to index buffer temporarily
        u32 k = arrayGetItem(indexStream, i);
        status = arrayAppend(indexBuffer, k);
        CHECKSTATUS(status);

        //printf("================================\n");
        char *indexBufferPlusKKey = arrayConcat(indexBuffer, ',');

        if (hashmapSearch(codeTable->map, indexBufferPlusKKey) == NULL) {
            //printf("index buffer + k not in hash map\n");

            // Add entry for index buffer + k in code table
            // with value as the next smallest code table index
            u32 test = getNextIndexCodeTable(codeTable);
            char *next = intToString(test);

            status = hashmapInsert(codeTable->map, indexBufferPlusKKey, next);
            CHECKSTATUS(status);

            // Add the code for just the index buffer to the code stream
            status = arrayPop(indexBuffer);
            CHECKSTATUS(status);

            char *indexBufferKey = arrayConcat(indexBuffer, ',');
            char *value = hashmapSearch(codeTable->map, indexBufferKey);
            CHECK_NULL_RETURN(value);

            status = arrayAppend(codeStream, atoi(value));
            CHECKSTATUS(status);
            free(indexBufferKey);

            // Set index buffer to k
            arrayReset(indexBuffer);
            arrayAppend(indexBuffer, k);

            // Pack into flexible code size and put into final ImageData
            addCodeStreamValueToImageData(imageData, atoi(value), currentCodeSize, true, test, &increaseCodeSizeFlag, &entriesEqualCodeSize);
            checkCondidtionsForCodeSizeIncrease(&increaseCodeSizeFlag, &entriesEqualCodeSize, &currentCodeSize);

            /**
             Need to handle code table having max number of entries
             4095. Send clear code and reset table?
            */
        } else {
            free(indexBufferPlusKKey);
        }
        
        //hashmapPrint(codeTable->map);
    }

    // Add last value to the codeStream and imageData
    char *indexBufferKey = arrayConcat(indexBuffer, ',');
    char *value = hashmapSearch(codeTable->map, indexBufferKey);
    CHECK_NULL_RETURN(value);

    status = arrayAppend(codeStream, atoi(value));
    CHECKSTATUS(status);
    free(indexBufferKey);
    
    addCodeStreamValueToImageData(imageData, atoi(value), currentCodeSize, false, 0, &increaseCodeSizeFlag, &entriesEqualCodeSize);
    checkCondidtionsForCodeSizeIncrease(&increaseCodeSizeFlag, &entriesEqualCodeSize, &currentCodeSize);


    // Add the end of information value
    char *endOfInfoValue = hashmapSearch(codeTable->map, "eoi");
    CHECK_NULL_RETURN(endOfInfoValue);

    status = arrayAppend(codeStream, atoi(endOfInfoValue));
    CHECKSTATUS(status);

    addCodeStreamValueToImageData(imageData, atoi(endOfInfoValue), currentCodeSize, false, 0, &increaseCodeSizeFlag, &entriesEqualCodeSize);
    checkCondidtionsForCodeSizeIncrease(&increaseCodeSizeFlag, &entriesEqualCodeSize, &currentCodeSize);

    
    // imageData set the second byte to number of bytes
    numberOfBytesOfDataInSubBlock = imageData->currentIndex + 1 - startByte;
    bitarraySetBookMarkValue(imageData, numberOfBytesOfDataInSubBlock);
    // imageData block terminator
    bitarrayAppend(imageData, 0x00);

    arrayPrint(codeStream);

    freeArray(indexBuffer);
    freeCodeTable(codeTable);
    freeArray(codeStream);

    return OPERATION_SUCCESS;
}
