#ifndef COMMON_HEAD
    #define COMMON_HEAD
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <math.h>
    #include <time.h>
    #include "main.h"
#endif //COMMON_HEAD

#ifndef COMMON_TYPES
    #define COMMON_TYPES
    #include "typesMPEG-4.h"
    #include "typesAVC.h"
    #include "typesGIF.h"
    #include "typesUtility.h"
#endif //COMMON_TYPES

#ifndef DATA_STRUCTURES
    #define DATA_STRUCTURES
    #include "linkedList.h"
    #include "hashmap.h"
    #include "array.h"
    #include "typesStorage.h"
#endif //DATA_STRUCTURES

#ifndef COMMON_UTIL
    #define COMMON_UTIL
    #include "bitUtility.h"
    #include "endianUtility.h"
    #include "printUtility.h"
    #include "memoryManagement.h"
#endif //COMMON_UTIL


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


char* intToString(size_t i) {
    u32 intStrLen = snprintf(NULL, 0, "%ld", i);
    char *str     = malloc( intStrLen + 1 );
    snprintf(str, intStrLen, "%ld", i);

    return str;
}

char* concatArray(array *arr, char entrySeparator) {
    char *concat = malloc(sizeof(char) * (arr->size * 2));
    for (size_t i = 0; i < arr->size; i += 2) {
        char *itemStr = intToString(arr->items[i]);
        concat[i] = *(itemStr);
        free(itemStr);
        concat[i + 1] = entrySeparator;
    }

    concat[arr->size * 2] = "\0";

    return concat;
}

codeTable* initCodeTable(colorTable *clrTable) {
    codeTable *table = calloc(1, sizeof(codeTable));
    HashMap *map = initHashMap(clrTable->size * 30);
    table->map = map;

    for (size_t i = 0; i < clrTable->size; i++) {
        char *str = intToString(i);
        insertHashMap(map, str, str);

        table->index = i;
    }

    return table;
}

void freeCodeTable(codeTable *table) {
    freeHashMap(table->map);
    free(table);
}


STATUS_CODE createLZWCodeStream(array *indexStream, colorTable *clrTable) {
    array *codeStream  = initArray(indexStream->size);
    array *indexBuffer = initArray(indexStream->size);

    codeTable *codeTable = initCodeTable(clrTable);


    appendArray(codeStream, atoi(searchHashMap(codeTable, "cc")));
    appendArray(indexBuffer, getItemArray(indexStream, 0));
    for (size_t i = 1; i < indexStream->size; i++) {
        u32 k = getItemArray(indexStream, i);

        appendArray(indexBuffer, k);
        char *indexBufferKey = concatArray(indexBuffer, ',');

        if (searchHashMap(codeStream, indexBufferKey) == NULL) {
            popArray(indexBuffer, k);

        }
    }




}

STATUS_CODE encodeImageData(FILE *gif, colorTable *clrTable, array *indexStream) {
    size_t status;
    u32 nmemb = 1;

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

    //status = encodeImageData(gif, globalColorTable, testArr);
    CHECKSTATUS(status);

    status = encodeTrailer(gif);
    CHECKSTATUS(status);

    return OPERATION_SUCCESS;
}
