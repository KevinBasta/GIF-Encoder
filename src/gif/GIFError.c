
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFExtendedInterface.h"
#include "GIFEncode.h"
#include "GIFTransformations.h"
#include "printUtility.h"

#include "lettersAndNumbers.h"

/**
 * @brief creates a gif that displays the word "ERROR ##!"
 * @param errorCode The number to put for ## above
 *
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE createErrorGif(u32 errorCode) {
    STATUS_CODE status;

    // create canvas
    GIFCanvas *canvas = canvasCreate(MAX_LETTER_PIXEL_WIDTH, ROW_HEIGHT_IN_PIXELS);
    CANVAS_NULL_CHECK(canvas);

    status = canvasSetFileName(canvas, "error.gif");
    CHECKSTATUS(status);

    status = canvasCreateGlobalColorTable(canvas);              CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 0, 0, 0);       CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 255, 255, 255); CHECKSTATUS(status);
    status = canvasSetBackgroundColorIndex(canvas, 0);          CHECKSTATUS(status);



    // Create FRAME #1
    char *errorString = "ERR";
    letterPattern *pattern = getLetterOrNumber(errorString[0]);

    // Create frame for the first letter
    GIFFrame *errFrame = frameCreate(pattern->width, pattern->height, 0, 0);
    FRAME_NULL_CHECK(errFrame);

    status = frameAddIndexStreamFromArray(errFrame, pattern->pattern, pattern->width * pattern->height);
    CHECKSTATUS(status);
    status = frameAddGraphicsControlInfo(errFrame, 1, 100);
    CHECKSTATUS(status);
    status = canvasAddFrame(canvas, errFrame);
    CHECKSTATUS(status);

    // Add the rest of the error string to the frame
    for (int i = 1; errorString[i] != '\0'; i++) {
        letterPattern *nextLetter = getLetterOrNumber(errorString[i]);
        status = appendToFrame(errFrame, nextLetter->pattern, nextLetter->width, nextLetter->height, 0, 0);
        CHECKSTATUS(status);
    }
    
    // Add the error number to the frame
    char *errorNumberString = intToString(errorCode, 3);
    for (int i = 0; errorNumberString[i] != '\0'; i++) {
        letterPattern *nextLetter = getLetterOrNumber(errorNumberString[i]);
        status = appendToFrame(errFrame, nextLetter->pattern, nextLetter->width, nextLetter->height, 0, 0);
        CHECKSTATUS(status);
    }
    free(errorNumberString);

    // Add an underscore mark to the frame
    u16 startCodePose = errFrame->imageWidth;
    letterPattern *endLetter = getLetterOrNumber('_');
    status = appendToFrame(errFrame, endLetter->pattern, endLetter->width, endLetter->height, 0, 0);
    CHECKSTATUS(status);
    u16 endCodePose = errFrame->imageWidth;




    // Create FRAME #2
    u32 maskFrameWidth = (endCodePose - startCodePose);
    u32 maskFrameSize = maskFrameWidth * errFrame->imageHeight;
    u8 *maskFrameIndexStream = calloc(maskFrameSize, sizeof(u8));
    for (int i = 0; i < maskFrameSize; i++)
        maskFrameIndexStream[i] = 0;

    // Create frame to mask the udnerscore
    GIFFrame *maskFrame = frameCreate(maskFrameWidth, errFrame->imageHeight, startCodePose, errFrame->imageTopPosition);
    FRAME_NULL_CHECK(maskFrame);

    status = frameAddIndexStreamFromArray(maskFrame, maskFrameIndexStream, maskFrameSize);
    CHECKSTATUS(status);
    status = frameAddGraphicsControlInfo(maskFrame, 1, 100);
    CHECKSTATUS(status);
    status = canvasAddFrame(canvas, maskFrame);
    CHECKSTATUS(status);



    // Update the canvas dimentions and expand the canvas
    status = canvasUpdateWidthAndHeight(canvas, errFrame->imageWidth, errFrame->imageHeight);
    CHECKSTATUS(status);

    u32 widthMuliplier  = 10; u32 heightMuliplier = 10;
    status = expandCanvas(canvas, widthMuliplier, heightMuliplier);
    CHECKSTATUS(status);

    status = createGIF(canvas, true, true);
    CHECKSTATUS(status);

    free(maskFrameIndexStream);

    return OPERATION_SUCCESS;
}
