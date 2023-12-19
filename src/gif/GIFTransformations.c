
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "searchUtility.h"
#include "GIFInterface.h"

// Expands a single frame in the x and y axis
static void gif_expandFrame(GIFFrame *frame, u32 widthMuliplier, u32 heightMuliplier) {
    u16 oldWidth  = frame->imageWidth;
    u16 oldHeight = frame->imageHeight;

    u16 newWidth  = oldWidth  * widthMuliplier;
    u16 newHeight = oldHeight * heightMuliplier;
  
    if (newWidth > INT16_MAX || newHeight > INT16_MAX)
        return;

    frame->imageWidth   = newWidth;
    frame->imageHeight  = newHeight;

    frame->imageLeftPosition = frame->imageLeftPosition * widthMuliplier;
    frame->imageTopPosition  = frame->imageTopPosition * heightMuliplier;

    u32 newArraySize = newWidth * newHeight;
    gif_array *newIndexStream = gif_arrayInit(newArraySize);

    for (u16 i = 0; i < oldHeight; i++) {
        for (u16 j = 0; j < oldWidth; j++) {
            for (u16 k = 0; k < widthMuliplier; k++) {
                for (u16 l = 0; l < heightMuliplier; l++) {
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

    for (int i = 0; i < newArraySize && newWidth < 50 && newHeight < 50; i++) {
        PRINTF("%d", newIndexStream->items[i]);
        if (i % newWidth == newWidth - 1) {
            PRINTF("\n");
        }
    }

    gif_freeArray(frame->indexStream);
    frame->indexStream = newIndexStream;
}

// Expand the canvas frame index streams in the x and y axis
WASM_EXPORT STATUS_CODE gif_expandCanvas(GIFCanvas *canvas, u32 widthMuliplier, u32 heightMuliplier) {
    STATUS_CODE status;
    CANVAS_NULL_CHECK(canvas);

    size_t newCanvasWidth  = canvas->canvasWidth * widthMuliplier;
    size_t newCanvasHeight = canvas->canvasHeight * heightMuliplier;

    if (newCanvasHeight > INT16_MAX || newCanvasWidth > INT16_MAX)
        return OPERATION_FAILED;

    canvas->canvasWidth     = newCanvasWidth;
    canvas->canvasHeight    = newCanvasHeight;
        
    GIFFrame **framesExpanded = calloc(canvas->frames->size, sizeof(GIFFrame*));
    size_t i = 0;

    GIFFrame *frame;
    gif_linkedlistResetIter(canvas->frames);
    status = gif_linkedlistYield(canvas->frames, (void**) (&frame));
    CHECKSTATUS(status);

    while (frame != NULL) {
        if (!gif_frameInArray(frame, framesExpanded, i)) {
            gif_expandFrame(frame, widthMuliplier, heightMuliplier);
            framesExpanded[i] = frame;
            i++;
        }
        
        status = gif_linkedlistYield(canvas->frames, (void**) (&frame));
        CHECKSTATUS(status);
    }

    gif_linkedlistResetIter(canvas->frames);

    free(framesExpanded);

    return OPERATION_SUCCESS;
}

/**
 * @brief Expand the indexStream of a frame by appending
 * another array to the left of it. Old frame index stream is
 * freed and replaced by the new one. Caller is responsibe for
 * freeing the "arrayToAppend" if it is stored on the heap. 
 * @param frame                     Frame to make new indexStream for
 * @param arrayToAppend             Array to append to the left of the
 * frame's index stream
 * @param widthOfAppendingArray     Width (signifies pixels)
 * @param heightOfAppendingArray    Height (signifies pixels)
 * @param trailingRows              Empty rows to add to the end
 * @param valueToUseForOverflow     Color table value to use for overflow
 *
 * @return OPERATION_SUCCESS or error code
 */
WASM_EXPORT STATUS_CODE gif_appendToFrame(GIFFrame *frame,
                          u8 *arrayToAppend, 
                          u32 widthOfAppendingArray, 
                          u32 heightOfAppendingArray, 
                          u32 trailingRows, 
                          u8 valueToUseForOverflow) {
    STATUS_CODE status;
    FRAME_NULL_CHECK(frame);

    gif_array *oldIndexStream       = frame->indexStream;
    oldIndexStream->currentIndex = 0;

    u32 oldIndexStreamWidth     = frame->imageWidth;
    u32 oldIndexStreamHeight    = frame->imageHeight;

    u32 heightDifference = max((heightOfAppendingArray + trailingRows), oldIndexStreamHeight) - 
                           min((heightOfAppendingArray + trailingRows), oldIndexStreamHeight);

    u32 newIndexStreamWidth  = frame->imageWidth + widthOfAppendingArray;
    u32 newIndexStreamHeight = frame->imageHeight + heightDifference;
    u32 newIndexStreamLength = newIndexStreamWidth * newIndexStreamHeight;

    gif_array *newIndexStream     = gif_arrayInit(newIndexStreamLength);
    ARRAY_NULL_CHECK(newIndexStream);

    u32 appendingArrayCounter = 0;
    for (u32 row = 0; row < newIndexStreamHeight; row++) {
        for (u32 column = 0; column < newIndexStreamWidth; column++) {
            if (column < oldIndexStreamWidth) {
                if (row > oldIndexStreamHeight - 1) {
                    status = gif_arrayAppend(newIndexStream, valueToUseForOverflow);
                } else {
                    status = gif_arrayAppend(newIndexStream, gif_arrayGetIncrement(oldIndexStream));
                }
            } else {
                if (row > heightOfAppendingArray - 1) {
                    status = gif_arrayAppend(newIndexStream, valueToUseForOverflow);
                } else {
                    status = gif_arrayAppend(newIndexStream, arrayToAppend[appendingArrayCounter]);
                }
                appendingArrayCounter++;
            }

            CHECKSTATUS(status);
        }
    }

    /* for (int i = 0; i < newIndexStreamLength; i++) {
        PRINTF("%d", newIndexStream->items[i]);
        if (i % newIndexStreamWidth == newIndexStreamWidth - 1) {
            PRINTF("\n");
        }
    } */

    gif_freeArray(oldIndexStream);
    frame->indexStream  = newIndexStream;
    frame->imageWidth   = newIndexStreamWidth;
    frame->imageHeight  = newIndexStreamHeight;

    return OPERATION_SUCCESS;
}
