
#ifndef GIF_INTERFACE_H
#define GIF_INTERFACE_H

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFCodeTable.h"
#include "GIFColorTable.h"

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
    colorTable *globalColorTable;

    // !TODO need linked list
    u32 numberOfFrames;
    GIFFrame **frames;

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
    // Local color table only used if
    // packedField_LocalColorTableFlag == 1
    //
    colorTable *localColorTable;

    //
    // Stream of integers between 0 and 255
    // each integer being equivalent to a pixel
    // using it's value as the color table index
    //
    array *indexStream;
    
} GIFFrame;

GIFCanvas *canvasCreate(u16 canvasWidth, u16 canvasHeight);
STATUS_CODE canvasAddGlobalColorTable(GIFCanvas *canvas, colorTable *clrTable);
STATUS_CODE canvasSetBackgroundColorIndex(GIFCanvas *canvas, u8 globalColorTableIndex);
STATUS_CODE canvasAddFrame(GIFCanvas *canvas, GIFFrame *frame);
void freeCanvas(GIFCanvas *canvas);

GIFFrame *frameCreate(u16 frameWidth, u16 frameHeight, u16 imageLeftPosition, u16 imageTopPosition);
STATUS_CODE frameAddLocalColorTable(GIFFrame *frame, colorTable *clrTable);
STATUS_CODE frameAddIndexStream(GIFFrame *frame, array *indexStream);
void freeFrame(GIFFrame *frame);

#endif