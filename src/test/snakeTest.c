
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFEncode.h"
#include "GIFTransformations.h"


static STATUS_CODE createOneBlockFrame(GIFCanvas *canvas, u16 leftPos, u16 topPos, u16 colorTableIndex, u32 delayTime) {
    STATUS_CODE status;

    GIFFrame *frame = gif_frameCreate(1, 1, leftPos, topPos);
    FRAME_NULL_CHECK(frame);

    status = gif_frameCreateIndexStream(frame, 1);
    CHECKSTATUS(status);
    status = gif_frameAppendToIndexStream(frame, colorTableIndex);
    CHECKSTATUS(status);

    status = gif_frameAddGraphicsControlInfo(frame, 3, delayTime);
    CHECKSTATUS(status);

    status = gif_canvasAddFrame(canvas, frame);
    CHECKSTATUS(status);

    return OPERATION_SUCCESS;
}
/**
 * @note This test may not work with some decoders since
 * the graphics control extention disposal method #3
 * has different interpretations. On my windows machine
 * the snake appears fine because the decoder used by windows
 * interprets disposal method #3 as "the previous full state"
 * where all the states before it (including the first background
 * image) are part of the state. With other decoders when disposal
 * method #3 is used, it's interpreted as "only the state of the
 * previous frame in isolation", leading to an empty canvas.
 */
STATUS_CODE gif_createSnakeTest() {
    STATUS_CODE status;

    u16 width  = 10;
    u16 height = 10;

    GIFCanvas *canvas = gif_canvasCreate(width, height);
    CANVAS_NULL_CHECK(canvas);

    status = gif_canvasSetFileName(canvas, "testGIF_Snake.gif");
    CHECKSTATUS(status);

    status = gif_canvasCreateGlobalColorTable(canvas);
    CHECKSTATUS(status);
    
    status = gif_canvasAddColorToColorTable(canvas, 76, 175, 80); // 0
    CHECKSTATUS(status);
    status = gif_canvasAddColorToColorTable(canvas, 32, 96, 161); // 1
    CHECKSTATUS(status);
    status = gif_canvasAddColorToColorTable(canvas, 161, 32, 32); // 2
    CHECKSTATUS(status);
    status = gif_canvasAddColorToColorTable(canvas, 0, 0, 0); // 3
    CHECKSTATUS(status);

    {
        // Background frame
        GIFFrame *backgroundFrame = gif_frameCreate(width, height, 0, 0);
        FRAME_NULL_CHECK(backgroundFrame);

        status = gif_frameCreateIndexStream(backgroundFrame, width * height);
        CHECKSTATUS(status);

        // Fill the background with green and put apple
        for (int i = 0; i < width * height; i++) {
            if (i == width - 1) {
                status = gif_frameAppendToIndexStream(backgroundFrame, 2);
            } else {
                status = gif_frameAppendToIndexStream(backgroundFrame, 0);
            }
            CHECKSTATUS(status);
        }

        status = gif_frameAddGraphicsControlInfo(backgroundFrame, 1, 100);
        CHECKSTATUS(status);

        status = gif_canvasAddFrame(canvas, backgroundFrame);
        CHECKSTATUS(status);
    }

    // Snake one block length body frame
    status = createOneBlockFrame(canvas, 0, 0, 1, 0);
    CHECKSTATUS(status);
    
    {
        // Snake two block length body frames
        bool directionTop = false;
        for (int imageLeftPosition = 0; imageLeftPosition < width; imageLeftPosition++) {
            
            int imageTopPosition = 0;
            if (directionTop == true) {
                imageTopPosition = 1;
            }
            
            for (; imageTopPosition < height; imageTopPosition++) {
                
                u32 snakeWidth      = 0;
                u32 snakeHeight     = 0;
                u32 snakeLeftPos    = 0;
                u32 snakeTopPos     = 0;

                if (imageTopPosition == height - 1 && imageLeftPosition != width - 1) {
                    snakeWidth  = 2;
                    snakeHeight = 1;
                } else {
                    snakeWidth  = 1;
                    snakeHeight = 2;
                }
                
                if (directionTop) {
                    snakeLeftPos = imageLeftPosition;
                    snakeTopPos  = height - 1 - imageTopPosition;
                } else {                    
                    snakeLeftPos = imageLeftPosition;
                    snakeTopPos  = imageTopPosition;
                }

                GIFFrame *frame = gif_frameCreate(snakeWidth, snakeHeight, snakeLeftPos, snakeTopPos);
                FRAME_NULL_CHECK(frame);

                status = gif_frameCreateIndexStream(frame, 2);
                CHECKSTATUS(status);
                status = gif_frameAppendToIndexStream(frame, 1);
                CHECKSTATUS(status);
                status = gif_frameAppendToIndexStream(frame, 1);
                CHECKSTATUS(status);

                status = gif_frameAddGraphicsControlInfo(frame, 3, 0);
                CHECKSTATUS(status);

                status = gif_canvasAddFrame(canvas, frame);
                CHECKSTATUS(status);
            }

            directionTop = !directionTop;
        }
    }

    // Snake one block length body frame
    status = createOneBlockFrame(canvas, width - 1, 0, 1, 0);
    CHECKSTATUS(status);

    // Apple mask frame
    status = createOneBlockFrame(canvas, width - 1, 0, 0, 400);
    CHECKSTATUS(status);

    u32 widthMuliplier  = 20; u32 heightMuliplier = 20;
    status = gif_expandCanvas(canvas, widthMuliplier, heightMuliplier);
    CHECKSTATUS(status);

    gif_createGIF(canvas, true, true);

    return OPERATION_SUCCESS;
}
