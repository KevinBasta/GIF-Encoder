
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFEncode.h"
#include "GIFTransformations.h"

STATUS_CODE createLocalColorTableTest() {
    STATUS_CODE status;

    // Create canvas
    GIFCanvas *canvas = canvasCreate(33, 17);
    CANVAS_NULL_CHECK(canvas);

    status = canvasSetFileName(canvas, "testGIF_LocalColorTable.gif");
    CHECKSTATUS(status);

    u8 tempIndexStream[] =
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
      0,2,0,0,1,1,0,0,1,1,0,0,1,1,0,0,2,1,1,0,0,1,1,0,0,1,1,0,0,1,1,2,0,
      0,2,0,0,1,1,0,0,1,1,0,0,1,1,0,0,2,1,1,0,0,1,1,0,0,1,1,0,0,1,1,2,0,
      0,2,2,2,2,1,0,0,1,1,0,0,1,1,0,0,2,1,1,0,0,1,1,0,0,1,1,0,2,2,2,2,0,
      0,2,0,0,2,1,0,0,1,1,0,0,1,1,0,0,2,1,1,0,0,1,1,0,0,1,1,0,2,1,1,2,0,
      0,2,0,0,2,1,0,0,1,1,0,0,1,1,0,2,2,2,1,0,0,1,1,0,0,1,1,0,2,1,1,2,0,
      0,2,0,0,2,1,0,0,1,1,0,0,1,1,2,0,2,1,2,0,0,1,1,0,0,1,1,0,2,1,1,2,0,
      0,2,0,0,2,1,0,0,1,1,0,0,1,1,2,0,2,1,2,0,0,1,1,0,0,1,1,0,2,1,1,2,0,
      0,2,0,0,2,1,0,0,1,1,0,0,1,1,2,0,2,1,2,0,0,1,1,0,0,1,1,0,2,1,1,2,0,
      0,2,0,0,2,1,0,0,1,1,0,0,1,1,0,2,2,2,1,0,0,1,1,0,0,1,1,0,2,1,1,2,0,
      0,2,0,0,2,1,0,0,1,1,0,0,1,1,0,0,2,1,1,0,0,1,1,0,0,1,1,0,2,1,1,2,0,
      0,2,2,2,2,1,0,0,1,1,0,0,1,1,0,0,2,1,1,0,0,1,1,0,0,1,1,0,2,2,2,2,0,
      0,2,0,0,1,1,0,0,1,1,0,0,1,1,0,0,2,1,1,0,0,1,1,0,0,1,1,0,0,1,1,2,0,
      0,2,0,0,1,1,0,0,1,1,0,0,1,1,0,0,2,1,1,0,0,1,1,0,0,1,1,0,0,1,1,2,0,
      0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
    
    {
        // First frame
        GIFFrame *frameOne = frameCreate(33, 17, 0, 0);
        status = frameAddIndexStreamFromArray(frameOne, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);

        // First frame local color table
        status = frameCreateLocalColorTable(frameOne); CHECKSTATUS(status);
        status = frameAddColorToColorTable(frameOne, 45, 133, 79); CHECKSTATUS(status);
        status = frameAddColorToColorTable(frameOne, 47, 158, 90); CHECKSTATUS(status);
        status = frameAddColorToColorTable(frameOne, 255, 255, 255); CHECKSTATUS(status);

        status = frameAddGraphicsControlInfo(frameOne, 0, 100);
        CHECKSTATUS(status);

        status = canvasAddFrame(canvas, frameOne);
        CHECKSTATUS(status);
    }
    
    {
        // Second frame
        GIFFrame *frameTwo = frameCreate(33, 17, 0, 0);
        status = frameAddIndexStreamFromArray(frameTwo, tempIndexStream, sizeof(tempIndexStream));
        CHECKSTATUS(status);
        
        // Second frame local color table
        status = frameCreateLocalColorTable(frameTwo); CHECKSTATUS(status);
        status = frameAddColorToColorTable(frameTwo, 47, 98, 153);   CHECKSTATUS(status);
        status = frameAddColorToColorTable(frameTwo, 46, 89, 135);   CHECKSTATUS(status);
        status = frameAddColorToColorTable(frameTwo, 0, 0, 0);     CHECKSTATUS(status);
        
        status = frameAddGraphicsControlInfo(frameTwo, 0, 100);
        CHECKSTATUS(status);

        status = canvasAddFrame(canvas, frameTwo);
        CHECKSTATUS(status);
    }
    
    u32 widthMuliplier  = 10; u32 heightMuliplier = 10;
    expandCanvas(canvas, widthMuliplier, heightMuliplier);

    createGIF(canvas, true, true);

    return OPERATION_SUCCESS;
}
