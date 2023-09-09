
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
    u8 frameOneTempIndexStream[] =
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
    array *frameOneIndexStream = arrayInitFromStackArray((u8*)&frameOneTempIndexStream, sizeof(frameOneTempIndexStream));
    
    GIFFrame *frameOne = frameCreate(0x0A, 0x0A, 0, 0);
    frameAddIndexStream(frameOne, frameOneIndexStream);

    u32 widthMuliplier  = 192; u32 heightMuliplier = 108;
    expandFrame(frameOne, widthMuliplier, heightMuliplier);


    RGB *colorsArray = calloc(4, sizeof(RGB));
    addRGBArrayEntry(colorsArray, 0, 0xFF, 0xFF, 0xFF);
    addRGBArrayEntry(colorsArray, 1, 0xFF, 0x00, 0x00);
    addRGBArrayEntry(colorsArray, 2, 0x00, 0x00, 0xFF);
    addRGBArrayEntry(colorsArray, 3, 0x00, 0x00, 0x00);
    colorTable *globalColorTable = calloc(1, sizeof(colorTable));
    globalColorTable->size = 4; globalColorTable->arr = colorsArray;

    GIFCanvas *canvas = canvasCreate(0x0A, 0x0A);
    canvasAddGlobalColorTable(canvas, globalColorTable);
    canvasSetBackgroundColorIndex(canvas, 0x00);
    canvasAddFrame(canvas, frameOne);

    encodeGIF(canvas);

    freeCanvas(canvas);
}

STATUS_CODE createErrorGif() {
    u8 frameOneTempIndexStream[] = 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,1,1,1,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,0,
      0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,
      0,1,1,1,0,1,1,0,0,1,1,0,0,1,0,0,1,0,1,1,0,0,1,0,
      0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,0,0,
      0,1,1,1,0,1,0,1,0,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    array *frameOneIndexStream = arrayInitFromStackArray((u8*)&frameOneTempIndexStream, sizeof(frameOneTempIndexStream));
    GIFFrame *frameOne = frameCreate(24, 7, 0, 0);
    frameAddIndexStream(frameOne, frameOneIndexStream);

    u32 widthMuliplier  = 50; u32 heightMuliplier = 50;
    expandFrame(frameOne, widthMuliplier, heightMuliplier);
    
    RGB *colorsArray = calloc(4, sizeof(RGB));
    addRGBArrayEntry(colorsArray, 0, 0, 0, 0);
    addRGBArrayEntry(colorsArray, 1, 255, 255, 255);
    // When colors occupy less than the total size
    // they need to be filled with fake (or 0) values
    addRGBArrayEntry(colorsArray, 2, 0, 255, 255);
    addRGBArrayEntry(colorsArray, 3, 255, 0, 255);
    colorTable *globalColorTable = calloc(1, sizeof(colorTable));
    globalColorTable->size = 4; globalColorTable->arr = colorsArray;

    GIFCanvas *canvas = canvasCreate(24, 7);
    canvasAddGlobalColorTable(canvas, globalColorTable);
    canvasSetBackgroundColorIndex(canvas, 0x00);
    canvasAddFrame(canvas, frameOne);

    encodeGIF(canvas);

    freeCanvas(canvas);
}
