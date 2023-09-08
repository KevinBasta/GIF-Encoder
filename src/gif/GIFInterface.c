
#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "linkedlist.h"
#include "GIFInterface.h"
#include "GIFColorTable.h"
#include "GIFCodeTable.h"

/////////// GIF Canvas Interface ///////////


GIFCanvas *canvasCreate(u16 canvasWidth, u16 canvasHeight) {
    GIFCanvas *newCanvas = calloc(1, sizeof(GIFCanvas));

    newCanvas->canvasWidth = canvasWidth;
    newCanvas->canvasHeight = canvasHeight;

    newCanvas->packedField_GlobalColorTableFlag     = 0;
    newCanvas->packedField_ColorResolution          = 0;
    newCanvas->packedField_SortFlag                 = 0;
    newCanvas->packedField_SizeOfGlobalColorTable   = 0;

    newCanvas->backgroundColorIndex = 0;
    newCanvas->pixelAspectRatio     = 0;

    newCanvas->globalColorTable = NULL;

    newCanvas->frames           = linkedlistInit();

    return newCanvas;
}

STATUS_CODE canvasAddGlobalColorTable(GIFCanvas *canvas, colorTable *clrTable) {
    CANVAS_NULL_CHECK(canvas);
    COLOR_TABLE_NULL_CHECK(clrTable);

    canvas->packedField_GlobalColorTableFlag = 1;
    
    // Color table must be fully populated before this function call
    canvas->packedField_ColorResolution         = clrTable->size;
    canvas->packedField_SizeOfGlobalColorTable  = clrTable->size;

    canvas->globalColorTable = clrTable; 

    return OPERATION_SUCCESS;
}

STATUS_CODE canvasSetBackgroundColorIndex(GIFCanvas *canvas, u8 globalColorTableIndex) {
    CANVAS_NULL_CHECK(canvas);

    // !!TODO - ERROR CHECK IF GREATER THAN BIGGEST CLR TABLE ITEM
    // ALSO IF GLOBAL CLR TABLE IS NULL AND THIS IS BEING SET TO NON ZERO
    canvas->backgroundColorIndex = globalColorTableIndex;
    
    return OPERATION_SUCCESS;
}

STATUS_CODE canvasAddFrame(GIFCanvas *canvas, GIFFrame *frame) {
    CANVAS_NULL_CHECK(canvas);
    FRAME_NULL_CHECK(frame);

    if (canvas->frames == NULL) {
        canvas->frames = linkedlistInit();
    }

    linkedlistAppend(canvas->frames, frame);

    return OPERATION_SUCCESS;
}

void freeCanvas(GIFCanvas *canvas) {
    if (canvas != NULL) {
        if (canvas->globalColorTable != NULL)
            freeColorTable(canvas->globalColorTable);
        
        freeFrameLinkedList(canvas->frames);

        free(canvas);
    }
}





/////////// GIF Frame Interface ///////////


GIFFrame *frameCreate(u16 frameWidth, u16 frameHeight, u16 imageLeftPosition, u16 imageTopPosition) {
    GIFFrame *newFrame = calloc(1, sizeof(GIFFrame));

    newFrame->imageLeftPosition = imageLeftPosition;
    newFrame->imageTopPosition  = imageTopPosition;

    newFrame->imageWidth        = frameWidth;
    newFrame->imageHeight       = frameHeight;

    newFrame->packedField_LocalColorTableFlag       = 0;
    newFrame->packedField_InterlaceFlag             = 0;
    newFrame->packedField_SortFlag                  = 0;
    newFrame->packedField_Reserved                  = 0;
    newFrame->packedField_SizeOfLocalColorTable     = 0;

    newFrame->localColorTable = NULL;

    newFrame->indexStream = NULL;

    return newFrame;
}

STATUS_CODE frameAddLocalColorTable(GIFFrame *frame, colorTable *clrTable) {
    FRAME_NULL_CHECK(frame);
    COLOR_TABLE_NULL_CHECK(clrTable);

    frame->packedField_LocalColorTableFlag   = 1;

    // Color table must be fully populated before this function call
    frame->packedField_SizeOfLocalColorTable = clrTable->size;
    
    frame->localColorTable = clrTable; 

    return OPERATION_SUCCESS;
}

STATUS_CODE frameAddIndexStream(GIFFrame *frame, array *indexStream) {
    FRAME_NULL_CHECK(frame);
    ARRAY_NULL_CHECK(indexStream);

    frame->indexStream = indexStream;

    return OPERATION_SUCCESS;
}

void freeFrame(GIFFrame *frame) {
    if (frame != NULL) {
        freeColorTable(frame->localColorTable);
        freeArray(frame->indexStream);

        free(frame);
    }
}
