
#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

#include "typesGIF.h"
#include "codeTable.h"

#include "hashmap.h"
#include "array.h"
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


STATUS_CODE encodeGraphicsControlExtension(FILE *gif) {
    size_t status;
    u32 nmemb = 1;


}

STATUS_CODE encodeImageDescriptor(FILE *gif) {
    size_t status;
    u32 nmemb = 1;

    u8 imageSeparator = 0x2C;
    u16 imageLeftPosition = 0x0000;
    u16 imageTopPosition  = 0x0000;
    u16 imageWidth  = 24;
    u16 imageHeight = 7;
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

STATUS_CODE createLZWCodeStream(array *indexStream, colorTable *clrTable) {
    STATUS_CODE status;
    
    array *codeStream  = initArray(indexStream->size);
    array *indexBuffer = initArray(indexStream->size);

    codeTable *codeTable = initCodeTable(clrTable);

    // Put the clear code in the code stream
    char *clearCodeValue = searchHashMap(codeTable->map, "cc");
    CHECK_NULL_RETURN(clearCodeValue);

    status = appendArray(codeStream, atoi(clearCodeValue));
    CHECKSTATUS(status);

    // Add first element of index stream to index buffer
    status = appendArray(indexBuffer, getItemArray(indexStream, 0));
    CHECKSTATUS(status);

    for (size_t i = 1; i < indexStream->size; i++) {
        // Add next index stream entry to index buffer temporarily
        u32 k = getItemArray(indexStream, i);
        status = appendArray(indexBuffer, k);
        CHECKSTATUS(status);

        printf("================================\n");
        char *indexBufferPlusKKey = concatArray(indexBuffer, ',');

        if (searchHashMap(codeTable->map, indexBufferPlusKKey) == NULL) {
            printf("index buffer + k not in hash map\n");
            
            // Add entry for index buffer + k in code table
            // with value as the next smallest code table index
            u32 test = getNextIndexCodeTable(codeTable);
            char *next = intToString(test);

            status = insertHashMap(codeTable->map, indexBufferPlusKKey, next);
            CHECKSTATUS(status);

            // Add the code for just the index buffer to the code stream
            status = popArray(indexBuffer);
            CHECKSTATUS(status);

            char *indexBufferKey = concatArray(indexBuffer, ',');
            char *value = searchHashMap(codeTable->map, indexBufferKey);
            CHECK_NULL_RETURN(value);

            status = appendArray(codeStream, atoi(value));
            CHECKSTATUS(status);
            free(indexBufferKey);

            // Set index buffer to k
            resetArray(indexBuffer);
            appendArray(indexBuffer, k);

            /**
             Need to handle code table having max number of entries
             4095. Send clear code and reset table?
            */
        } else {
            free(indexBufferPlusKKey);
        }
        
        //printHashMap(codeTable->map);
    }

    char *endOfInfoValue = searchHashMap(codeTable->map, "eoi");
    CHECK_NULL_RETURN(endOfInfoValue);

    status = appendArray(codeStream, atoi(endOfInfoValue));
    CHECKSTATUS(status);

    freeArray(indexBuffer);
    freeCodeTable(codeTable);

    printArray(codeStream);

    return OPERATION_SUCCESS;
}

STATUS_CODE encodeImageData(FILE *gif, colorTable *clrTable, array *indexStream) {
    size_t status;
    u32 nmemb = 1;

    createLZWCodeStream(indexStream, clrTable);
    //globalColorTable->size

    
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

STATUS_CODE createGIF() {
    size_t status;
    FILE *gif = fopen("test.gif","wb");
    
    status = encodeHeader(gif);
    CHECKSTATUS(status);

    status = encodeLogicalScreenDescriptor(gif, 24, 7, 0b10000001, 0, 0);
    CHECKSTATUS(status);

    // temp for testing
    RGB *blackAndWhite = calloc(2, sizeof(RGB));
    addRGBArrayEntry(blackAndWhite, 0, 0, 0, 0);
    addRGBArrayEntry(blackAndWhite, 1, 255, 255, 255);
    colorTable globalColorTable = { 2, blackAndWhite };

    // may need to pass in a gif struct that contains color resolution
    // for number of entries 
    status = encodeGlobalColorTable(gif, &globalColorTable);
    CHECKSTATUS(status);

    status = encodeImageDescriptor(gif);
    CHECKSTATUS(status);

    u8 indexStream[] = 
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,1,1,1,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,0,
     0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,
     0,1,1,1,0,1,1,0,0,1,1,0,0,1,0,0,1,0,1,1,0,0,1,0,
     0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,0,0,
     0,1,1,1,0,1,0,1,0,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    array *testArr = initArray(sizeof(indexStream));
    for (u32 i = 0; i < sizeof(indexStream); i++) {
        appendArray(testArr, indexStream[i]);
    }
    printArray(testArr);

    status = encodeImageData(gif, &globalColorTable, testArr);
    CHECKSTATUS(status);

    status = encodeTrailer(gif);
    CHECKSTATUS(status);

    return OPERATION_SUCCESS;
}
