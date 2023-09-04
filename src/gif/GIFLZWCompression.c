
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

static STATUS_CODE encodeLZWChunk(array *indexBuffer, codeTable *codeTable, u8 *currentCodeSize, array *indexStream, bitarray *imageData) {
    STATUS_CODE status;
    queue *increaseCodeSize = queueInit(100);

    for (size_t i = indexStream->currentIndex; i <= indexStream->size; i++) {
        // Get next index from index stream
        u32 k = arrayGetIncrement(indexStream);

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
            u32 codeTableIndex = codetableGetNextIndex(codeTable);
            char *codeTableIndexString = intToString(codeTableIndex);
            status = hashmapInsert(codeTable->map, indexBufferPlusKKey, codeTableIndexString);
            CHECKSTATUS(status);
            
            char *indexBufferKey = arrayConcat(indexBuffer, ',');
            char *indexBufferValue = hashmapSearch(codeTable->map, indexBufferKey);
            CHECK_NULL_RETURN(indexBufferValue);
            u32 indexBufferIntegralValue = atoi(indexBufferValue);
            free(indexBufferKey);
            
            arrayReset(indexBuffer);
            arrayAppend(indexBuffer, k);

            if (queueGetCurrentLength(increaseCodeSize) != 0) {
                bool dequeueValue;
                queueDequeue(increaseCodeSize, &dequeueValue);
                if (dequeueValue == true) {
                    (*currentCodeSize)++;
                }
            }

            status = bitarrayAppendPackedNormalizedRight(imageData, indexBufferIntegralValue, *currentCodeSize);
            CHECKSTATUS(status);

            if (pow(2, *currentCodeSize) - 1 == codeTableIndex) {
                // only increase current code size after the current index buffer 
                // (that currently has just k) is written to the codestream
                queueEnqueue(increaseCodeSize, false);
                queueEnqueue(increaseCodeSize, true);
            }

            if (codeTable->map->currentCount == 4095) {                
                freeQueue(increaseCodeSize);
                return OPERATION_SUCCESS;
            }
        }
    }

    freeQueue(increaseCodeSize);

    return OPERATION_SUCCESS;
}


static STATUS_CODE encodeFlexibleCodeSizeCodeStream(colorTable *clrTable, array *indexStream, bitarray *imageData) {
    STATUS_CODE status;

    codeTable *codeTable    = NULL;
    array     *indexBuffer  = arrayInit(indexStream->size);

    // Put the LZW color table min code size in the image data
    u8 currentCodeSize = getLWZMinCodeSize(clrTable->size - 1);
    status = bitarrayAppend(imageData, currentCodeSize);
    u8 startingCodeSize = ++currentCodeSize;

    // Save the clear code value for use throughout the algorithm
    u16 clearCodeValue = getClearCodeValue(clrTable->size - 1);
    CHECKSTATUS(status);
    
    // Put the first index stream item in the image data
    status = arrayAppend(indexBuffer, arrayGetIncrement(indexStream));
    CHECKSTATUS(status);

    // Add clear code, intitalize code table, encode LZW up to code table 4095 limit
    while (indexStream->currentIndex < indexStream->size) {
        status = bitarrayAppendPackedNormalizedRight(imageData, clearCodeValue, currentCodeSize);
        CHECKSTATUS(status);

        if (codeTable != NULL)
            freeCodeTable(codeTable);
        codeTable = codetableInit(clrTable);
        currentCodeSize = startingCodeSize;

        status = encodeLZWChunk(indexBuffer, codeTable, &currentCodeSize, indexStream, imageData);
        CHECKSTATUS(status);
    }

    // Put the code value of the last index buffer state into the image data
    char *indexBufferKey = arrayConcat(indexBuffer, ',');
    
    u32 indexBufferValue;
    status = hashmapSearchConvert(codeTable->map, indexBufferKey, &indexBufferValue);
    CHECKSTATUS(status);
    
    free(indexBufferKey);

    status = bitarrayAppendPackedNormalizedRight(imageData, indexBufferValue, currentCodeSize);
    CHECKSTATUS(status);


    // Put the end of information code value into the image data
    u32 endOfInformationCodeValue;
    status = hashmapSearchConvert(codeTable->map, "eoi", &endOfInformationCodeValue);
    CHECKSTATUS(status);

    status = bitarrayAppendPackedNormalizedRight(imageData, endOfInformationCodeValue, currentCodeSize);
    CHECKSTATUS(status);


    // free created structures 
    freeCodeTable(codeTable);
    freeArray(indexBuffer);

    return OPERATION_SUCCESS;
}


STATUS_CODE createLZWImageData(colorTable *clrTable, array *indexStream, bitarray *imageData) {
    STATUS_CODE status; 

    //
    // Setting a rule to insert the size of each subblock after its written
    // Boundry is 0xFF + 1 because of the extra CC code at start of imageData
    //
    status = bitarraySetIntervalInsertRule(imageData, 0xFF + 1, 0xFF);
    CHECKSTATUS(status);

    // Run the LZW encoding/compression algorithm on the index stream
    status = encodeFlexibleCodeSizeCodeStream(clrTable, indexStream, imageData);
    CHECKSTATUS(status);

    // Set the number of bytes for the last chunk in the imageData bitarray
    status = bitarraySetBookMarkValue(imageData, imageData->currentIndex - imageData->markIndex - 1);
    CHECKSTATUS(status);

    return OPERATION_SUCCESS;
}



/**
 * @brief Compress an indexStream's data using the LZW algorithm
 * and pack the generated codeStream using flexible code sizes
 * @param clrTable      - Color table to use
 * @param indexStream   - A frame described in terms of color table
 * @param imageData     - Output compressed data generated by this function
 * @return OPERATION_SUCCESS or OPERATION_FAILED
 */
STATUS_CODE revisedLZWImageData(colorTable *clrTable, array *indexStream, bitarray *imageData) {
    STATUS_CODE status;
    
    codeTable *codeTable    = codetableInit(clrTable);
    array     *indexBuffer  = arrayInit(indexStream->size);

    // Setting up imageData for flexible code sizes
    bitarraySetIntervalInsertRule(imageData, 0xFF + 1, 0xFF);
    u8 currentCodeSize = getLWZMinCodeSize(clrTable->size - 1);
    bitarrayAppend(imageData, currentCodeSize);

    u8 startingCodeSize = ++currentCodeSize;

    // Save the clear code value for use throughout the algorithm
    u32 clearCodeValue;
    status = hashmapSearchConvert(codeTable->map, "cc", &clearCodeValue);
    CHECKSTATUS(status);

    // Add first element of index stream to index buffer
    status = arrayAppend(indexBuffer, arrayGetIncrement(indexStream));
    CHECKSTATUS(status);

    queue *increaseCodeSize = queueInit(100);

    for (size_t i = 1; i <= indexStream->size; i++) {
        // Get next index from index stream
        u32 k = arrayGetIncrement(indexStream);

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
            u32 codeTableIndex = codetableGetNextIndex(codeTable);
            char *codeTableIndexString = intToString(codeTableIndex);
            status = hashmapInsert(codeTable->map, indexBufferPlusKKey, codeTableIndexString);
            CHECKSTATUS(status);
            
            char *indexBufferKey = arrayConcat(indexBuffer, ',');
            char *indexBufferValue = hashmapSearch(codeTable->map, indexBufferKey);
            CHECK_NULL_RETURN(indexBufferValue);
            u32 indexBufferIntegralValue = atoi(indexBufferValue);
            free(indexBufferKey);
            
            arrayReset(indexBuffer);
            arrayAppend(indexBuffer, k);

            if (queueGetCurrentLength(increaseCodeSize) != 0) {
                bool dequeueValue;
                queueDequeue(increaseCodeSize, &dequeueValue);
                if (dequeueValue == true) {
                    currentCodeSize++;
                }
            }

            status = bitarrayAppendPackedNormalizedRight(imageData, indexBufferIntegralValue, currentCodeSize);
            CHECKSTATUS(status);
            if (pow(2, currentCodeSize) - 1 == codeTableIndex) {
                // only increase current code size after the current index buffer 
                // (that currently has just k) is written to the codestream
                queueEnqueue(increaseCodeSize, false);
                queueEnqueue(increaseCodeSize, true);
            }

            // if maximum code table value reached, send clear code and reset code table
            if (codeTable->map->currentCount == 4095) {
                printf("%d %d %d %d\n", i, currentCodeSize, getCurrentIndexCodeTable(codeTable), codeTable->map->currentCount);
                arrayPrint(indexBuffer);
                
                // Put the clear code in the code stream
                bitarrayAppendPackedNormalizedRight(imageData, clearCodeValue, currentCodeSize);

                // reset code table
                freeCodeTable(codeTable);
                codeTable       = codetableInit(clrTable);
                currentCodeSize = startingCodeSize;
            }
        }
    }

    char *indexBufferKey = arrayConcat(indexBuffer, ',');
    
    u32 indexBufferValue;
    status = hashmapSearchConvert(codeTable->map, indexBufferKey, &indexBufferValue);
    CHECKSTATUS(status);
    
    free(indexBufferKey);

    status = bitarrayAppendPackedNormalizedRight(imageData, indexBufferValue, currentCodeSize);
    CHECKSTATUS(status);

    u32 endOfInformationCodeValue;
    status = hashmapSearchConvert(codeTable->map, "eoi", &endOfInformationCodeValue);
    CHECKSTATUS(status);

    status = bitarrayAppendPackedNormalizedRight(imageData, endOfInformationCodeValue, currentCodeSize);
    CHECKSTATUS(status);

    // imageData set the second byte to number of bytes
    bitarraySetBookMarkValue(imageData, imageData->currentIndex - imageData->markIndex - 1);

    freeQueue(increaseCodeSize);
    freeArray(indexBuffer);
    freeCodeTable(codeTable);

    return OPERATION_SUCCESS;
}
