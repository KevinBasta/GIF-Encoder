
#ifndef GIF_ENCODER_BIT_ARRAY_H
#define GIF_ENCODER_BIT_ARRAY_H

#include <stdbool.h>
#include <stdint.h>
#include "main.h"

typedef struct gif_bitarray {
    size_t size;
    size_t currentIndex;
    u8     currentBit;
    
    bool   intervalInsertFlag;
    size_t intervalInsertBoundry;
    u8     intervalInsertItem;
    long   markIndex;
    
    u8 *items;
} gif_bitarray;

gif_bitarray *gif_bitarrayInit(size_t size);
STATUS_CODE gif_bitarrayAppend(gif_bitarray *arr, u8 item);
STATUS_CODE gif_bitarrayAppendPackedLeft(gif_bitarray *arr, u32 item, u32 minNumberOfBits);
STATUS_CODE gif_bitarrayAppendPackedRight(gif_bitarray *arr, u32 item, u32 minNumberOfBits);

STATUS_CODE gif_bitarrayBookMark(gif_bitarray *arr, u32 offset);
STATUS_CODE gif_bitarraySetBookMarkValue(gif_bitarray *arr, u8 item);
STATUS_CODE gif_bitarraySetIntervalInsertRule(gif_bitarray *arr, size_t boundry, u8 insertItem);

void gif_bitarrayPrint(gif_bitarray *arr);
void gif_freeBitArray(gif_bitarray *arr);

#endif // GIF_ENCODER_BIT_ARRAY_H