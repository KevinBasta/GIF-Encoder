
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFTransformations.h"

STATUS_CODE gif_createMaxColorIndexTest() {
    STATUS_CODE status;

    // Create canvas
    GIFCanvas *canvas = gif_canvasCreate(255, 1);
    CANVAS_NULL_CHECK(canvas);

    status = gif_canvasSetFileName(canvas, "testGIF_MaxColorIndex.gif");
    CHECKSTATUS(status);

    status = gif_canvasCreateGlobalColorTable(canvas);
    CHECKSTATUS(status);
    
    for (int i = 0; i < 255; i++) {
        status = gif_canvasAddColorToColorTable(canvas, 0, i, i);
        CHECKSTATUS(status);
    }

    u8 tempIndexStream[255];
    for (int i = 0; i < 255; i++) {
        tempIndexStream[i] = i;
    }
    
    {
        GIFFrame *frameOne = gif_frameCreate(255, 1, 0, 0);
        FRAME_NULL_CHECK(frameOne);

        status = gif_frameAddIndexStreamFromArray(frameOne, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = gif_canvasAddFrame(canvas, frameOne);
        CHECKSTATUS(status);
    }

    u32 widthMuliplier  = 12; u32 heightMuliplier = 1000;
    status = gif_expandCanvas(canvas, widthMuliplier, heightMuliplier);

    gif_createGIF(canvas, true, true);

    return OPERATION_SUCCESS;
}
