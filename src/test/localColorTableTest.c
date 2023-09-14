
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFEncode.h"
#include "GIFTransformations.h"

STATUS_CODE createTestGif() {
    STATUS_CODE status;

    // create color table
    colorTable *globalColorTable = colortableInit();
    status = colortableAppendRGB(globalColorTable, 0xFF, 0xFF, 0xFF);       
    CHECKSTATUS(status);
    status = colortableAppendRGB(globalColorTable, 0xFF, 0x00, 0x00); 
    CHECKSTATUS(status);
    status = colortableAppendRGB(globalColorTable, 0x00, 0x00, 0xFF); 
    CHECKSTATUS(status);

    // create canvas
    GIFCanvas *canvas = canvasCreate(0x0A, 0x0A);
    status = canvasAddGlobalColorTable(canvas, globalColorTable);   
    CHECKSTATUS(status);
    status = canvasSetBackgroundColorIndex(canvas, 0x00);           
    CHECKSTATUS(status);

    u8 frameOneTempIndexStream[] =
    { 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
      1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
      1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
      1, 1, 1, 0, 0, 0, 0, 2, 2, 2,
      1, 1, 1, 0, 0, 0, 0, 2, 2, 2,
      2, 2, 2, 0, 0, 0, 0, 1, 1, 1,
      2, 2, 2, 0, 0, 0, 0, 1, 1, 1,
      2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
      2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
      2, 2, 2, 2, 2, 1, 1, 1, 1, 1 };
    
    array *frameOneIndexStream = arrayInitFromStackArray((u8*)&frameOneTempIndexStream, sizeof(frameOneTempIndexStream));
    GIFFrame *frameOne = frameCreate(24, 7, 0, 0);
    status = frameAddIndexStream(frameOne, frameOneIndexStream);
    CHECKSTATUS(status);

    canvasAddFrame(canvas, frameOne);
    
    u32 widthMuliplier  = 192; u32 heightMuliplier = 108;
    expandCanvas(canvas, widthMuliplier, heightMuliplier);

    encodeGIF(canvas);

    freeCanvas(canvas);

    return OPERATION_SUCCESS;
}
