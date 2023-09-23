
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

STATUS_CODE createSmallGif() {
    STATUS_CODE status;

    // Create canvas
    GIFCanvas *canvas = canvasCreate(SMALL_TEST_WIDTH, SMALL_TEST_HEIGHT);
    CANVAS_NULL_CHECK(canvas);

    status = canvasSetFileName(canvas, "testGIF_Small.gif");
    CHECKSTATUS(status);

    status = canvasCreateGlobalColorTable(canvas);
    CHECKSTATUS(status);
    
    status = canvasAddColorToColorTable(canvas, 0, 0, 0); // index 0 - black
    CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 88, 89, 89); // index 1 - grey
    CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 143, 82, 70); // index 2 - red
    CHECKSTATUS(status);

    {
        u8 tempIndexStream[] = 
         {1,1,};

        // Base Red Light Frame
        GIFFrame *frame = frameCreate(SMALL_TEST_WIDTH, SMALL_TEST_HEIGHT, 0, 0);
        FRAME_NULL_CHECK(frame);

        status = frameAddIndexStreamFromArray(frame, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = canvasAddFrame(canvas, frame);
        CHECKSTATUS(status);
    }

    // u32 widthMuliplier  = 10; u32 heightMuliplier = 10;
    // status = expandCanvas(canvas, widthMuliplier, heightMuliplier);

    status = createGIF(canvas, true, true);
    CHECKSTATUS(status);

    return OPERATION_SUCCESS;
}