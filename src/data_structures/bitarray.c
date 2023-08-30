
#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include "main.h"

#include "bitarray.h"

STATUS_CODE zeroRestOfByte(u8 *item, u8 currentBit) {
    for (u8 i = currentBit; i < 8; i++) {
        *item &= ~(1 << i);
    }
}

bitarray *bitarrayInit(size_t size) {
    bitarray *arr = calloc(1, sizeof(bitarray));

    arr->items = calloc(size, sizeof(u8));
    arr->size = size;
    arr->currentIndex = 0;
    arr->currentBit = 0;
    
    return arr;
}

STATUS_CODE bitarrayAppend(bitarray *arr, u8 item) {
    if (arr == NULL) {
        return OPERATION_FAILED;
    }

    if (arr->currentIndex + 1 >= arr->size) {
        // realloc
    }
    
    if (arr->currentBit != 0) {
        zeroRestOfByte(&(arr->items[arr->currentIndex]), arr->currentBit);
        arr->currentBit = 0;
        arr->currentIndex++;
    }

    arr->items[arr->currentIndex] = item;
    arr->currentIndex++;
    
    return OPERATION_SUCCESS;
}

/**
 * @brief Compute the min number of bits it takes
 * (with no leading zeros) to represent the integer
 * @param item integer getting the bitsize of
 * @return number of bits the item occupies
 */
u32 getOccupiedBits(u32 item) {
    // count bits from left side to first non zero bit
    u32 leadingBits = 0;

    for (u32 i = 0; i < sizeof(item) * 8; i++) {
        bool isZero = (item & (1 << (sizeof(item) - i - 1))) == 0;

        if (isZero == false) {
            break;
        } else {
            leadingBits++;
        }
    }

    return sizeof(item) - leadingBits;
}

STATUS_CODE bitarrayAppendPacked(bitarray *arr, u32 item) {
    u32 itemBitLength = getOccupiedBits(item);
    u32 remainingBitsInCurrentArrayByte = 7 - arr->currentBit;

    if (remainingBitsInCurrentArrayByte > itemBitLength) {
        for (u32 i = 0; i < itemBitLength; i++) {
            u8 currentItem = arr->items[arr->currentIndex];
            //arr->items[arr->currentIndex] = currentItem
        }
    }

}
