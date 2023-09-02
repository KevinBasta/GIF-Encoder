
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "bitarray.h"
#include "hashmap.h"
#include "queue.h"
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
        if (*entriesEqualCodeSize == 0) {
            (*currentCodeSize)++;
            *increaseCodeSizeFlag = false;
            printf("CURRENT CODE SIZE INCREASED: %d\n", *currentCodeSize);
        } else {
            (*entriesEqualCodeSize)--;
        }
        /* if (*entriesEqualCodeSize % 2 == 0) {
            (*currentCodeSize)++;
        }

        if (*entriesEqualCodeSize == 0) {
            *increaseCodeSizeFlag = false;
        } else {
            (*entriesEqualCodeSize)--;
        } */
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
    bitarrayAppendPackedNormalizedRight(imageData, value, currentCodeSize);
    //printf("%d\n", currentCodeSize);
    //printf("pow: %d new entry: %d\n", pow(2, currentCodeSize) - 1, newCodeTableEntry);
    if (hasNewCodeTableEntry == true) {
        if (pow(2, currentCodeSize) - 1 == newCodeTableEntry) {
            *increaseCodeSizeFlag = true;
            *entriesEqualCodeSize += 1;
            //printf("entries equal code size: %d\n", *entriesEqualCodeSize);
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
    array *indexBuffer = arrayInit(indexStream->size * 10);
    array *codeStream = arrayInit(indexStream->size * 10);

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

    // printf("indexStream->size: %d\n", indexStream->size);
    for (size_t i = 1; i < indexStream->size; i++) {
        // Add next index stream entry to index buffer temporarily
        u32 k = arrayGetItem(indexStream, i);
        status = arrayAppend(indexBuffer, k);
        CHECKSTATUS(status);

        //printf("================================\n");
        char *indexBufferPlusKKey = arrayConcat(indexBuffer, ',');
        
        /* if (i == 4818) {
            printf("%d\n", k);
            printf("%s\n", indexBufferPlusKKey);
            printf("%d\n", indexBuffer->currentIndex);
        } */

        if (hashmapSearch(codeTable->map, indexBufferPlusKKey) == NULL) {
            //printf("index buffer + k not in hash map\n");

            /* if (i == 4818) {
                printf("NOT IN HASH\n");
            } */
            // Add entry for index buffer + k in code table
            // with value as the next smallest code table index
            u32 test = getNextIndexCodeTable(codeTable);
            char *next = intToString(test);
            /* if (i == 4818) {
                printf("%d\n", test);
                printf("%s\n", next);
            } */

            status = hashmapInsert(codeTable->map, indexBufferPlusKKey, next);
            CHECKSTATUS(status);

            // Add the code for just the index buffer to the code stream
            status = arrayPop(indexBuffer);
            CHECKSTATUS(status);

            char *indexBufferKey = arrayConcat(indexBuffer, ',');
            char *value = hashmapSearch(codeTable->map, indexBufferKey);
            CHECK_NULL_RETURN(value);

            /* if (i == 4818) {
                printf("%d\n", atoi(value));
                printf("%s\n", value);
                printf("%s\n", indexBufferKey);
            } */
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
            //printf("current code size: %d\n", currentCodeSize);
            /* if (getCurrentIndexCodeTable(codeTable) > 4095 || currentCodeSize >= 12 || i == 4817) {
                printf("GREATER THAN MAX CODE TABLE ENTRY\n");
                char *clearCodeValue = hashmapSearch(codeTable->map, "cc");
                CHECK_NULL_RETURN(clearCodeValue);

                status = arrayAppend(codeStream, atoi(clearCodeValue));
                CHECKSTATUS(status);

                freeCodeTable(codeTable);
                codeTable = initCodeTable(clrTable);
                
                currentCodeSize = getLWZMinCodeSize(clrTable->size - 1);
                increaseCodeSizeFlag = false;
                entriesEqualCodeSize = 0;
            } */
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
    
    //addCodeStreamValueToImageData(imageData, atoi(value), currentCodeSize, false, 0, &increaseCodeSizeFlag, &entriesEqualCodeSize);
    //checkCondidtionsForCodeSizeIncrease(&increaseCodeSizeFlag, &entriesEqualCodeSize, &currentCodeSize);


    // Add the end of information value
    char *endOfInfoValue = hashmapSearch(codeTable->map, "eoi");
    CHECK_NULL_RETURN(endOfInfoValue);

    status = arrayAppend(codeStream, atoi(endOfInfoValue));
    CHECKSTATUS(status);

    //addCodeStreamValueToImageData(imageData, atoi(endOfInfoValue), currentCodeSize, false, 0, &increaseCodeSizeFlag, &entriesEqualCodeSize);
    //checkCondidtionsForCodeSizeIncrease(&increaseCodeSizeFlag, &entriesEqualCodeSize, &currentCodeSize);

    
    // imageData set the second byte to number of bytes
    numberOfBytesOfDataInSubBlock = imageData->currentIndex + 1 - startByte;
    printf("TOTAL BYTES %d\n", numberOfBytesOfDataInSubBlock);
    bitarraySetBookMarkValue(imageData, numberOfBytesOfDataInSubBlock);
    // imageData block terminator
    bitarrayAppend(imageData, 0x00);

    arrayPrint(codeStream);

    freeArray(indexBuffer);
    freeCodeTable(codeTable);
    freeArray(codeStream);

    return OPERATION_SUCCESS;
}



STATUS_CODE revisedLZWImageData(colorTable *clrTable, array *indexStream, bitarray *imageData) {
    STATUS_CODE status;
    
    codeTable *codeTable    = initCodeTable(clrTable);
    array *codeStream       = arrayInit(indexStream->size);
    array *indexBuffer      = arrayInit(indexStream->size);

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
    bitarrayBookMark(imageData);
    bitarrayAppend(imageData, 0);
    currentCodeSize += 1;

    u32 startByte = imageData->currentIndex;
    queue *increaseCodeSize = queueInit(100);

    for (size_t i = 1; i < indexStream->currentIndex; i++) {
        // Get next index from index stream
        u32 k = arrayGetItem(indexStream, i);

        status = arrayAppend(indexBuffer, k);
        CHECKSTATUS(status);
        
        char *indexBufferPlusKKey = arrayConcat(indexBuffer, ',');
        //printf("Index buffer plus k: %s\n", indexBufferPlusKKey);
        
        status = arrayPop(indexBuffer);
        CHECKSTATUS(status);

        if (hashmapSearch(codeTable->map, indexBufferPlusKKey) != NULL) {
            status = arrayAppend(indexBuffer, k); CHECKSTATUS(status);
            free(indexBufferPlusKKey);
        } else {
            u32 codeTableIndex = getNextIndexCodeTable(codeTable);
            char *codeTableIndexString = intToString(codeTableIndex);
            status = hashmapInsert(codeTable->map, indexBufferPlusKKey, codeTableIndexString);
            CHECKSTATUS(status);
            
            char *indexBufferKey = arrayConcat(indexBuffer, ',');
            char *indexBufferValue = hashmapSearch(codeTable->map, indexBufferKey);
            CHECK_NULL_RETURN(indexBufferValue);
            u32 indexBufferIntegralValue = atoi(indexBufferValue);
            free(indexBufferKey);
            
            status = arrayAppend(codeStream, indexBufferIntegralValue);

            arrayReset(indexBuffer);
            arrayAppend(indexBuffer, k);
            k = 0;

            if (queueGetCurrentLength(increaseCodeSize) != 0) {
                bool dequeueValue;
                queueDequeue(increaseCodeSize, &dequeueValue);
                if (dequeueValue == true) {
                    currentCodeSize++;
                }
            }

            bitarrayAppendPackedNormalizedRight(imageData, indexBufferIntegralValue, currentCodeSize);
            if (pow(2, currentCodeSize) - 1 == codeTableIndex) {
                // only increase current code size after the current index buffer 
                // (that currently has just k) is written to the codestream
                queueEnqueue(increaseCodeSize, false);
                queueEnqueue(increaseCodeSize, true);
            }


            // if maximum code table value reached, send clear code and reset code table
            if ((codeTableIndex > 4095 || currentCodeSize > 12) && i + 1 != indexStream->currentIndex) {
                // Put the clear code in the code stream
                char *clearCodeValue = hashmapSearch(codeTable->map, "cc");
                CHECK_NULL_RETURN(clearCodeValue);

                status = arrayAppend(codeStream, atoi(clearCodeValue));
                CHECKSTATUS(status);

                // reset code table
                freeCodeTable(codeTable);
                codeTable       = initCodeTable(clrTable);
                currentCodeSize = getLWZMinCodeSize(clrTable->size - 1) + 1;
            }
        }
    }

    char *indexBufferKey = arrayConcat(indexBuffer, ',');
    char *indexBufferValue = hashmapSearch(codeTable->map, indexBufferKey);
    CHECK_NULL_RETURN(indexBufferValue);
    u32 indexBufferIntegralValue = atoi(indexBufferValue);
    free(indexBufferKey);

    status = arrayAppend(codeStream, indexBufferIntegralValue); CHECKSTATUS(status);
    bitarrayAppendPackedNormalizedRight(imageData, indexBufferIntegralValue, currentCodeSize);

    char *endOfInformationCodeValue = hashmapSearch(codeTable->map, "eoi");
    CHECK_NULL_RETURN(endOfInformationCodeValue);
    u32 endOfInformationCodeIntegralValue = atoi(endOfInformationCodeValue);

    status = arrayAppend(codeStream, endOfInformationCodeIntegralValue); CHECKSTATUS(status);
    bitarrayAppendPackedNormalizedRight(imageData, endOfInformationCodeIntegralValue, currentCodeSize);

    // imageData set the second byte to number of bytes
    bitarraySetBookMarkValue(imageData, imageData->currentIndex + 1 - startByte);
    // imageData block terminator
    bitarrayAppend(imageData, 0x00);


    freeArray(indexBuffer);
    freeArray(codeStream);
    freeCodeTable(codeTable);

    return OPERATION_SUCCESS;
}

