
#ifndef GIF_TRANSFORMATIONS_H
#define GIF_TRANSFORMATIONS_H

#include <stdint.h>
#include "main.h"

#include "GIFInterface.h"

STATUS_CODE expandCanvas(GIFCanvas *canvas, u32 widthMuliplier, u32 heightMuliplier);
STATUS_CODE appendToFrame(GIFFrame *frame, u8 *arrayToAppend, u32 widthOfAppendingArray, u32 heightOfAppendingArray, u32 trailingRows, u8 valueToUseForOverflow);

#endif // GIF_TRANSFORMATIONS_H