
#include <stdlib.h>
#include <string.h>
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

#define ERROR_STRING_BUFFER_LENGTH 50

static STATUS_CODE appendToString(char *outString, u32 *outIndex, u32 outLength, char *inString) {
    if (strlen(inString) + *outIndex > outLength)
        return OPERATION_OVERFLOW;
    
    for (u32 i = 0; inString[i] != '\0'; i++) {
        outString[(*outIndex)++] = inString[i];
    }

    return OPERATION_SUCCESS;
}

static STATUS_CODE createErrorString(char *string, u32 stringLength, u32 errorCode) {
    STATUS_CODE status;
    u32 stringIndex = 0;
    
    char prefix[]        = "ERR";
    char *errorCodeString = intToString(errorCode, 10);
    char suffix[]        = "_";

    status = appendToString(string, &stringIndex, stringLength, prefix);
    CHECKSTATUS(status);
    status = appendToString(string, &stringIndex, stringLength, errorCodeString);
    CHECKSTATUS(status);
    status = appendToString(string, &stringIndex, stringLength, suffix);
    CHECKSTATUS(status);

    string[stringIndex] = '\0';

    free(errorCodeString);

    return OPERATION_SUCCESS;
}

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

    status = canvasSetFileName(canvas, "ERROR.gif");
    CHECKSTATUS(status);

    status = canvasCreateGlobalColorTable(canvas);              CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 0, 0, 0);       CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 255, 255, 255); CHECKSTATUS(status);
    status = canvasSetBackgroundColorIndex(canvas, 0);          CHECKSTATUS(status);

    // canvas data to update based on letters appended
    u16 canvasWidth  = 0;
    u16 canvasHeight = 0;

    // Mask position
    u16 startMaskPos = 0;
    u16 endMaskPose  = 0;

    {
        // Create FRAME #1
        char errorString[ERROR_STRING_BUFFER_LENGTH];
        status = createErrorString(errorString, ERROR_STRING_BUFFER_LENGTH, errorCode);
        CHECKSTATUS(status);

        // Get pattern of first letter and create frame
        letterPattern *pattern = getLetterOrNumber(errorString[0]);
        GIFFrame *errFrame     = frameCreate(pattern->width, pattern->height, 0, 0);
        FRAME_NULL_CHECK(errFrame);
        
        // Create index stream for the first letter
        status = frameAddIndexStreamFromArray(errFrame, pattern->pattern, pattern->width * pattern->height);
        CHECKSTATUS(status);

        // Add the rest of the error string to the index stream
        for (int i = 1; errorString[i] != '\0'; i++) {
            if (errorString[i] == '_')
                startMaskPos = errFrame->imageWidth;

            letterPattern *nextLetter = getLetterOrNumber(errorString[i]);
            status = appendToFrame(errFrame, nextLetter->pattern, nextLetter->width, nextLetter->height, 0, 0);
            CHECKSTATUS(status);

            if (errorString[i] == '_')
                endMaskPose = errFrame->imageWidth;
        }

        // Save width and height of this frame
        canvasWidth  = errFrame->imageWidth;
        canvasHeight = errFrame->imageHeight;

        // Put a delay, set disposal method to keep, and add to canvas
        status = frameAddGraphicsControlInfo(errFrame, 1, 100);
        CHECKSTATUS(status);
        status = canvasAddFrame(canvas, errFrame);
        CHECKSTATUS(status);
    }

    {
        // Create FRAME #2
        u32 maskFrameWidth = (endMaskPose - startMaskPos);
        u32 maskFrameSize  = maskFrameWidth * canvasHeight;
        GIFFrame *maskFrame = frameCreate(maskFrameWidth, canvasHeight, startMaskPos, 0);
        FRAME_NULL_CHECK(maskFrame);
        
        // Create index stream and fill it
        status = frameCreateIndexStream(maskFrame, maskFrameSize);
        CHECKSTATUS(status);
        
        for (int i = 0; i < maskFrameSize; i++) {
            status = frameAppendToIndexStream(maskFrame, 0);
            CHECKSTATUS(status);
        }

        status = frameAddGraphicsControlInfo(maskFrame, 1, 100);
        CHECKSTATUS(status);
        status = canvasAddFrame(canvas, maskFrame);
        CHECKSTATUS(status);
    }

    // Update the canvas dimentions and expand the canvas
    status = canvasUpdateWidthAndHeight(canvas, canvasWidth, canvasHeight);
    CHECKSTATUS(status);

    u32 widthMuliplier  = 10; u32 heightMuliplier = 10;
    status = expandCanvas(canvas, widthMuliplier, heightMuliplier);
    CHECKSTATUS(status);

    status = createGIF(canvas, true, false);
    CHECKSTATUS(status);

    return OPERATION_SUCCESS;
}
