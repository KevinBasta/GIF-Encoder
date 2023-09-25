
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

STATUS_CODE gif_createTransparancyTest() {
    STATUS_CODE status;

    // Create canvas
    GIFCanvas *canvas = gif_canvasCreate(TRANSPARENT_SMILY_FACE_TEST_WIDTH, TRANSPARENT_SMILY_FACE_TEST_HEIGHT);
    CANVAS_NULL_CHECK(canvas);

    status = gif_canvasSetFileName(canvas, "testGIF_Transparancy.gif");
    CHECKSTATUS(status);

    status = gif_canvasCreateGlobalColorTable(canvas);
    CHECKSTATUS(status);
    
    // Set to transparent
    status = gif_canvasAddColorToColorTable(canvas, 0, 0, 0);
    CHECKSTATUS(status);
    status = gif_canvasAddColorToColorTable(canvas, 0, 0, 0); // index 1 - black
    CHECKSTATUS(status);
    status = gif_canvasAddColorToColorTable(canvas, 255, 255, 255); // index 2 - white
    CHECKSTATUS(status);

    // Set background to the transparent color
    status = gif_canvasSetBackgroundColorIndex(canvas, 2);
    CHECKSTATUS(status);

    GIFFrame *frameOne;
    GIFFrame *frameTwo;
    GIFFrame *frameThree;
    GIFFrame *frameFour;

    {
        u8 tempIndexStream[] = 
        {0,1,1,0,0,1,1,0,
         0,1,1,0,0,1,1,0,
         0,1,1,0,0,1,1,0,
         0,1,1,0,0,1,1,0,
         0,0,0,0,0,0,0,0,
         1,0,0,0,0,0,0,1,
         0,1,1,1,1,1,1,0,};

        frameOne = gif_frameCreate(TRANSPARENT_SMILY_FACE_TEST_WIDTH, TRANSPARENT_SMILY_FACE_TEST_HEIGHT, 0, 0);
        FRAME_NULL_CHECK(frameOne);

        status = gif_frameAddIndexStreamFromArray(frameOne, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = gif_frameSetTransparanetColorIndexInColorTable(frameOne, 0);
        CHECKSTATUS(status);

        status = gif_frameAddGraphicsControlInfo(frameOne, 2, 200);
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

        frameTwo = gif_frameCreate(TRANSPARENT_SMILY_FACE_TEST_WIDTH, TRANSPARENT_SMILY_FACE_TEST_HEIGHT, 0, 0);
        FRAME_NULL_CHECK(frameTwo);

        status = gif_frameAddIndexStreamFromArray(frameTwo, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = gif_frameSetTransparanetColorIndexInColorTable(frameTwo, 0);
        CHECKSTATUS(status);

        status = gif_frameAddGraphicsControlInfo(frameTwo, 2, 1);
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

        frameThree = gif_frameCreate(TRANSPARENT_SMILY_FACE_TEST_WIDTH, TRANSPARENT_SMILY_FACE_TEST_HEIGHT, 0, 0);
        FRAME_NULL_CHECK(frameThree);

        status = gif_frameAddIndexStreamFromArray(frameThree, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = gif_frameSetTransparanetColorIndexInColorTable(frameThree, 0);
        CHECKSTATUS(status);

        status = gif_frameAddGraphicsControlInfo(frameThree, 2, 1);
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

        frameFour = gif_frameCreate(TRANSPARENT_SMILY_FACE_TEST_WIDTH, TRANSPARENT_SMILY_FACE_TEST_HEIGHT, 0, 0);
        FRAME_NULL_CHECK(frameFour);

        status = gif_frameAddIndexStreamFromArray(frameFour, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = gif_frameSetTransparanetColorIndexInColorTable(frameFour, 0);
        CHECKSTATUS(status);

        status = gif_frameAddGraphicsControlInfo(frameFour, 2, 200);
        CHECKSTATUS(status);
    }

    status = gif_canvasAddFrame(canvas, frameOne);
    CHECKSTATUS(status);
    status = gif_canvasAddFrame(canvas, frameTwo);
    CHECKSTATUS(status);
    status = gif_canvasAddFrame(canvas, frameThree);
    CHECKSTATUS(status);
    status = gif_canvasAddFrame(canvas, frameFour);
    CHECKSTATUS(status);
    status = gif_canvasAddFrame(canvas, frameThree);
    CHECKSTATUS(status);
    status = gif_canvasAddFrame(canvas, frameTwo);
    CHECKSTATUS(status);

    u32 widthMuliplier  = 10; u32 heightMuliplier = 10;
    status = gif_expandCanvas(canvas, widthMuliplier, heightMuliplier);

    gif_createGIF(canvas, true, true);

    return OPERATION_SUCCESS;
}