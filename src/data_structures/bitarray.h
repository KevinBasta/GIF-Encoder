
#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdint.h>
#include "main.h"

typedef struct bitarray {
    size_t size;
    size_t currentIndex;
    u8 currentBit: 4;
    u8 *mark;
    u8 *items;
} bitarray;

STATUS_CODE bitarrayBookMark(bitarray *arr);
STATUS_CODE bitarraySetBookMarkValue(bitarray *arr, u8 item);

bitarray *bitarrayInit(size_t size);
STATUS_CODE bitarrayAppend(bitarray *arr, u8 item);
STATUS_CODE bitarrayAppendPacked(bitarray *arr, u32 item);
STATUS_CODE bitarrayAppendPackedNormalized(bitarray *arr, u32 item, u32 occupiedBits, u32 minNumberOfBits);
void bitarrayPrint(bitarray *arr);

#endif // BIT_ARRAY_H