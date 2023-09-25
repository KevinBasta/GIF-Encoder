
#ifndef GIF_LZW_COMPRESSION_H
#define GIF_LZW_COMPRESSION_H

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "bitarray.h"
#include "GIFColorTable.h"

STATUS_CODE gif_createLZWImageData(gif_colorTable *clrTable, gif_array *indexStream, gif_bitarray *imageData);
STATUS_CODE gif_createLZWImageDataInitialDraft(gif_colorTable *clrTable, gif_array *indexStream, gif_bitarray *imageData);

#endif // GIF_LZW_COMPRESSION_H