
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFEncode.h"
#include "GIFTransformations.h"


STATUS_CODE createErrorGif(u32 errorCode) {
    STATUS_CODE status;

    // create color table
    colorTable *globalColorTable = colortableInit();
    status = colortableAppendRGB(globalColorTable, 0, 0, 0);       
    CHECKSTATUS(status);
    status = colortableAppendRGB(globalColorTable, 255, 255, 255); 
    CHECKSTATUS(status);
    status = colortableAppendRGB(globalColorTable, 255, 0, 0); 
    CHECKSTATUS(status);
    status = colortableAppendRGB(globalColorTable, 255, 0, 0); 
    CHECKSTATUS(status);
    status = colortableAppendRGB(globalColorTable, 255, 0, 0); 
    CHECKSTATUS(status);
    status = colortableAppendRGB(globalColorTable, 255, 0, 0); 
    CHECKSTATUS(status);

    // create canvas
    GIFCanvas *canvas = canvasCreate(24, 7);
    status = canvasAddGlobalColorTable(canvas, globalColorTable);   
    CHECKSTATUS(status);
    status = canvasSetBackgroundColorIndex(canvas, 0x00);           
    CHECKSTATUS(status);

    // Frame one
    u8 frameOneTempIndexStream[] = 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,1,1,1,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,0,
      0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,
      0,1,1,1,0,1,1,0,0,1,1,0,0,1,0,0,1,0,1,1,0,0,1,0,
      0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,0,0,
      0,1,1,1,0,1,0,1,0,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    array *frameOneIndexStream = arrayInitFromStackArray((u8*)&frameOneTempIndexStream, sizeof(frameOneTempIndexStream));
    GIFFrame *frameOne = frameCreate(24, 7, 0, 0);
    status = frameAddIndexStream(frameOne, frameOneIndexStream);
    CHECKSTATUS(status);
    status = frameAddGraphicsControlInfo(frameOne, 1, 100);
    CHECKSTATUS(status);
    // status = frameSetTransparanetColorIndexInColorTable(frameOne, 0);
    // CHECKSTATUS(status);

    canvasAddFrame(canvas, frameOne);

    // Frame two
    u8 frameTwoTempIndexStream[] = 
    { 0,0,0,
      0,0,0,
      0,0,0,
      0,0,0,
      0,0,0,
      0,0,0,
      0,0,0 };

    array *frameTwoIndexStream = arrayInitFromStackArray((u8*)&frameTwoTempIndexStream, sizeof(frameTwoTempIndexStream));
    GIFFrame *frameTwo = frameCreate(3, 7, 21, 0);
    status = frameAddIndexStream(frameTwo, frameTwoIndexStream);
    CHECKSTATUS(status);
    status = frameAddGraphicsControlInfo(frameTwo, 1, 100);
    CHECKSTATUS(status);

    canvasAddFrame(canvas, frameTwo);

    // Expand the gif
    u32 widthMuliplier  = 50; u32 heightMuliplier = 50;
    expandCanvas(canvas, widthMuliplier, heightMuliplier);

    encodeGIF(canvas);

    freeCanvas(canvas);

    return OPERATION_SUCCESS;
}

u8 zero[] =
 {0,0,0,0,0,
  0,1,1,1,0,
  0,1,0,1,0,
  0,1,0,1,0,
  0,1,0,1,0,
  0,1,1,1,0,
  0,0,0,0,0};

u8 one[] = 
 {0,0,0,0,0,
  0,1,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,1,1,1,0,
  0,0,0,0,0};

u8 two[] = 
 {0,0,0,0,0,
  0,1,1,1,0,
  0,0,0,1,0,
  0,1,1,1,0,
  0,1,0,0,0,
  0,1,1,1,0,
  0,0,0,0,0};

u8 three[] = 
 {0,0,0,0,0,
  0,1,1,1,0,
  0,0,0,1,0,
  0,1,1,1,0,
  0,0,0,1,0,
  0,1,1,1,0,
  0,0,0,0,0};

u8 four[] = 
 {0,0,0,0,0,
  0,1,0,1,0,
  0,1,0,1,0,
  0,1,1,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,0,0};

u8 five[] = 
 {0,0,0,0,0,
  0,1,1,1,0,
  0,1,0,0,0,
  0,1,1,1,0,
  0,0,0,1,0,
  0,1,1,1,0,
  0,0,0,0,0};

u8 six[] = 
 {0,0,0,0,0,
  0,1,1,1,0,
  0,1,0,0,0,
  0,1,1,1,0,
  0,1,0,1,0,
  0,1,1,1,0,
  0,0,0,0,0};

u8 seven[] = 
 {0,0,0,0,0,
  0,1,1,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,0,0};

u8 eight[] = 
 {0,0,0,0,0,
  0,1,1,1,0,
  0,1,0,1,0,
  0,1,1,1,0,
  0,1,0,1,0,
  0,1,1,1,0,
  0,0,0,0,0};

u8 nine[] = 
 {0,0,0,0,0,
  0,1,1,1,0,
  0,1,0,1,0,
  0,1,1,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,0,0};

u8 space[] = 
 {0,0,
  0,0,
  0,0,
  0,0,
  0,0,
  0,0,
  0,0};

u8 exclamationMark[] = 
 {0,0,0,
  0,1,0,
  0,1,0,
  0,1,0,
  0,0,0,
  0,1,0,
  0,0,0};
