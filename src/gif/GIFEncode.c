
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

STATUS_CODE createLZWCodeStream(colorTable *clrTable, array *indexStream, array *codeStream) {
    STATUS_CODE status;
    
    array *indexBuffer = arrayInit(indexStream->size);

    codeTable *codeTable = initCodeTable(clrTable);

    // Put the clear code in the code stream
    char *clearCodeValue = hashmapSearch(codeTable->map, "cc");
    CHECK_NULL_RETURN(clearCodeValue);

    status = arrayAppend(codeStream, atoi(clearCodeValue));
    CHECKSTATUS(status);

    // Add first element of index stream to index buffer
    status = arrayAppend(indexBuffer, arrayGetItem(indexStream, 0));
    CHECKSTATUS(status);

    for (size_t i = 1; i < indexStream->size; i++) {
        // Add next index stream entry to index buffer temporarily
        u32 k = arrayGetItem(indexStream, i);
        status = arrayAppend(indexBuffer, k);
        CHECKSTATUS(status);

        printf("================================\n");
        char *indexBufferPlusKKey = arrayConcat(indexBuffer, ',');

        if (hashmapSearch(codeTable->map, indexBufferPlusKKey) == NULL) {
            printf("index buffer + k not in hash map\n");
            
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

            /**
             Need to handle code table having max number of entries
             4095. Send clear code and reset table?
            */
        } else {
            free(indexBufferPlusKKey);
        }
        
        //hashmapPrint(codeTable->map);
    }

    char *endOfInfoValue = hashmapSearch(codeTable->map, "eoi");
    CHECK_NULL_RETURN(endOfInfoValue);

    status = arrayAppend(codeStream, atoi(endOfInfoValue));
    CHECKSTATUS(status);

    freeArray(indexBuffer);
    freeCodeTable(codeTable);

    arrayPrint(codeStream);

    return OPERATION_SUCCESS;
}

STATUS_CODE codeStreamFlexibleCodeSizes(colorTable *clrTable, array *codeStream, bitarray *imageData) {
    STATUS_CODE status;

    u8 currentCodeSize = getLWZMinCodeSize(clrTable->size);
    bitarrayAppend(imageData, currentCodeSize);
    bitarrayBookMark(imageData); // Mark to put number of bytes of data
    bitarrayAppend(imageData, 0);

    u8 numberOfBytesOfDataInSubBlock = 0;
    u32 startByte = imageData->currentIndex;

    for (size_t i = 0; i < codeStream->currentIndex; i++) {
        bitarrayPrint(imageData);
        printf("%d\n", codeStream->items[i]);
        u32 occupiedBits = getOccupiedBits(codeStream->items[i]);

        if (pow(2, currentCodeSize) == codeStream->items[i]) {
            // append the next code with new code size
            bitarrayAppendPackedNormalized(imageData, codeStream->items[i], occupiedBits, currentCodeSize);
            
            // Set the second byte in the image data block
            numberOfBytesOfDataInSubBlock = imageData->currentIndex - startByte + 1;
            bitarraySetBookMarkValue(imageData, numberOfBytesOfDataInSubBlock);
            
            // Image block terminator
            bitarrayAppend(imageData, 0);

            // Start next image data block
            currentCodeSize++;
            bitarrayAppend(imageData, currentCodeSize);
            bitarrayBookMark(imageData); // Mark to put number of bytes of data
            bitarrayAppend(imageData, 0);
            startByte = imageData->currentIndex;
        } else {
            bitarrayAppendPackedNormalized(imageData, codeStream->items[i], occupiedBits, currentCodeSize);
        }
    }

    // Set the second byte in the image data block
    numberOfBytesOfDataInSubBlock = imageData->currentIndex - startByte;
    bitarraySetBookMarkValue(imageData, numberOfBytesOfDataInSubBlock);
            
    // Image block terminator
    bitarrayAppend(imageData, 0);



    return OPERATION_SUCCESS;
}



STATUS_CODE encodeImageData(FILE *gif, colorTable *clrTable, array *indexStream) {
    size_t status;
    u32 nmemb = 1;

    array *codeStream = arrayInit(indexStream->size);
    bitarray *imageData = bitarrayInit(codeStream->size * 2);
    createLZWCodeStream(clrTable, indexStream, codeStream);
    codeStreamFlexibleCodeSizes(clrTable, codeStream, imageData);
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
