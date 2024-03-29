
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
#include "GIFColorTable.h"
#include "GIFCodeTable.h"
#include "printUtility.h"
#include "bitUtility.h"

static STATUS_CODE gif_encodeLZWChunk(gif_array *indexBuffer, gif_codeTable *codeTable, u8 *currentCodeSize, gif_array *indexStream, gif_bitarray *imageData) {
    STATUS_CODE status;
    
    // Queue false and true when the code size should
    // be increased to increase after the current item 
    gif_queue *increaseCodeSize = gif_queueInit(100);

    for (size_t i = indexStream->currentIndex; i < indexStream->size; i++) {
        // Get next index from index stream
        u32 k = gif_arrayGetIncrement(indexStream);


        // Add k to the index buffer temporarily to create a hashmap search string
        status = gif_arrayAppend(indexBuffer, k);
        CHECKSTATUS(status);
        
/*         char *indexBufferPlusKKey = gif_arrayConcat(indexBuffer, ',');
        if (indexBufferPlusKKey == NULL)
            return LZW_INDEX_BUFFER_PLUS_K_CONCAT_NULL;
        
        status = gif_arrayPop(indexBuffer);
        CHECKSTATUS(status);
 */
        u32 searchRet;
        if (gif_hashmapSearch(codeTable->map, indexBuffer, &searchRet) == OPERATION_SUCCESS) {
            // index buffer is index buffer + k

        } else {
            // Insert indexBufferPlusKKey in hashmap with value as the next code table index
            gif_array *indexBufferPlusKKey = gif_arrayCopy(indexBuffer);
            if (indexBufferPlusKKey == NULL)
                return LZW_INDEX_BUFFER_PLUS_K_CONCAT_NULL;

            u32 codeTableIndex          = gif_codetableGetNextIndex(codeTable);
            status = gif_hashmapInsert(codeTable->map, indexBufferPlusKKey, codeTableIndex);
            CHECKSTATUS(status);
            

            // Get the code value of the index buffer without K
            status = gif_arrayPop(indexBuffer);
            CHECKSTATUS(status);

            gif_array *indexBufferKey = gif_arrayCopy(indexBuffer);
            if (indexBufferKey == NULL)
                return LZW_INDEX_BUFFER_CONCAT_NULL;

            u32 indexBufferValue;
            status = gif_hashmapSearch(codeTable->map, indexBufferKey, &indexBufferValue);
            CHECKSTATUS(status);
            
            gif_freeArray(indexBufferKey);
            

            // Reset the index buffer and add k to it
            status = gif_arrayReset(indexBuffer);
            CHECKSTATUS(status);
            status = gif_arrayAppend(indexBuffer, k);
            CHECKSTATUS(status);


            // Check if should increase codeSize based on queue
            if (gif_queueGetCurrentLength(increaseCodeSize) != 0) {
                bool dequeueValue;
                gif_queueDequeue(increaseCodeSize, &dequeueValue);
                if (dequeueValue == true) {
                    (*currentCodeSize)++;
                }
            }

            // Put the index buffer value in the code stream
            status = gif_bitarrayAppendPackedRight(imageData, indexBufferValue, *currentCodeSize);
            CHECKSTATUS(status);

            // Check if a code size increase should happen
            if (pow(2, *currentCodeSize) - 1 == codeTableIndex) {
                // only increase current code size after the current index buffer 
                // (that currently has just k) is written to the codestream
                gif_queueEnqueue(increaseCodeSize, false);
                gif_queueEnqueue(increaseCodeSize, true);
            }


            // Check if the code table 4095 limit was reached
            if (codeTable->map->currentCount == 4095) {                
                gif_freeQueue(increaseCodeSize);
                return OPERATION_SUCCESS;
            }
        }
    }

    gif_freeQueue(increaseCodeSize);

    return OPERATION_SUCCESS;
}


static STATUS_CODE gif_encodeFlexibleCodeSizeCodeStream(gif_colorTable *clrTable, gif_array *indexStream, gif_bitarray *imageData) {
    STATUS_CODE status;

    gif_codeTable *codeTable = NULL;
    if (!(indexStream->currentIndex + 1 < indexStream->size)) {
        // Needs to be initialized for the edgecase where 
        // the while loop below isn't executed
        codeTable    = gif_codetableInit(clrTable);
        CODE_TABLE_NULL_CHECK(codeTable);
    }

    gif_array     *indexBuffer;
    if (indexStream->size < 10) {
        indexBuffer = gif_arrayInit(indexStream->size * 50);
    } else {
        indexBuffer = gif_arrayInit(indexStream->size);
    }

    // Put the LZW color table min code size in the image data
    u8 currentCodeSize = gif_getLWZMinCodeSize(clrTable->lastIndex);
    status = gif_bitarrayAppend(imageData, currentCodeSize);
    u8 startingCodeSize = ++currentCodeSize;

    // Save the clear code value for use throughout the algorithm
    u16 clearCodeValue = gif_getClearCodeValue(clrTable->lastIndex);
    CHECKSTATUS(status);
    
    // Put the first index stream item in the image data
    status = gif_arrayAppend(indexBuffer, gif_arrayGetIncrement(indexStream));
    CHECKSTATUS(status);

    // Add clear code, intitalize code table, encode LZW up to code table 4095 limit
    while (indexStream->currentIndex < indexStream->size) {
        status = gif_bitarrayAppendPackedRight(imageData, clearCodeValue, currentCodeSize);
        CHECKSTATUS(status);

        if (codeTable != NULL)
            gif_freeCodeTable(codeTable);
        codeTable = gif_codetableInit(clrTable);
        CODE_TABLE_NULL_CHECK(codeTable);
        currentCodeSize = startingCodeSize;

        status = gif_encodeLZWChunk(indexBuffer, codeTable, &currentCodeSize, indexStream, imageData);
        CHECKSTATUS(status);
    }

    // Put the code value of the last index buffer state into the image data
    gif_array *indexBufferKey = gif_arrayCopy(indexBuffer);
    if (indexBufferKey == NULL)
        return LZW_INDEX_BUFFER_LAST_CONCAT_NULL;
    
    u32 indexBufferValue;
    status = gif_hashmapSearch(codeTable->map, indexBufferKey, &indexBufferValue);
    CHECKSTATUS(status);
    
    gif_freeArray(indexBufferKey);

    status = gif_bitarrayAppendPackedRight(imageData, indexBufferValue, currentCodeSize);
    CHECKSTATUS(status);


    // Put the end of information code value into the image data
    u32 endOfInformationCodeValue = gif_getEOICodeValue(clrTable->lastIndex);

    status = gif_bitarrayAppendPackedRight(imageData, endOfInformationCodeValue, currentCodeSize);
    CHECKSTATUS(status);


    // free created structures 
    gif_freeCodeTable(codeTable);
    gif_freeArray(indexBuffer);

    return OPERATION_SUCCESS;
}

/**
 * @brief Compress an indexStream's data using the LZW algorithm
 * and pack the generated codeStream using flexible code sizes
 *
 * @note The step of generating a "codeStream" is skipped in favour of
 * directly creating the flexible code size packed stream that is written
 * out to the gif file. This was done to avoid extra memory consumption
 * and to avoid having to keep track of where the code size increases 
 * occured.
 *
 * @param clrTable      - Color table to use
 * @param indexStream   - A frame described in terms of color table
 * @param imageData     - Output compressed data generated by this function
 * @return OPERATION_SUCCESS or OPERATION_FAILED
 */
STATUS_CODE gif_createLZWImageData(gif_colorTable *clrTable, gif_array *indexStream, gif_bitarray *imageData) {
    STATUS_CODE status; 

    indexStream->currentIndex = 0;
    
    //
    // Setting a rule to insert the size of each subblock after its written
    // Boundry is 0xFF + 1 because of the extra CC code at start of imageData
    //
    status = gif_bitarraySetIntervalInsertRule(imageData, 0xFF + 1, 0xFF);
    CHECKSTATUS(status);

    // Run the LZW encoding/compression algorithm on the index stream
    status = gif_encodeFlexibleCodeSizeCodeStream(clrTable, indexStream, imageData);
    CHECKSTATUS(status);

    // Set the number of bytes for the last chunk in the imageData bitarray
    status = gif_bitarraySetBookMarkValue(imageData, imageData->currentIndex - imageData->markIndex);
    CHECKSTATUS(status);

    indexStream->currentIndex = 0;

    return OPERATION_SUCCESS;
}



/**
 * @brief This function is initial working draft of the LZW flexible code size algorithm.
 * What is accomplished here is broken down and documented more clearly in the above 
 * three functions. This function is kept for debugging purposes.
 */
/* 
STATUS_CODE gif_createLZWImageDataInitialDraft(gif_colorTable *clrTable, gif_array *indexStream, gif_bitarray *imageData) {
    STATUS_CODE status;
    
    gif_codeTable *codeTable    = gif_codetableInit(clrTable);
    gif_array     *indexBuffer  = gif_arrayInit(indexStream->size);

    // Setting up imageData for flexible code sizes
    gif_bitarraySetIntervalInsertRule(imageData, 0xFF + 1, 0xFF);
    u8 currentCodeSize = gif_getLWZMinCodeSize(clrTable->lastIndex);
    gif_bitarrayAppend(imageData, currentCodeSize);

    u8 startingCodeSize = ++currentCodeSize;

    // Save the clear code value for use throughout the algorithm
    u32 clearCodeValue;
    status = gif_hashmapSearchConvert(codeTable->map, "cc", &clearCodeValue);
    CHECKSTATUS(status);

    // Add first element of index stream to index buffer
    status = gif_arrayAppend(indexBuffer, gif_arrayGetIncrement(indexStream));
    CHECKSTATUS(status);

    status = gif_bitarrayAppendPackedRight(imageData, clearCodeValue, currentCodeSize);
    CHECKSTATUS(status);

    gif_queue *increaseCodeSize = gif_queueInit(100);

    for (size_t i = 1; i < indexStream->size; i++) {
        // Get next index from index stream
        u32 k = gif_arrayGetIncrement(indexStream);

        status = gif_arrayAppend(indexBuffer, k);
        CHECKSTATUS(status);
        
        char *indexBufferPlusKKey = gif_arrayConcat(indexBuffer, ',');
        //PRINTF("Index buffer plus k: %s\n", indexBufferPlusKKey);
        
        status = gif_arrayPop(indexBuffer);
        CHECKSTATUS(status);

        if (gif_hashmapSearch(codeTable->map, indexBufferPlusKKey) != NULL) {
            status = gif_arrayAppend(indexBuffer, k); CHECKSTATUS(status);
            free(indexBufferPlusKKey);
        } else {
            u32 codeTableIndex = gif_codetableGetNextIndex(codeTable);
            char *codeTableIndexString = gif_intToString(codeTableIndex, 6);
            status = gif_hashmapInsert(codeTable->map, indexBufferPlusKKey, codeTableIndexString);
            CHECKSTATUS(status);
            
            char *indexBufferKey = gif_arrayConcat(indexBuffer, ',');
            char *indexBufferValue = gif_hashmapSearch(codeTable->map, indexBufferKey);
            CHECK_NULL(indexBufferValue);
            u32 indexBufferIntegralValue = atoi(indexBufferValue);
            free(indexBufferKey);
            
            gif_arrayReset(indexBuffer);
            gif_arrayAppend(indexBuffer, k);

            if (gif_queueGetCurrentLength(increaseCodeSize) != 0) {
                bool dequeueValue;
                gif_queueDequeue(increaseCodeSize, &dequeueValue);
                if (dequeueValue == true) {
                    currentCodeSize++;
                }
            }

            status = gif_bitarrayAppendPackedRight(imageData, indexBufferIntegralValue, currentCodeSize);
            CHECKSTATUS(status);
            if (pow(2, currentCodeSize) - 1 == codeTableIndex) {
                // only increase current code size after the current index buffer 
                // (that currently has just k) is written to the codestream
                gif_queueEnqueue(increaseCodeSize, false);
                gif_queueEnqueue(increaseCodeSize, true);
            }

            // if maximum code table value reached, send clear code and reset code table
            if (codeTable->map->currentCount == 4095) {
                // PRINTF("%ld %d %d %ld\n", i, currentCodeSize, getCurrentIndexCodeTable(codeTable), codeTable->map->currentCount);
                gif_arrayPrint(indexBuffer);
                
                // Put the clear code in the code stream
                gif_bitarrayAppendPackedRight(imageData, clearCodeValue, currentCodeSize);

                // reset code table
                gif_freeCodeTable(codeTable);
                codeTable       = gif_codetableInit(clrTable);
                currentCodeSize = startingCodeSize;
            }
        }
    }

    char *indexBufferKey = gif_arrayConcat(indexBuffer, ',');
    
    u32 indexBufferValue;
    status = gif_hashmapSearchConvert(codeTable->map, indexBufferKey, &indexBufferValue);
    CHECKSTATUS(status);
    
    free(indexBufferKey);

    status = gif_bitarrayAppendPackedRight(imageData, indexBufferValue, currentCodeSize);
    CHECKSTATUS(status);

    u32 endOfInformationCodeValue;
    status = gif_hashmapSearchConvert(codeTable->map, "eoi", &endOfInformationCodeValue);
    CHECKSTATUS(status);

    status = gif_bitarrayAppendPackedRight(imageData, endOfInformationCodeValue, currentCodeSize);
    CHECKSTATUS(status);

    // imageData set the second byte to number of bytes
    gif_bitarraySetBookMarkValue(imageData, imageData->currentIndex - imageData->markIndex - 1);

    gif_freeQueue(increaseCodeSize);
    gif_freeArray(indexBuffer);
    gif_freeCodeTable(codeTable);

    return OPERATION_SUCCESS;
}
*/