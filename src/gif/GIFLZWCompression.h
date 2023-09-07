
#ifndef GIF_LZW_COMPRESSION_H
#define GIF_LZW_COMPRESSION_H

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "bitarray.h"
#include "GIFColorTable.h"

STATUS_CODE createLZWImageData(colorTable *clrTable, array *indexStream, bitarray *imageData);
STATUS_CODE createLZWImageDataInitialDraft(colorTable *clrTable, array *indexStream, bitarray *imageData);

#endif // GIF_LZW_COMPRESSION_H