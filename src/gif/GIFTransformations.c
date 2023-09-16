
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFInterface.h"


static void expandFrame(GIFFrame *frame, u32 widthMuliplier, u32 heightMuliplier) {
    u16 oldWidth  = frame->imageWidth;
    u16 oldHeight = frame->imageHeight;

    u16 newWidth  = oldWidth  * widthMuliplier;
    u16 newHeight = oldHeight * heightMuliplier;
  
    frame->imageWidth   = newWidth;
    frame->imageHeight  = newHeight;

    frame->imageLeftPosition = frame->imageLeftPosition * widthMuliplier;
    frame->imageTopPosition  = frame->imageTopPosition * heightMuliplier;

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

// Expand the canvas frame index streams in the x and y axis
STATUS_CODE expandCanvas(GIFCanvas *canvas, u32 widthMuliplier, u32 heightMuliplier) {
    STATUS_CODE status;
    CANVAS_NULL_CHECK(canvas);

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


STATUS_CODE appendToFrame(GIFFrame *frame,
                          u8 *arrayToAppend, 
                          u32 widthOfAppendingArray, 
                          u32 heightOfAppendingArray, 
                          u32 trailingRows, 
                          u8 valueToUseForOverflow) {
    STATUS_CODE status;
    FRAME_NULL_CHECK(frame);

    array *oldIndexStream       = frame->indexStream;
    oldIndexStream->currentIndex = 0;

    u32 oldIndexStreamWidth     = frame->imageWidth;
    u32 oldIndexStreamHeight    = frame->imageHeight;

    u32 heightDifference = max((heightOfAppendingArray + trailingRows), oldIndexStreamHeight) - 
                           min((heightOfAppendingArray + trailingRows), oldIndexStreamHeight);
    printf("HEIGHT DIFF %d\n", heightDifference);

    u32 newIndexStreamWidth  = frame->imageWidth + widthOfAppendingArray;
    u32 newIndexStreamHeight = frame->imageHeight + heightDifference;
    u32 newIndexStreamLength = newIndexStreamWidth * newIndexStreamHeight;

    array *newIndexStream     = arrayInit(newIndexStreamLength);

    // u32 oldIndexStreamCounter = 0;
    u32 appendingArrayCounter = 0;
    for (u32 row = 0; row < newIndexStreamHeight; row++) {
        for (u32 column = 0; column < newIndexStreamWidth; column++) {
            printf("%d %d\n", row, column);
            if (column < oldIndexStreamWidth) {
                if (row > oldIndexStreamHeight - 1) {
                    arrayAppend(newIndexStream, valueToUseForOverflow);
                } else {
                    arrayAppend(newIndexStream, arrayGetIncrement(oldIndexStream));
                }
            } else {
                if (row > heightOfAppendingArray - 1) {
                    arrayAppend(newIndexStream, valueToUseForOverflow);
                } else {
                    arrayAppend(newIndexStream, arrayToAppend[appendingArrayCounter]);
                }
                appendingArrayCounter++;
            }
        }
    }

    for (int i = 0; i < newIndexStreamLength; i++) {
        printf("%d", newIndexStream->items[i]);
        if (i % newIndexStreamWidth == newIndexStreamWidth - 1) {
            printf("\n");
        }
    }


    freeArray(oldIndexStream);
    frame->indexStream  = newIndexStream;
    frame->imageWidth   = newIndexStreamWidth;
    frame->imageHeight  = newIndexStreamHeight;

    return OPERATION_SUCCESS;
}