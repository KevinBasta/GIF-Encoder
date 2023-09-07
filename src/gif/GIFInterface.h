
#ifndef GIF_INTERFACE_H
#define GIF_INTERFACE_H

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFCodeTable.h"
#include "GIFColorTable.h"

// Main Storage Structs //

typedef struct GIFGlobalRecord {
    // LogicalScreenDescriptor
    u16 canvasWidth; 
    u16 canvasHeight;
    u8 packedFieldCanvas;
    u8 backgroundColorIndex;
    u8 pixelAspectRatio;
    
    colorTable *globalColorTable;

    // ImageDescriptor
    u16 imageLeftPosition;
    u16 imageTopPosition;
    u16 imageWidth;
    u16 imageHeight;
    u8 packedFieldImage;

    colorTable *localColorTable;

    array *indexStream;
} GIFGlobalRecord;


typedef struct GIFFrameRecord {
    // ImageDescriptor
    u16 imageLeftPosition;
    u16 imageTopPosition;
    u16 imageWidth;
    u16 imageHeight;
    u8 packedFieldImage;

    colorTable *localColorTable;
    array *indexStream;
} GIFFrameRecord;


#endif