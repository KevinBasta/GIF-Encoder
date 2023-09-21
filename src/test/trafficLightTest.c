
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFEncode.h"
#include "GIFTransformations.h"

#define TRAFFIC_LIGHT_WIDTH_IN_PIXELS       6
#define TRAFFIC_LIGHT_HEIGHT_IN_PIXELS      16

STATUS_CODE createTrafficLightGif() {
    STATUS_CODE status;

    // Create canvas
    GIFCanvas *canvas = canvasCreate(TRAFFIC_LIGHT_WIDTH_IN_PIXELS, TRAFFIC_LIGHT_HEIGHT_IN_PIXELS);
    CANVAS_NULL_CHECK(canvas);

    status = canvasSetFileName(canvas, "trafficLightTest.gif");
    CHECKSTATUS(status);

    status = canvasCreateGlobalColorTable(canvas);
    CHECKSTATUS(status);
    
    status = canvasAddColorToColorTable(canvas, 0, 0, 0); // index 0 - black
    CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 88, 89, 89); // index 1 - grey
    CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 230, 10, 0); // index 2 - red
    CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 36, 196, 4); // index 3 - green
    CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 219, 212, 4); // index 4 - yellow 
    CHECKSTATUS(status);

    {
        u8 tempIndexStream[] = 
        {0,0,0,0,0,0,
         0,0,2,2,0,0,
         0,2,2,2,2,0,
         0,2,2,2,2,0,
         0,0,2,2,0,0,
         0,0,0,0,0,0,
         0,0,1,1,0,0,
         0,1,1,1,1,0,
         0,1,1,1,1,0,
         0,0,1,1,0,0,
         0,0,0,0,0,0,
         0,0,1,1,0,0,
         0,1,1,1,1,0,
         0,1,1,1,1,0,
         0,0,1,1,0,0,
         0,0,0,0,0,0,};

        // Base Red Light Frame
        GIFFrame *frame = frameCreate(TRAFFIC_LIGHT_WIDTH_IN_PIXELS, TRAFFIC_LIGHT_HEIGHT_IN_PIXELS, 0, 0);
        FRAME_NULL_CHECK(frame);

        status = frameAddIndexStreamFromArray(frame, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = frameAddGraphicsControlInfo(frame, 1, 200);
        CHECKSTATUS(status);

        status = canvasAddFrame(canvas, frame);
        CHECKSTATUS(status);
    }

    {
        u8 tempIndexStream[] = 
        {0,1,1,0,
         1,1,1,1,
         1,1,1,1,
         0,1,1,0,
         0,0,0,0,
         0,1,1,0,
         1,1,1,1,
         1,1,1,1,
         0,1,1,0,
         0,0,0,0,
         0,3,3,0,
         3,3,3,3,
         3,3,3,3,
         0,3,3,0,};

        // Green Light
        GIFFrame *frame = frameCreate(TRAFFIC_LIGHT_WIDTH_IN_PIXELS - 2, TRAFFIC_LIGHT_HEIGHT_IN_PIXELS - 2, 1, 1);
        FRAME_NULL_CHECK(frame);

        status = frameAddIndexStreamFromArray(frame, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = frameSetTransparanetColorIndexInColorTable(frame, 5);
        CHECKSTATUS(status);

        status = frameAddGraphicsControlInfo(frame, 1, 400);
        CHECKSTATUS(status);

        status = canvasAddFrame(canvas, frame);
        CHECKSTATUS(status);
    }

    {
        u8 tempIndexStream[] = 
        {0,1,1,0,
         1,1,1,1,
         1,1,1,1,
         0,1,1,0,
         0,0,0,0,
         0,4,4,0,
         4,4,4,4,
         4,4,4,4,
         0,4,4,0,
         0,0,0,0,
         0,1,1,0,
         1,1,1,1,
         1,1,1,1,
         0,1,1,0,};

        // Yellow Light
        GIFFrame *frame = frameCreate(TRAFFIC_LIGHT_WIDTH_IN_PIXELS - 2, TRAFFIC_LIGHT_HEIGHT_IN_PIXELS - 2, 1, 1);
        FRAME_NULL_CHECK(frame);

        status = frameAddIndexStreamFromArray(frame, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = frameSetTransparanetColorIndexInColorTable(frame, 5);
        CHECKSTATUS(status);

        status = frameAddGraphicsControlInfo(frame, 1, 100);
        CHECKSTATUS(status);

        status = canvasAddFrame(canvas, frame);
        CHECKSTATUS(status);
    }

    u32 widthMuliplier  = 10; u32 heightMuliplier = 10;
    status = expandCanvas(canvas, widthMuliplier, heightMuliplier);

    createGIF(canvas, true, true);

    return OPERATION_SUCCESS;
}