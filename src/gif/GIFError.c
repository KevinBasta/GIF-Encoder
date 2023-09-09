
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFEncode.h"

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

static STATUS_CODE expandCanvas(GIFCanvas *canvas, u32 widthMuliplier, u32 heightMuliplier) {
    STATUS_CODE status;

    canvas->canvasWidth     = canvas->canvasWidth * widthMuliplier;
    canvas->canvasHeight    = canvas->canvasHeight * heightMuliplier;

    GIFFrame *frame;
    status = linkedlistYield(canvas->frames, (void**) (&frame));
    CHECKSTATUS(status);

    while (frame != NULL) {
        expandFrame(frame, widthMuliplier, heightMuliplier);
        
        status = linkedlistYield(canvas->frames, (void**) (&frame));
        CHECKSTATUS(status);
    }

    return OPERATION_SUCCESS;
}

STATUS_CODE createTestGif() {
    STATUS_CODE status;

    // create color table
    colorTable *globalColorTable = colortableInit(3);
    status = colortableAppendRGB(globalColorTable, 0xFF, 0xFF, 0xFF);       
    CHECKSTATUS(status);
    status = colortableAppendRGB(globalColorTable, 0xFF, 0x00, 0x00); 
    CHECKSTATUS(status);
    status = colortableAppendRGB(globalColorTable, 0x00, 0x00, 0xFF); 
    CHECKSTATUS(status);

    // create canvas
    GIFCanvas *canvas = canvasCreate(0x0A, 0x0A);
    status = canvasAddGlobalColorTable(canvas, globalColorTable);   
    CHECKSTATUS(status);
    status = canvasSetBackgroundColorIndex(canvas, 0x00);           
    CHECKSTATUS(status);

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
    GIFFrame *frameOne = frameCreate(24, 7, 0, 0);
    status = frameAddIndexStream(frameOne, frameOneIndexStream);
    CHECKSTATUS(status);

    canvasAddFrame(canvas, frameOne);
    
    u32 widthMuliplier  = 192; u32 heightMuliplier = 108;
    expandCanvas(canvas, widthMuliplier, heightMuliplier);

    encodeGIF(canvas);

    freeCanvas(canvas);

    return OPERATION_SUCCESS;
}

STATUS_CODE createErrorGif() {
    STATUS_CODE status;

    // create color table
    colorTable *globalColorTable = colortableInit(2);
    status = colortableAppendRGB(globalColorTable, 0, 0, 0);       
    CHECKSTATUS(status);
    status = colortableAppendRGB(globalColorTable, 255, 255, 255); 
    CHECKSTATUS(status);

    // create canvas
    GIFCanvas *canvas = canvasCreate(24, 7);
    status = canvasAddGlobalColorTable(canvas, globalColorTable);   
    CHECKSTATUS(status);
    status = canvasSetBackgroundColorIndex(canvas, 0x00);           
    CHECKSTATUS(status);

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
    status = frameAddIndexStream(frameOne, frameOneIndexStream);
    CHECKSTATUS(status);

    canvasAddFrame(canvas, frameOne);

    u32 widthMuliplier  = 50; u32 heightMuliplier = 50;
    expandCanvas(canvas, widthMuliplier, heightMuliplier);

    encodeGIF(canvas);

    freeCanvas(canvas);

    return OPERATION_SUCCESS;
}
