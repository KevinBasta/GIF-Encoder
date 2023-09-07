
#ifndef GIF_ENCODE_H
#define GIF_ENCODE_H

#include <stdint.h>
#include "main.h"

#include "GIFColorTable.h"
#include "GIFInterface.h"

STATUS_CODE encodeGIF(GIFGlobalRecord *gifData);
void addRGBArrayEntry(RGB *table, u32 index, u8 red, u8 green, u8 blue);

#endif // GIF_ENCODE_H