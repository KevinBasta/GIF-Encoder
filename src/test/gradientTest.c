
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFTransformations.h"

STATUS_CODE createGradientTest() {
    STATUS_CODE status;

    GIFCanvas *canvas = canvasCreate(254, 1);
    CANVAS_NULL_CHECK(canvas);

    status = canvasCreateGlobalColorTable(canvas);
    CHECKSTATUS(status);
    
    for (int i = 0; i < 255; i++) {
        status = canvasAddColorToColorTable(canvas, 0, i, i);
        CHECKSTATUS(status);
    }

    status = canvasSetBackgroundColorIndex(canvas, 0);
    CHECKSTATUS(status);
    
    u8 tempIndexStream[1];
    for (int i = 0; i < 254; i++) {
        tempIndexStream[0] = i;

        GIFFrame *frameOne = frameCreate(1, 1, i, 0);
        FRAME_NULL_CHECK(frameOne);

        status = frameAddIndexStreamFromArray(frameOne, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        status = frameAddGraphicsControlInfo(frameOne, 1, 0);
        CHECKSTATUS(status);

        status = frameSetTransparanetColorIndexInColorTable(frameOne, 255);
        CHECKSTATUS(status);

        status = canvasAddFrame(canvas, frameOne);
        CHECKSTATUS(status);
    }

    u32 widthMuliplier  = 20; u32 heightMuliplier = 500;
    status = expandCanvas(canvas, widthMuliplier, heightMuliplier);

    createGIF(canvas, true, true);

    return OPERATION_SUCCESS;
}
