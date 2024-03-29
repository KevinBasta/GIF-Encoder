
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "linkedlist.h"
#include "GIFInterface.h"
#include "GIFColorTable.h"
#include "GIFCodeTable.h"
#include "GIFEncode.h"
#include "GIFError.h"

/**
 * @brief Encode a canvas and all it's frames
 * @param canvas            Canvas record to encode
 * @param createGIFOnError  If true and there was an
 * error while encoding the canvas record, encode a 
 * gif that says "ERROR" + the error that happened  
 * @param freeCanvasRecord  If true free the canvas
 * record passed in as the first argument
 * 
 * @return OPERATION_SUCCESS or error code
 */
WASM_EXPORT STATUS_CODE gif_createGIF(GIFCanvas *canvas, bool freeCanvasRecord, bool createGIFOnError) {
    STATUS_CODE status;

    CANVAS_NULL_CHECK(canvas);

    status = gif_encodeGIF(canvas);

    if (status != OPERATION_SUCCESS && createGIFOnError) {
        // Ignoring potential error code
        gif_createErrorGif(status);
    }

    if (freeCanvasRecord) {
        gif_freeCanvas(canvas);
    }

    return status;
}


/////////// GIF Canvas Interface ///////////

/**
 * @brief Create GIF canvas record
 * @param canvasWidth  Width in pixels
 * @param canvasHeight Height in pixels
 *
 * @return GIFCanvas pointer or NULL
 */
WASM_EXPORT GIFCanvas *gif_canvasCreate(u16 canvasWidth, u16 canvasHeight) {
    GIFCanvas *newCanvas = calloc(1, sizeof(GIFCanvas));
    if (newCanvas == NULL)
        return NULL;

    newCanvas->canvasWidth = canvasWidth;
    newCanvas->canvasHeight = canvasHeight;

    newCanvas->packedField_GlobalColorTableFlag     = 0;
    newCanvas->packedField_ColorResolution          = 0;
    newCanvas->packedField_SortFlag                 = 0;
    newCanvas->packedField_SizeOfGlobalColorTable   = 0;

    newCanvas->backgroundColorIndex = 0;
    newCanvas->pixelAspectRatio     = 0;

    newCanvas->globalColorTable = NULL;

    newCanvas->frames           = gif_linkedlistInit();
    if (newCanvas->frames == NULL) {
        free(newCanvas);
        return NULL;
    }

    newCanvas->fileName = NULL;

    return newCanvas;
}

/**
 * @brief There are two ways to add a global color table to a canvas.
 * [1]: Call to give colorTable struct pointer
 * [2]: Call to create one and call for each entry insert
 */

// METHOD #1
// Give a colorTable struct pointer directly to the canvas
WASM_EXPORT STATUS_CODE gif_canvasAddGlobalColorTable(GIFCanvas *canvas, gif_colorTable *clrTable) {
    CANVAS_NULL_CHECK(canvas);
    COLOR_TABLE_NULL_CHECK(clrTable);

    if (canvas->globalColorTable != NULL) {
        gif_freeColorTable(canvas->globalColorTable);
    }

    canvas->packedField_GlobalColorTableFlag = 1;
    
    // Color table must be fully populated before this function call
    // or the size of color table packed value may be incorrect
    u32 sizeLog = log2(clrTable->lastIndex);
    canvas->packedField_ColorResolution         = sizeLog;
    canvas->packedField_SizeOfGlobalColorTable  = sizeLog;

    canvas->globalColorTable = clrTable; 

    return OPERATION_SUCCESS;
}


// METHOD #2
// Create color table (colorTable struct) and append individual entries
WASM_EXPORT STATUS_CODE gif_canvasCreateGlobalColorTable(GIFCanvas *canvas) { 
    CANVAS_NULL_CHECK(canvas);

    if (canvas->globalColorTable != NULL) {
        gif_freeColorTable(canvas->globalColorTable);
    }

    gif_colorTable *clrTable = gif_colortableInit();
    COLOR_TABLE_NULL_CHECK(clrTable);

    canvas->packedField_GlobalColorTableFlag = 1;

    u32 sizeLog = log2(clrTable->lastIndex);
    canvas->packedField_ColorResolution         = sizeLog;
    canvas->packedField_SizeOfGlobalColorTable  = sizeLog;
    
    canvas->globalColorTable = clrTable;

    return OPERATION_SUCCESS;
}

WASM_EXPORT STATUS_CODE gif_canvasAddColorToColorTable(GIFCanvas *canvas, u8 red, u8 green, u8 blue) {
    STATUS_CODE status;
    
    CANVAS_NULL_CHECK(canvas);
    COLOR_TABLE_NULL_CHECK(canvas->globalColorTable);

    status = gif_colortableAppendRGB(canvas->globalColorTable, red, green, blue);       
    CHECKSTATUS(status);

    u32 sizeLog = log2(canvas->globalColorTable->lastIndex);
    canvas->packedField_ColorResolution         = sizeLog;
    canvas->packedField_SizeOfGlobalColorTable  = sizeLog;

    return OPERATION_SUCCESS;
}

/**
 * @brief Only for use if there is a global color table. Specify
 * which global color table index to use as the background
 * @param canvas                    Canvas  to specify background color of
 * @param globalColorTableIndex     Index in global color table to use
 *
 * @return OPERATION_SUCCESS or error code
 */
WASM_EXPORT STATUS_CODE gif_canvasSetBackgroundColorIndex(GIFCanvas *canvas, u8 globalColorTableIndex) {
    CANVAS_NULL_CHECK(canvas);

    if (canvas->globalColorTable == NULL)
        return CANVAS_BACKGROUND_WITHOUT_GLOBAL_CLR_TABLE;

    if (globalColorTableIndex > canvas->globalColorTable->lastIndex)
        return CANVAS_BACKGROUND_INDEX_OUT_OF_BOUNDS;

    canvas->backgroundColorIndex = globalColorTableIndex;
    
    return OPERATION_SUCCESS;
}

/**
 * @brief Add a frame to the canvas frames linked list
 * @param canvas    Canvas to add to
 * @param frame     Frame to add
 * 
 * @return OPERATION_SUCCESS or error code
 */
WASM_EXPORT STATUS_CODE gif_canvasAddFrame(GIFCanvas *canvas, GIFFrame *frame) {
    STATUS_CODE status;
    
    CANVAS_NULL_CHECK(canvas);
    FRAME_NULL_CHECK(frame);

    if (canvas->frames == NULL) {
        canvas->frames = gif_linkedlistInit();
    }

    status = gif_linkedlistAppend(canvas->frames, frame);
    CHECKSTATUS(status);

    return OPERATION_SUCCESS;
}

/**
 * @brief Set the output file name of the gif
 * @param canvas    Canvas to set name of
 * @param name      Name to set to. MUST CONTAIN .gif suffix.
 * Caller is expected to manage the memeory of this variable
 * if it's stored on the heap. 
 * @return OPERATION_SUCCESS or error code
 */
WASM_EXPORT STATUS_CODE gif_canvasSetFileName(GIFCanvas *canvas, char *newName) {
    CANVAS_NULL_CHECK(canvas);

    u32 newNameLength = strlen(newName);
    if (newNameLength == 0)
        return OPERATION_FAILED;

    if (canvas->fileName != NULL)
        free(canvas->fileName);

    canvas->fileName = calloc(newNameLength + 1, sizeof(char));
    strncpy(canvas->fileName, newName, newNameLength);
    canvas->fileName[newNameLength] = '\0';

    return OPERATION_SUCCESS;
}

// Free the canvas record and all it's frames
WASM_EXPORT void gif_freeCanvas(GIFCanvas *canvas) {
    if (canvas != NULL) {
        if (canvas->globalColorTable != NULL)
            gif_freeColorTable(canvas->globalColorTable);
        
        if (canvas->frames != NULL)
            gif_freeFrameLinkedList(canvas->frames);

        if (canvas->fileName != NULL)
            free(canvas->fileName);

        free(canvas);
    }
}





/////////// GIF Frame Interface ///////////


/**
 * @brief Create a GIF frame record
 * @param frameWidth        In pixels
 * @param frameHeight       In pixels
 * @param imageLeftPosition Pixels from the left before image start
 * @param imageTopPosition  Pixels from the top before image start
 * @return GIFFrame pointer or NULL
 */
WASM_EXPORT GIFFrame *gif_frameCreate(u16 frameWidth, u16 frameHeight, u16 imageLeftPosition, u16 imageTopPosition) {
    GIFFrame *newFrame = calloc(1, sizeof(GIFFrame));
    if (newFrame == NULL)
        return NULL;

    newFrame->imageLeftPosition = imageLeftPosition;
    newFrame->imageTopPosition  = imageTopPosition;

    newFrame->imageWidth        = frameWidth;
    newFrame->imageHeight       = frameHeight;

    newFrame->packedField_LocalColorTableFlag       = 0;
    newFrame->packedField_InterlaceFlag             = 0;
    newFrame->packedField_SortFlag                  = 0;
    newFrame->packedField_Reserved                  = 0;
    newFrame->packedField_SizeOfLocalColorTable     = 0;

    newFrame->packedField_GCE_Reserved              = 0;
    newFrame->packedField_GCE_DisposalMethod        = 2;
    newFrame->packedField_GCE_UserInputFlag         = 0;
    newFrame->packedField_GCE_TransparentColorFlag  = 0;

    newFrame->delayTime             = 0;
    newFrame->transparentColorIndex = 0;

    newFrame->localColorTable = NULL;

    newFrame->indexStream = NULL;

    newFrame->persistentFrameEncodeData = false;
    newFrame->imageData = NULL;

    return newFrame;
}

/**
 * @brief There are two ways to add a color table to a frame.
 * [1]: Call to give colorTable struct pointer
 * [2]: Call to create one and call for each entry insert
 */

// METHOD #1
// Give a colorTable struct pointer directly to the frame
WASM_EXPORT STATUS_CODE gif_frameAddLocalColorTable(GIFFrame *frame, gif_colorTable *clrTable) {
    FRAME_NULL_CHECK(frame);
    COLOR_TABLE_NULL_CHECK(clrTable);

    if (frame->localColorTable != NULL) {
        gif_freeColorTable(frame->localColorTable);
    }

    frame->packedField_LocalColorTableFlag   = 1;

    // Color table must be fully populated before this function call
    // or the size of color table packed value may be incorrect
    u32 sizeLog = log2(clrTable->lastIndex);
    frame->packedField_SizeOfLocalColorTable = sizeLog;
    
    frame->localColorTable = clrTable; 

    return OPERATION_SUCCESS;
}


// METHOD #2
// Create color table (colorTable struct) and append individual entries
WASM_EXPORT STATUS_CODE gif_frameCreateLocalColorTable(GIFFrame *frame) { 
    FRAME_NULL_CHECK(frame);

    if (frame->localColorTable != NULL) {
        gif_freeColorTable(frame->localColorTable);
    }

    gif_colorTable *clrTable = gif_colortableInit();
    COLOR_TABLE_NULL_CHECK(clrTable);

    frame->packedField_LocalColorTableFlag   = 1;
    
    u32 sizeLog = log2(clrTable->lastIndex);
    frame->packedField_SizeOfLocalColorTable   = sizeLog;
    
    frame->localColorTable = clrTable; 

    return OPERATION_SUCCESS;
}

WASM_EXPORT STATUS_CODE gif_frameAddColorToColorTable(GIFFrame *frame, u8 red, u8 green, u8 blue) {
    STATUS_CODE status;
    
    FRAME_NULL_CHECK(frame);
    COLOR_TABLE_NULL_CHECK(frame->localColorTable);

    status = gif_colortableAppendRGB(frame->localColorTable, red, green, blue);       
    CHECKSTATUS(status);

    u32 sizeLog = log2(frame->localColorTable->lastIndex);
    frame->packedField_SizeOfLocalColorTable = sizeLog;

    return OPERATION_SUCCESS;
}

/**
 * @brief There are three ways to add an index stream
 * to a frame.
 * [1]: Call to give an array struct pointer
 * [2]: Call to give a stack array and it's size
 * [3]: Call to create one and call for each entry insert
 */

// METHOD #1
// Give the array struct pointer directly to the frame
WASM_EXPORT STATUS_CODE gif_frameAddIndexStream(GIFFrame *frame, gif_array *indexStream) {
    FRAME_NULL_CHECK(frame);
    ARRAY_NULL_CHECK(indexStream);

    if (frame->indexStream != NULL) {
        gif_freeArray(frame->indexStream);
    }

    frame->indexStream = indexStream;

    return OPERATION_SUCCESS;
}

// METHOD #2
// Give an array pointer and its size to the frame 
// NOTE: size MUST BE EXACT!
WASM_EXPORT STATUS_CODE gif_frameAddIndexStreamFromArray(GIFFrame *frame, u8 stackArr[], size_t size) {
    FRAME_NULL_CHECK(frame);

    if (frame->indexStream != NULL) {
        gif_freeArray(frame->indexStream);
    }

    frame->indexStream = gif_arrayInitFromStackArray((u8*)stackArr, size);
    ARRAY_NULL_CHECK(frame->indexStream);

    return OPERATION_SUCCESS;
}

// METHOD #3
// Create index stream (array struct) and append individual entries
// NOTE: indexStreamSize MUST BE EXACT!
WASM_EXPORT STATUS_CODE gif_frameCreateIndexStream(GIFFrame *frame, size_t indexStreamSize) {
    FRAME_NULL_CHECK(frame);

    if (frame->indexStream != NULL) {
        gif_freeArray(frame->indexStream);
    }

    frame->indexStream = gif_arrayInit(indexStreamSize);
    ARRAY_NULL_CHECK(frame->indexStream);

    return OPERATION_SUCCESS;
}

WASM_EXPORT STATUS_CODE gif_frameAppendToIndexStream(GIFFrame *frame, u32 item) {
    STATUS_CODE status;
    
    FRAME_NULL_CHECK(frame);
    ARRAY_NULL_CHECK(frame->indexStream);

    // TODO! On encode zero the rest of the array if it is not full
    status = gif_arrayAppend(frame->indexStream, item);
    CHECKSTATUS(status);

    return OPERATION_SUCCESS;
}

/**
 * @brief Add animation information to the frame record
 * @param frame             The frame to add to
 * @param disposalMethod    What to do with the frame
 * when the next frame comes up. There are three options:
 * [1] leave the image in place and draw the next image on top of it
 * [2] canvas should be restored to the background color
 * [3] restore the canvas to it's previous state before the current image was drawn
 * @param delayTime The number of hundredths of a second to wait before next frame
 * 
 * @return OPERATION_SUCCESS or error code
 */
WASM_EXPORT STATUS_CODE gif_frameAddGraphicsControlInfo(GIFFrame *frame, u8 disposalMethod, u16 delayTime) {
    FRAME_NULL_CHECK(frame);

    frame->packedField_GCE_DisposalMethod = disposalMethod;
    frame->delayTime                      = delayTime;

    return OPERATION_SUCCESS;
}

/**
 * @brief Set a color in the color table to be a transparent color.
 * Applys to whichever color table is currently active (global or local).
 * @param frame                     Frame to specify the transparent color for
 * @param transparentColorIndex     Index of the color table to use as the color
 * 
 * @return OPERATION_SUCCESS or error code
 */
WASM_EXPORT STATUS_CODE gif_frameSetTransparanetColorIndexInColorTable(GIFFrame *frame, u8 transparentColorIndex) {
    FRAME_NULL_CHECK(frame);

    frame->packedField_GCE_TransparentColorFlag = 1;
    frame->transparentColorIndex                = transparentColorIndex;

    return OPERATION_SUCCESS;
}

// Free a frame record
WASM_EXPORT void gif_freeFrame(GIFFrame *frame) {
    if (frame != NULL) {
        if (frame->localColorTable != NULL)
            gif_freeColorTable(frame->localColorTable);

        if (frame->indexStream != NULL)
            gif_freeArray(frame->indexStream);

        if (frame->imageData != NULL)
            gif_freeBitArray(frame->imageData);

        free(frame);
    }
}
