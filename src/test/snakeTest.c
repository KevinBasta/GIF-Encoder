
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFEncode.h"
#include "GIFTransformations.h"

STATUS_CODE createSnakeTest() {
    STATUS_CODE status;

    u16 width  = 10;
    u16 height = 10;

    GIFCanvas *canvas = canvasCreate(width, height);
    CANVAS_NULL_CHECK(canvas);

    status = canvasSetFileName(canvas, "snakeTest.gif");
    CHECKSTATUS(status);

    status = canvasCreateGlobalColorTable(canvas);
    CHECKSTATUS(status);
    
    status = canvasAddColorToColorTable(canvas, 76, 175, 80);
    CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 32, 96, 161);
    CHECKSTATUS(status);
    status = canvasAddColorToColorTable(canvas, 161, 32, 32);
    CHECKSTATUS(status);

    status = canvasSetBackgroundColorIndex(canvas, 0);
    CHECKSTATUS(status);
    
    bool directionTop = false;

    for (int imageLeftPosition = 0; imageLeftPosition < width; imageLeftPosition++) {
        for (int imageTopPosition = 0; imageTopPosition < height; imageTopPosition++) {
            u8 tempIndexStream[100];
            for (int i = 0; i < 100; i++) {
                if (i == width - 1) {
                    tempIndexStream[i] = 2;
                } else {
                    tempIndexStream[i] = 0;
                }
            }

            
            if (directionTop) {
                tempIndexStream[(imageLeftPosition) + ((height - imageTopPosition) * width)] = 1;

                if (height - imageTopPosition + 1 < height) {
                    tempIndexStream[(imageLeftPosition) + ((height - imageTopPosition + 1) * width)] = 1;
                } else if (imageLeftPosition - 1 > 0){
                    tempIndexStream[(imageLeftPosition - 1) + ((height - imageTopPosition) * width)] = 1;
                }
                /* if (imageTopPosition - 2 > 0) {
                    tempIndexStream[(imageLeftPosition) + ((imageTopPosition - 2) * width)] = 1;
                } else if (imageLeftPosition - 2 > 0) {
                    tempIndexStream[(imageLeftPosition - 2) + ((imageTopPosition - 2) * width)] = 1;
                } */
            } else {
                tempIndexStream[(imageLeftPosition) + (imageTopPosition * width)] = 1;

                if (imageTopPosition - 1 > 0) {
                    tempIndexStream[(imageLeftPosition) + ((imageTopPosition - 1) * width)] = 1;
                } else if (imageLeftPosition - 1 > 0){
                    tempIndexStream[(imageLeftPosition - 1) + ((imageTopPosition) * width)] = 1;
                }

                /* if (imageTopPosition - 2 > 0) {
                    tempIndexStream[(imageLeftPosition) + ((imageTopPosition - 2) * width)] = 1;
                } else if (imageLeftPosition - 2 > 0) {
                    tempIndexStream[(imageLeftPosition - 2) + ((imageTopPosition - 2) * width)] = 1;
                } */
            }

            GIFFrame *frameOne = frameCreate(width, height, 0, 0);
            FRAME_NULL_CHECK(frameOne);

            status = frameAddIndexStreamFromArray(frameOne, tempIndexStream, sizeof(tempIndexStream));
            CHECKSTATUS(status);

            status = frameAddGraphicsControlInfo(frameOne, 2, 0);
            CHECKSTATUS(status);

            status = canvasAddFrame(canvas, frameOne);
            CHECKSTATUS(status);
        }

        directionTop = !directionTop;
    }

    u32 widthMuliplier  = 10; u32 heightMuliplier = 10;
    status = expandCanvas(canvas, widthMuliplier, heightMuliplier);

    createGIF(canvas, true, true);

    return OPERATION_SUCCESS;
}
