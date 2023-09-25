
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFTransformations.h"

STATUS_CODE gif_createGradientTest() {
    STATUS_CODE status;

    GIFCanvas *canvas = gif_canvasCreate(254, 1);
    CANVAS_NULL_CHECK(canvas);

    status = gif_canvasSetFileName(canvas, "testGIF_Gradient.gif");
    CHECKSTATUS(status);

    status = gif_canvasCreateGlobalColorTable(canvas);
    CHECKSTATUS(status);
    
    for (int i = 0; i < 255; i++) {
        status = gif_canvasAddColorToColorTable(canvas, 0, i, i);
        CHECKSTATUS(status);
    }

    status = gif_canvasSetBackgroundColorIndex(canvas, 0);
    CHECKSTATUS(status);
    
    u8 tempIndexStream[1];
    for (int i = 0; i < 254; i++) {
        tempIndexStream[0] = i;

        GIFFrame *frameOne = gif_frameCreate(1, 1, i, 0);
        FRAME_NULL_CHECK(frameOne);

        status = gif_frameAddIndexStreamFromArray(frameOne, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = gif_frameAddGraphicsControlInfo(frameOne, 1, 0);
        CHECKSTATUS(status);

        status = gif_frameSetTransparanetColorIndexInColorTable(frameOne, 255);
        CHECKSTATUS(status);

        status = gif_canvasAddFrame(canvas, frameOne);
        CHECKSTATUS(status);
    }

    u32 widthMuliplier  = 20; u32 heightMuliplier = 500;
    status = gif_expandCanvas(canvas, widthMuliplier, heightMuliplier);

    gif_createGIF(canvas, true, true);

    return OPERATION_SUCCESS;
}
