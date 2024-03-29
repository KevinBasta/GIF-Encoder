
#ifndef GIF_INTERFACE_H
#define GIF_INTERFACE_H

#include <stdbool.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "bitarray.h"
#include "linkedlist.h"
#include "GIFCodeTable.h"
#include "GIFColorTable.h"

// In bits for both packed bytes
#define CANVAS_GLOBAL_COLOR_TABLE_FLAG_OFFSET       7
#define CANVAS_COLOR_RESOLUTION_OFFSET              4
#define CANVAS_SORT_FLAG_OFFSET                     3 
#define CANVAS_SIZE_OF_GLOBAL_COLOR_TABLE_OFFSET    0

#define FRAME_LOCAL_COLOR_TABLE_FLAG_OFFSET         7
#define FRAME_INTERLACE_FLAG_OFFSET                 6
#define FRAME_SORT_FLAG_OFFSET                      5
#define FRAME_RESERVED_OFFSET                       3
#define FRAME_SIZE_OF_LOCAL_COLOR_TABLE_OFFSET      0

#define GRAPHICS_CONTROL_RESERVED_OFFSET                5
#define GRAPHICS_CONTROL_DISPOSAL_METHOD_OFFSET         2
#define GRAPHICS_CONTROL_USER_INPUT_FLAG_OFFSET         1
#define GRAPHICS_CONTROL_TRANSPARENT_COLOR_FLAG_OFFSET  0

// Main Storage Structs //

typedef struct GIFCanvas {
    //
    // LogicalScreenDescriptor Fields Start
    //

    // In pixels
    u16 canvasWidth; 
    u16 canvasHeight;
    
    //
    // Packed field:
    // [7]    : global color table flag
    //        : [1] global color table will follow
    //        : [0] no global color table
    //
    // [4-6]  : color resolution
    //        : 2^(n+1) computes entries in global color table
    //
    // [3]    : sort flag
    //        : [1] global color table sorted in decreasing importance
    //        : [0] not sorted
    //
    // [0-2]  : (!unused) called the "size of global color table"
    //
    u8 packedField_GlobalColorTableFlag:   1;
    u8 packedField_ColorResolution:        3;
    u8 packedField_SortFlag:               1;
    u8 packedField_SizeOfGlobalColorTable: 3;
    
    //
    // Which index in the global color table should 
    // be used for pixels on the virtual canvas that 
    // aren't overlayed by an image.
    //
    u8 backgroundColorIndex;
    
    // Ignored Field
    u8 pixelAspectRatio;

    //
    // LogicalScreenDescriptor Fields End
    //

    //
    // Global color table only used if
    // packedField_GlobalColorTableFlag == 1
    //
    gif_colorTable *globalColorTable;

    gif_linkedlist *frames;

    //
    // General canvas data
    //
    char *fileName;

} GIFCanvas;


typedef struct GIFFrame {
    //
    // ImageDescriptor Fields Start
    //

    // Where the image should begin on the canvas
    u16 imageLeftPosition;
    u16 imageTopPosition;

    // In pixels
    u16 imageWidth;
    u16 imageHeight;

    //
    // Packed field:
    // [7]   : local color table flag 
    //       : [1] use local color table that follows
    //       : [0] no local color table
    //
    // [6]   : interlace flag
    //       : changes the way images are rendered onto the screen to reduce flicker
    //       : requires encoding in a specific order 
    //
    // [5]   : sort flag 
    //       : [1] local color table sorted in decreasing importance 
    //       : [0] not sorted
    //
    // [3-4] : Reserved
    //
    // [0-2] : size of local color table
    //       : 2^(n+1) computes entries in local color table
    //
    u8 packedField_LocalColorTableFlag:   1;
    u8 packedField_InterlaceFlag:         1;
    u8 packedField_SortFlag:              1;
    u8 packedField_Reserved:              2;
    u8 packedField_SizeOfLocalColorTable: 3;
    
    //
    // ImageDescriptor Fields End
    //


    //
    // Graphic Control Extention Fields Start
    //

    //
    // Packed Field:
    // [5-7] : reserved for future use
    //
    // [2-4] : disposal method
    //       : [1] leave the image in place and draw the next image on top of it
    //       : [2] canvas should be restored to the background color
    //       : [3] restore the canvas to it's previous state before the current image was drawn
    //
    // [1]   : user input flag
    //       : [1] wait for some input from viewer beofre moving to next scene
    //       : [0] don't wait for input
    //
    // [0]   : transparent color flag
    //       : [1] transparancy used
    //       : [0] transparancy not used
    //
    u8 packedField_GCE_Reserved:             3;
    u8 packedField_GCE_DisposalMethod:       3;
    u8 packedField_GCE_UserInputFlag:        1;
    u8 packedField_GCE_TransparentColorFlag: 1;

    // number of hundredths of a second to wait before moving on to the next scene
    u16 delayTime;
    u8 transparentColorIndex;

    //
    // Graphic Control Extention Fields End
    //


    //
    // Local color table only used if
    // packedField_LocalColorTableFlag == 1
    //
    gif_colorTable *localColorTable;

    //
    // Stream of integers between 0 and 255
    // each integer being equivalent to a pixel
    // using it's value as the color table index
    //
    gif_array *indexStream;
    
    // 
    // Optimization fields to avoid
    // re-encoding the same frames
    //
    bool persistentFrameEncodeData;
    gif_bitarray *imageData;

} GIFFrame;


// Encode the canvas record
STATUS_CODE gif_createGIF(GIFCanvas *canvas, bool freeCanvasRecord, bool createGIFOnError);


// Create a canvas record
GIFCanvas *gif_canvasCreate(u16 canvasWidth, u16 canvasHeight);

STATUS_CODE gif_canvasAddGlobalColorTable(GIFCanvas *canvas, gif_colorTable *clrTable);
STATUS_CODE gif_canvasCreateGlobalColorTable(GIFCanvas *canvas);
STATUS_CODE gif_canvasAddColorToColorTable(GIFCanvas *canvas, u8 red, u8 green, u8 blue);

STATUS_CODE gif_canvasSetBackgroundColorIndex(GIFCanvas *canvas, u8 globalColorTableIndex);
STATUS_CODE gif_canvasAddFrame(GIFCanvas *canvas, GIFFrame *frame);

STATUS_CODE gif_canvasSetFileName(GIFCanvas *canvas, char *newName);

void gif_freeCanvas(GIFCanvas *canvas);


// Create a frame record
GIFFrame *gif_frameCreate(u16 frameWidth, u16 frameHeight, u16 imageLeftPosition, u16 imageTopPosition);

STATUS_CODE gif_frameAddLocalColorTable(GIFFrame *frame, gif_colorTable *clrTable);
STATUS_CODE gif_frameCreateLocalColorTable(GIFFrame *frame);
STATUS_CODE gif_frameAddColorToColorTable(GIFFrame *frame, u8 red, u8 green, u8 blue);

STATUS_CODE gif_frameAddIndexStream(GIFFrame *frame, gif_array *indexStream);
STATUS_CODE gif_frameAddIndexStreamFromArray(GIFFrame *frame, u8 stackArr[], size_t size);
STATUS_CODE gif_frameCreateIndexStream(GIFFrame *frame, size_t indexStreamSize);
STATUS_CODE gif_frameAppendToIndexStream(GIFFrame *frame, u32 item);

STATUS_CODE gif_frameAddGraphicsControlInfo(GIFFrame *frame, u8 disposalMethod, u16 delayTime);
STATUS_CODE gif_frameSetTransparanetColorIndexInColorTable(GIFFrame *frame, u8 transparentColorIndex);
void gif_freeFrame(GIFFrame *frame);

#endif // GIF_INTERFACE_H