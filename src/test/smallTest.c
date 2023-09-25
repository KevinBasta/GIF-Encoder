
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFEncode.h"
#include "GIFTransformations.h"

#define SMALL_TEST_WIDTH    2
#define SMALL_TEST_HEIGHT   1

STATUS_CODE gif_createSmallGif() {
    STATUS_CODE status;

    // Create canvas
    GIFCanvas *canvas = gif_canvasCreate(SMALL_TEST_WIDTH, SMALL_TEST_HEIGHT);
    CANVAS_NULL_CHECK(canvas);

    status = gif_canvasSetFileName(canvas, "testGIF_Small.gif");
    CHECKSTATUS(status);

    status = gif_canvasCreateGlobalColorTable(canvas);
    CHECKSTATUS(status);
    
    status = gif_canvasAddColorToColorTable(canvas, 0, 0, 0); // index 0 - black
    CHECKSTATUS(status);
    status = gif_canvasAddColorToColorTable(canvas, 88, 89, 89); // index 1 - grey
    CHECKSTATUS(status);
    status = gif_canvasAddColorToColorTable(canvas, 143, 82, 70); // index 2 - red
    CHECKSTATUS(status);

    {
        u8 tempIndexStream[] = 
         {1,1,};

        // Base Red Light Frame
        GIFFrame *frame = gif_frameCreate(SMALL_TEST_WIDTH, SMALL_TEST_HEIGHT, 0, 0);
        FRAME_NULL_CHECK(frame);

        status = gif_frameAddIndexStreamFromArray(frame, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = gif_canvasAddFrame(canvas, frame);
        CHECKSTATUS(status);
    }

    u32 widthMuliplier  = 2; u32 heightMuliplier = 2;
    status = gif_expandCanvas(canvas, widthMuliplier, heightMuliplier);

    status = gif_createGIF(canvas, true, true);
    CHECKSTATUS(status);

    return OPERATION_SUCCESS;
}