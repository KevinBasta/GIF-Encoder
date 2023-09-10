
#include <stdlib.h>
#include <stdio.h>

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

STATUS_CODE expandCanvas(GIFCanvas *canvas, u32 widthMuliplier, u32 heightMuliplier) {
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
