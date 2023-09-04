
#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdbool.h>
#include <stdint.h>
#include "main.h"

typedef struct bitarray {
    size_t size;
    size_t currentIndex;
    u8     currentBit: 4;
    
    bool   intervalInsertFlag;
    size_t intervalInsertBoundry;
    u8     intervalInsertItem;
    long   markIndex;
    
    u8 *items;
} bitarray;

STATUS_CODE bitarrayBookMark(bitarray *arr, u32 offset);
STATUS_CODE bitarraySetBookMarkValue(bitarray *arr, u8 item);
STATUS_CODE bitarraySetIntervalInsertRule(bitarray *arr, size_t boundry, u8 insertItem);

bitarray *bitarrayInit(size_t size);
STATUS_CODE bitarrayAppend(bitarray *arr, u8 item);
STATUS_CODE bitarrayAppendPacked(bitarray *arr, u32 item);
STATUS_CODE bitarrayAppendPackedNormalizedLeft(bitarray *arr, u32 item, u32 occupiedBits, u32 minNumberOfBits);
STATUS_CODE bitarrayAppendPackedNormalizedRight(bitarray *arr, u32 item, u32 minNumberOfBits);
void bitarrayPrint(bitarray *arr);
void freeBitArray(bitarray *arr);

#endif // BIT_ARRAY_H