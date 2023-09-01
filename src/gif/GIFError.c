
#include <stdlib.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "typesGIF.h"
#include "typesStorage.h"
#include "GIFEncode.h"

STATUS_CODE createTestGif() {
    GIF_Data *gifData = calloc(1, sizeof(GIF_Data));
    
    gifData->canvasWidth            = 0x0A;
    gifData->canvasHeight           = 0x0A;
    gifData->packedFieldCanvas      = 0b10010001;
    gifData->backgroundColorIndex   = 0x00;
    gifData->pixelAspectRatio       = 0x00;

    gifData->imageLeftPosition  = 0x0000;
    gifData->imageTopPosition   = 0x0000;
    gifData->imageWidth         = 0x0A;
    gifData->imageHeight        = 0x0A;
    gifData->packedFieldImage   = 0b00000000;

    u8 indexStreamFrameOne[] =
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

    array *frameOne = arrayInit(sizeof(indexStreamFrameOne));
    for (u32 i = 0; i < sizeof(indexStreamFrameOne); i++) {
        arrayAppend(frameOne, indexStreamFrameOne[i]);
    }

    gifData->indexStream = frameOne;

    RGB *colorsArray = calloc(4, sizeof(RGB));
    addRGBArrayEntry(colorsArray, 0, 0xFF, 0xFF, 0xFF);
    addRGBArrayEntry(colorsArray, 1, 0xFF, 0x00, 0x00);
    addRGBArrayEntry(colorsArray, 2, 0x00, 0x00, 0xFF);
    addRGBArrayEntry(colorsArray, 3, 0x00, 0x00, 0x00);
    colorTable *globalColorTable = calloc(1, sizeof(colorTable));
    globalColorTable->size = 4; globalColorTable->arr = colorsArray;
    gifData->globalColorTable = globalColorTable;

    createGIF(gifData);
}


STATUS_CODE createErrorGif() {
    GIF_Data *gifData = calloc(1, sizeof(GIF_Data));

    gifData->canvasWidth            = 24;
    gifData->canvasHeight           = 7;
    gifData->packedFieldCanvas      = 0b10010001;
    gifData->backgroundColorIndex   = 0x00;
    gifData->pixelAspectRatio       = 0x00;

    gifData->imageLeftPosition  = 0x0000;
    gifData->imageTopPosition   = 0x0000;
    gifData->imageWidth         = 24;
    gifData->imageHeight        = 7;
    gifData->packedFieldImage   = 0b00000000;

    u8 indexStreamFrameOne[] = 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,1,1,1,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,0,
      0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,
      0,1,1,1,0,1,1,0,0,1,1,0,0,1,0,0,1,0,1,1,0,0,1,0,
      0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,0,0,
      0,1,1,1,0,1,0,1,0,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    array *frameOne = arrayInit(sizeof(indexStreamFrameOne));
    for (u32 i = 0; i < sizeof(indexStreamFrameOne); i++) {
        arrayAppend(frameOne, indexStreamFrameOne[i]);
    }

    gifData->indexStream = frameOne;

    RGB *colorsArray = calloc(2, sizeof(RGB));
    addRGBArrayEntry(colorsArray, 0, 0, 0, 0);
    addRGBArrayEntry(colorsArray, 1, 255, 255, 255);
    colorTable *globalColorTable = calloc(1, sizeof(colorTable));
    globalColorTable->size = 2; globalColorTable->arr = colorsArray;
    gifData->globalColorTable = globalColorTable;

  createGIF(gifData);
}