
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <stdint.h>
#include "main.h"

#include "typesGIF.h"
#include "GIFCodeTable.h"

#include "array.h"
#include "bitarray.h"
#include "hashmap.h"
#include "bitUtility.h"
#include "endianUtility.h"
#include "printUtility.h"


STATUS_CODE encodeHeader(FILE *gif) {
    size_t status;

    const char header[3]    = "GIF";
    const char version[3]   = "89a";
    u32 nmemb = 1;

    status = fwrite(header, sizeof(header), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);
    status = fwrite(version, sizeof(header), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    return OPERATION_SUCCESS;
}

/**
 * @param canvasWidth   -   in pixels 
 * @param canvasHeight  -   in pixels
 */
STATUS_CODE encodeLogicalScreenDescriptor(FILE *gif, u16 canvasWidth, u16 canvasHeight, u8 packedField, u8 backgroundColorIndex, u8 pixelAspectRatio) {
    size_t status;
    u32 nmemb = 1;

    canvasWidth = littleEndianU16(canvasWidth);
    status = fwrite(&canvasWidth, sizeof(canvasWidth), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    canvasHeight = littleEndianU16(canvasHeight);
    status = fwrite(&canvasHeight, sizeof(canvasHeight), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    // Packed field:
    // 7    - global color table flag
    // 4-6  - color resolution (2^(n+1) computes entries in global color table)
    // 3    - sort flag
    // 0-2  - size of global color table
    status = fwrite(&packedField, sizeof(packedField), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&backgroundColorIndex, sizeof(backgroundColorIndex), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&pixelAspectRatio, sizeof(pixelAspectRatio), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    return OPERATION_SUCCESS;
}

STATUS_CODE encodeGlobalColorTable(FILE *gif, colorTable *globalColorTable) {
    size_t status;
    u32 nmemb = 1;

    for (int i = 0; i < globalColorTable->size; i++) {
        RGB entry = globalColorTable->arr[i];
        status = fwrite(&entry.red, sizeof(u8), nmemb, gif);
        CHECK_FWRITE_STATUS(status, nmemb);
        
        status = fwrite(&entry.green,sizeof(u8), nmemb, gif);
        CHECK_FWRITE_STATUS(status, nmemb);

        status = fwrite(&entry.blue, sizeof(u8), nmemb, gif);
        CHECK_FWRITE_STATUS(status, nmemb);
    }

    return OPERATION_SUCCESS;
}

STATUS_CODE encodeImageDescriptor(FILE *gif) {
    size_t status;
    u32 nmemb = 1;

    u8 imageSeparator = 0x2C;
    u16 imageLeftPosition = 0x0000;
    u16 imageTopPosition  = 0x0000;
    //u16 imageWidth  = 24;
    u16 imageWidth  = 0x0A;
    //u16 imageHeight = 7;
    u16 imageHeight = 0x0A;
    u8 packedField = 0b00000000;

    status = fwrite(&imageSeparator, sizeof(imageSeparator), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&imageLeftPosition, sizeof(imageLeftPosition), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&imageTopPosition, sizeof(imageTopPosition), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    imageWidth = littleEndianU16(imageWidth);
    status = fwrite(&imageWidth, sizeof(imageWidth), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    imageHeight = littleEndianU16(imageHeight);
    status = fwrite(&imageHeight, sizeof(imageHeight), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&packedField, sizeof(packedField), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    return OPERATION_SUCCESS;
}


void checkCondidtionsForCodeSizeIncrease(bool *increaseCodeSizeFlag, u32 *entriesEqualCodeSize, u32 *currentCodeSize) {    
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

void addCodeStreamValueToImageData(bitarray *imageData, 
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

STATUS_CODE createLZWCodeStream(colorTable *clrTable, array *indexStream, bitarray *imageData) {
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


    freeArray(indexBuffer);
    freeCodeTable(codeTable);

    arrayPrint(codeStream);

    return OPERATION_SUCCESS;
}

STATUS_CODE encodeImageData(FILE *gif, colorTable *clrTable, array *indexStream) {
    size_t status;
    u32 nmemb = 1;

    bitarray *imageData = bitarrayInit(indexStream->size * 2);
    createLZWCodeStream(clrTable, indexStream, imageData);
    bitarrayPrint(imageData);
    
    for (u32 i = 0; i < imageData->currentIndex; i++) {
        status = fwrite(&(imageData->items[i]), sizeof(u8), nmemb, gif);
        CHECK_FWRITE_STATUS(status, nmemb);
    }

    return OPERATION_SUCCESS;
}


STATUS_CODE encodeTrailer(FILE *gif) {
    size_t status;
    u32 nmemb = 1;

    u8 endOfFileValue = 0x3B;
    status = fwrite(&endOfFileValue, sizeof(endOfFileValue), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    return OPERATION_SUCCESS;
}

void addRGBArrayEntry(RGB *table, u32 index, u8 red, u8 green, u8 blue) {
    table[index].red = red;
    table[index].green = green;
    table[index].blue = blue;
}

STATUS_CODE encodeGraphicsControlExtension(FILE *gif) {
    size_t status;
    u32 nmemb = 1;


}

STATUS_CODE createGIF() {
    size_t status;
    FILE *gif = fopen("test.gif","wb");
    
    status = encodeHeader(gif);
    CHECKSTATUS(status);

    //status = encodeLogicalScreenDescriptor(gif, 24, 7, 0b10000001, 0, 0);
    status = encodeLogicalScreenDescriptor(gif, 0x0A, 0x0A, 0b10010001, 0, 0);
    CHECKSTATUS(status);

    // temp for testing
    RGB *blackAndWhite = calloc(4, sizeof(RGB));
    addRGBArrayEntry(blackAndWhite, 0, 0xFF, 0xFF, 0xFF);
    addRGBArrayEntry(blackAndWhite, 1, 0xFF, 0x00, 0x00);
    addRGBArrayEntry(blackAndWhite, 2, 0x00, 0x00, 0xFF);
    addRGBArrayEntry(blackAndWhite, 3, 0x00, 0x00, 0x00);
    //addRGBArrayEntry(blackAndWhite, 0, 0, 0, 0);
    //addRGBArrayEntry(blackAndWhite, 1, 255, 255, 255);
    colorTable globalColorTable = { 4, blackAndWhite };

    // may need to pass in a gif struct that contains color resolution
    // for number of entries 
    status = encodeGlobalColorTable(gif, &globalColorTable);
    CHECKSTATUS(status);

    status = encodeImageDescriptor(gif);
    CHECKSTATUS(status);

    //status = encodeGraphicsControlExtension(gif);
    
    /* u8 indexStream[] = 
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,1,1,1,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,0,
     0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,
     0,1,1,1,0,1,1,0,0,1,1,0,0,1,0,0,1,0,1,1,0,0,1,0,
     0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,0,0,
     0,1,1,1,0,1,0,1,0,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; */

    u8 indexStream[] =
    { 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
      1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
      1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
      1, 1, 1, 0, 0, 0, 0, 2, 2, 2,
      1, 1, 1, 0, 0, 0, 0, 2, 2, 2,
      2, 2, 2, 0, 0, 0, 0, 1, 1, 1,
      2, 2, 2, 0, 0, 0, 0, 1, 1, 1,
      2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
      2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
      2, 2, 2, 2, 2, 1, 1, 1, 1, 1 };

    array *testArr = arrayInit(sizeof(indexStream));
    for (u32 i = 0; i < sizeof(indexStream); i++) {
        arrayAppend(testArr, indexStream[i]);
    }
    arrayPrint(testArr);

    status = encodeImageData(gif, &globalColorTable, testArr);
    CHECKSTATUS(status);

    status = encodeTrailer(gif);
    CHECKSTATUS(status);

    return OPERATION_SUCCESS;
}
