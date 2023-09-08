
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFEncode.h"
#include "memoryManagement.h"

static void expandFrame(GIFFrame *frame, u32 widthMuliplier, u32 heightMuliplier) {
    u16 oldWidth  = frame->imageWidth;
    u16 oldHeight = frame->imageHeight;

    u16 newWidth  = oldWidth  * widthMuliplier;
    u16 newHeight = oldHeight * heightMuliplier;
  
    frame->imageWidth   = newWidth;
    frame->imageHeight  = newHeight;

    u32 newArraySize = newWidth * newHeight;
    array *newIndexStream = arrayInit(newArraySize);

    for (int i = 0; i < oldHeight; i++) {
        for (int j = 0; j < oldWidth; j++) {
            for (int k = 0; k < widthMuliplier; k++) {
                for (int l = 0; l < heightMuliplier; l++) {
                    // 
                    // current frame entry: (i * oldWidth) + j
                    // 
                    // new frame entry transformations:
                    // x axis: (j * widthMuliplier) + k 
                    //         (j * widthMuliplier): get starting x position in new array
                    //         + k: repeat the pixel (factor multipler) times in the x axis
                    // 
                    // y axis: (((i * heightMuliplier) + l) * newWidth)
                    //         (((i * heightMuliplier): get starting y position in new array
                    //         + l): repeat the pixel (factor multiper) times in the y axis
                    //         * newWidth): skip already written rows in new array
                    //
                    newIndexStream->items[(((i * heightMuliplier) + l) * newWidth) + (j * widthMuliplier) + k] = frame->indexStream->items[(i * oldWidth) + j];
                }
            }
        }
    }

    for (int i = 0; i < newArraySize && newWidth < 100; i++) {
        printf("%d", newIndexStream->items[i]);
        if (i % newWidth == newWidth - 1) {
            printf("\n");
        }
    }

    freeArray(frame->indexStream);
    frame->indexStream = newIndexStream;
}

static void expandCanvas(GIFCanvas *canvas, u32 widthMuliplier, u32 heightMuliplier) {
    
}

STATUS_CODE createTestGif() {
    
    GIFCanvas *canvas = canvasCreate(0x0A, 0x0A);
    canvasAddGlobalColorTable();

    GIFFrame *frame1 = frameCreate(0x0A, 0x0A, 0x00, 0x00);
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

    // u32 widthMuliplier  = 6;
    // u32 heightMuliplier = 8;
    u32 widthMuliplier  = 192;
    u32 heightMuliplier = 108;
    // u32 widthMuliplier  = 150;
    // u32 heightMuliplier = 150;
    
    array *newFrame = arrayInit((widthMuliplier * gifData->canvasWidth) * (heightMuliplier * gifData->canvasHeight));
    expandFrame(newFrame, gifData, (u8*) &indexStreamFrameOne, widthMuliplier, heightMuliplier);

    gifData->indexStream = newFrame;

    RGB *colorsArray = calloc(4, sizeof(RGB));
    addRGBArrayEntry(colorsArray, 0, 0xFF, 0xFF, 0xFF);
    addRGBArrayEntry(colorsArray, 1, 0xFF, 0x00, 0x00);
    addRGBArrayEntry(colorsArray, 2, 0x00, 0x00, 0xFF);
    addRGBArrayEntry(colorsArray, 3, 0x00, 0x00, 0x00);
    colorTable *globalColorTable = calloc(1, sizeof(colorTable));
    globalColorTable->size = 4; globalColorTable->arr = colorsArray;
    gifData->globalColorTable = globalColorTable;

    encodeGIF(gifData);

    freeGifData(gifData);
}

STATUS_CODE createErrorGif() {
    GIFCanvas *gifData = calloc(1, sizeof(GIFCanvas));

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

    u32 widthMuliplier  = 50;
    u32 heightMuliplier = 50;
    
    array *newFrame = arrayInit((widthMuliplier * gifData->canvasWidth) * (heightMuliplier * gifData->canvasHeight));
    expandFrame(newFrame, gifData, (u8*) &indexStreamFrameOne, widthMuliplier, heightMuliplier);

    gifData->indexStream = newFrame;

    RGB *colorsArray = calloc(4, sizeof(RGB));
    addRGBArrayEntry(colorsArray, 0, 0, 0, 0);
    addRGBArrayEntry(colorsArray, 1, 255, 255, 255);
    // When colors occupy less than the total size
    // they need to be filled with fake (or 0) values
    addRGBArrayEntry(colorsArray, 2, 0, 255, 255);
    addRGBArrayEntry(colorsArray, 3, 255, 0, 255);
    colorTable *globalColorTable = calloc(1, sizeof(colorTable));
    globalColorTable->size = 4; globalColorTable->arr = colorsArray;
    gifData->globalColorTable = globalColorTable;

    encodeGIF(gifData);

    freeGifData(gifData);
}
