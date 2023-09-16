
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFEncode.h"
#include "GIFTransformations.h"
#include "printUtility.h"

#include "lettersAndNumbers.h"

STATUS_CODE createErrorGif(u32 errorCode) {
    // creates a gif that displays
    // the word "ERROR!" in case the main
    // program goes wrong somewhere
    // also serves as a good test
    STATUS_CODE status;

    // create canvas
    GIFCanvas *canvas = canvasCreate(MAX_LETTER_PIXEL_WIDTH, ROW_HEIGHT_IN_PIXELS);
    status = canvasCreateGlobalColorTable(canvas);              CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 0, 0, 0);       CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 255, 255, 255); CHECKSTATUS(status);
    status = canvasSetBackgroundColorIndex(canvas, 0);          CHECKSTATUS(status);

    // FRAME #1
    char *errorString = "ERROR ";
    letterPattern *pattern = getLetterOrNumber(errorString[0]);
    GIFFrame *errFrame = frameCreate(pattern->width, pattern->height, 0, 0);
    frameAddIndexStreamFromArray(errFrame, pattern->pattern, pattern->width * pattern->height);
    frameAddGraphicsControlInfo(errFrame, 1, 100);

    // Add the rest of the error string to the frame
    for (int i = 1; errorString[i] != '\0'; i++) {
        letterPattern *nextLetter = getLetterOrNumber(errorString[i]);
        appendToFrame(errFrame, nextLetter->pattern, nextLetter->width, nextLetter->height, 0, 0);
    }
    
    // Add the error number
    char *errorNumberString = intToString(errorCode, 3);
    u16 startCodePose = errFrame->imageWidth;
    for (int i = 0; errorNumberString[i] != '\0'; i++) {
        letterPattern *nextLetter = getLetterOrNumber(errorNumberString[i]);
        appendToFrame(errFrame, nextLetter->pattern, nextLetter->width, nextLetter->height, 0, 0);
    }
    free(errorNumberString);
    u16 endCodePose = errFrame->imageWidth;

    // Add an exclamation mark
    letterPattern *exclamation = getLetterOrNumber('!');
    appendToFrame(errFrame, exclamation->pattern, exclamation->width, exclamation->height, 0, 0);

    canvasAddFrame(canvas, errFrame);


    // FRAME #2
    u32 maskFrameWidth = (endCodePose - startCodePose);
    u32 maskFrameSize = maskFrameWidth * errFrame->imageHeight;
    u8 *maskFrameIndexStream = calloc(maskFrameSize, sizeof(u8));
    for (int i = 0; i < maskFrameSize; i++)
        maskFrameIndexStream[i] = 0;

    GIFFrame *maskFrame = frameCreate(maskFrameWidth, errFrame->imageHeight, startCodePose, errFrame->imageTopPosition);
    frameAddIndexStreamFromArray(maskFrame, maskFrameIndexStream, maskFrameSize);
    frameAddGraphicsControlInfo(maskFrame, 1, 100);
    canvasAddFrame(canvas, maskFrame);

    canvasUpdateWidthAndHeight(canvas, errFrame->imageWidth, errFrame->imageHeight);
    u32 widthMuliplier  = 10; u32 heightMuliplier = 10;
    expandCanvas(canvas, widthMuliplier, heightMuliplier);

    encodeGIF(canvas);

    free(maskFrameIndexStream);
    freeCanvas(canvas);

    return OPERATION_SUCCESS;
}
