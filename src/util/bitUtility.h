
#ifndef BIT_UTIL_H
#define BIT_UTIL_H

#include <stdint.h>
#include "main.h"

bool compareNBytes(u8 *firstItem, u8 secondItem[], u32 numberOfBytes);
u32 getOccupiedBits(u32 item);
u8 getNBits(u32 startBit, u32 endBit, u8 data);
u8 getNthBit(u32 bitNumber, u8 data);

#endif // BIT_UTIL_H