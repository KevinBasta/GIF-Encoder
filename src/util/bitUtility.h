
#ifndef GIF_BIT_UTIL_H
#define GIF_BIT_UTIL_H

#include <stdint.h>
#include "main.h"

bool gif_compareNBytes(u8 *firstItem, u8 secondItem[], u32 numberOfBytes);
u32 gif_getOccupiedBits(u32 item);
u8 gif_getNBits(u32 startBit, u32 endBit, u8 data);
u8 gif_getNthBit(u32 bitNumber, u8 data);

#endif // GIF_BIT_UTIL_H