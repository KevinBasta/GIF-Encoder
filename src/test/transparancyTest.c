
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFEncode.h"
#include "GIFTransformations.h"

#define TRANSPARENT_SMILY_FACE_TEST_WIDTH       8
#define TRANSPARENT_SMILY_FACE_TEST_HEIGHT      7

STATUS_CODE createTransparancyTest() {
    STATUS_CODE status;

    // Create canvas
    GIFCanvas *canvas = canvasCreate(TRANSPARENT_SMILY_FACE_TEST_WIDTH, TRANSPARENT_SMILY_FACE_TEST_HEIGHT);
    CANVAS_NULL_CHECK(canvas);

    status = canvasSetFileName(canvas, "testGIF_Transparancy.gif");
    CHECKSTATUS(status);

    status = canvasCreateGlobalColorTable(canvas);
    CHECKSTATUS(status);
    
    // Set to transparent
    status = canvasAddColorToColorTable(canvas, 0, 0, 0);
    CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 0, 0, 0); // index 1 - black
    CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 255, 255, 255); // index 2 - white
    CHECKSTATUS(status);

    // Set background to the transparent color
    status = canvasSetBackgroundColorIndex(canvas, 2);
    CHECKSTATUS(status);

    GIFFrame *frameOne;
    GIFFrame *frameTwo;
    GIFFrame *frameThree;
    GIFFrame *frameFour;
    GIFFrame *frameFive;

    {
        u8 tempIndexStream[] = 
        {0,1,1,0,0,1,1,0,
         0,1,1,0,0,1,1,0,
         0,1,1,0,0,1,1,0,
         0,1,1,0,0,1,1,0,
         0,0,0,0,0,0,0,0,
         1,0,0,0,0,0,0,1,
         0,1,1,1,1,1,1,0,};

        frameOne = frameCreate(TRANSPARENT_SMILY_FACE_TEST_WIDTH, TRANSPARENT_SMILY_FACE_TEST_HEIGHT, 0, 0);
        FRAME_NULL_CHECK(frameOne);

        status = frameAddIndexStreamFromArray(frameOne, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = frameSetTransparanetColorIndexInColorTable(frameOne, 0);
        CHECKSTATUS(status);

        status = frameAddGraphicsControlInfo(frameOne, 2, 200);
        CHECKSTATUS(status);
    }

    {
        u8 tempIndexStream[] = 
        {0,1,1,0,0,0,0,0,
         0,1,1,0,1,1,1,1,
         0,1,1,0,0,1,1,0,
         0,1,1,0,0,1,1,0,
         0,0,0,0,0,0,0,0,
         1,0,0,0,0,0,0,1,
         0,1,1,1,1,1,1,0,};

        frameTwo = frameCreate(TRANSPARENT_SMILY_FACE_TEST_WIDTH, TRANSPARENT_SMILY_FACE_TEST_HEIGHT, 0, 0);
        FRAME_NULL_CHECK(frameTwo);

        status = frameAddIndexStreamFromArray(frameTwo, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = frameSetTransparanetColorIndexInColorTable(frameTwo, 0);
        CHECKSTATUS(status);

        status = frameAddGraphicsControlInfo(frameTwo, 2, 1);
        CHECKSTATUS(status);
    }

    {
        u8 tempIndexStream[] = 
        {0,1,1,0,0,0,0,0,
         0,1,1,0,1,0,0,1,
         0,1,1,0,0,1,1,0,
         0,1,1,0,0,1,1,0,
         0,0,0,0,0,0,0,0,
         1,0,0,0,0,0,0,1,
         0,1,1,1,1,1,1,0,};

        frameThree = frameCreate(TRANSPARENT_SMILY_FACE_TEST_WIDTH, TRANSPARENT_SMILY_FACE_TEST_HEIGHT, 0, 0);
        FRAME_NULL_CHECK(frameThree);

        status = frameAddIndexStreamFromArray(frameThree, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = frameSetTransparanetColorIndexInColorTable(frameThree, 0);
        CHECKSTATUS(status);

        status = frameAddGraphicsControlInfo(frameThree, 2, 1);
        CHECKSTATUS(status);
    }
    
    {
        u8 tempIndexStream[] = 
        {0,1,1,0,0,0,0,0,
         0,1,1,0,0,0,0,0,
         0,1,1,0,1,0,0,1,
         0,1,1,0,0,1,1,0,
         0,0,0,0,0,0,0,0,
         1,0,0,0,0,0,0,1,
         0,1,1,1,1,1,1,0,};

        frameFour = frameCreate(TRANSPARENT_SMILY_FACE_TEST_WIDTH, TRANSPARENT_SMILY_FACE_TEST_HEIGHT, 0, 0);
        FRAME_NULL_CHECK(frameFour);

        status = frameAddIndexStreamFromArray(frameFour, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = frameSetTransparanetColorIndexInColorTable(frameFour, 0);
        CHECKSTATUS(status);

        status = frameAddGraphicsControlInfo(frameFour, 2, 200);
        CHECKSTATUS(status);
    }

    status = canvasAddFrame(canvas, frameOne);
    CHECKSTATUS(status);
    status = canvasAddFrame(canvas, frameTwo);
    CHECKSTATUS(status);
    status = canvasAddFrame(canvas, frameThree);
    CHECKSTATUS(status);
    status = canvasAddFrame(canvas, frameFour);
    CHECKSTATUS(status);
    status = canvasAddFrame(canvas, frameThree);
    CHECKSTATUS(status);
    status = canvasAddFrame(canvas, frameTwo);
    CHECKSTATUS(status);

    u32 widthMuliplier  = 10; u32 heightMuliplier = 10;
    status = expandCanvas(canvas, widthMuliplier, heightMuliplier);

    createGIF(canvas, true, true);

    return OPERATION_SUCCESS;
}