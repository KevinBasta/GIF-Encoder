
#ifndef STORAGE_TYPES_H
#define STORAGE_TYPES_H

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "typesGIF.h"

// Main Storage Structs //

typedef struct GIF_Data {
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
} GIF_Data;

#endif // STORAGE_TYPES_H
