
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <stdint.h>
#include "main.h"

#include "bitarray.h"
#include "bitUtility.h"
#include "printUtility.h"

STATUS_CODE zeroRestOfByte(u8 *item, u8 currentBit) {
    for (u8 i = currentBit; i < 8; i++) {
        *item &= ~(1 << i);
    }
}

STATUS_CODE bitarrayBookMark(bitarray *arr) {
    if (arr == NULL)
        return OPERATION_FAILED;

    arr->mark = &(arr->items[arr->currentIndex]);

    return OPERATION_SUCCESS;
}

STATUS_CODE bitarraySetBookMarkValue(bitarray *arr, u8 item) {
    if (arr == NULL)
        return OPERATION_FAILED;

    if (arr->mark == NULL)
        return OPERATION_FAILED;

    *(arr->mark) = item;
    printf("%d %d\n", *(arr->mark), item);

    return OPERATION_SUCCESS;
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
    if (arr == NULL)
        return OPERATION_FAILED;

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

STATUS_CODE bitarrayAppendPacked(bitarray *arr, u32 item) {
    if (arr == NULL)
        return OPERATION_FAILED;

    if (arr->currentIndex + 1 >= arr->size) {
        // realloc
    }

    u32 itemBitLength = getOccupiedBits(item);
    u32 remainingBitsInCurrentArrayByte = 8 - arr->currentBit;


    if (remainingBitsInCurrentArrayByte >= itemBitLength) {
        u8 currentItem = arr->items[arr->currentIndex];

        arr->items[arr->currentIndex] = currentItem | (item << (remainingBitsInCurrentArrayByte - itemBitLength));
        arr->currentBit += itemBitLength;
        
        if (arr->currentBit == 8) {
            arr->currentIndex++;
            arr->currentBit = 0;
        }
    } else {
        u32 remainingBitsToBeWritten = itemBitLength;
        while (remainingBitsToBeWritten > 0) {
            u8 currentItem = arr->items[arr->currentIndex];

            if (remainingBitsToBeWritten > remainingBitsInCurrentArrayByte) {
                u32 leftOverBits = (remainingBitsToBeWritten - remainingBitsInCurrentArrayByte);
                
                arr->items[arr->currentIndex] = currentItem | (item >> leftOverBits);
                arr->currentBit += remainingBitsToBeWritten - leftOverBits;
                
                remainingBitsToBeWritten -= remainingBitsInCurrentArrayByte;
            } else { 
                arr->items[arr->currentIndex] = currentItem | (item << (remainingBitsInCurrentArrayByte - remainingBitsToBeWritten));
                arr->currentBit += remainingBitsToBeWritten;
                
                remainingBitsToBeWritten -= min(8, remainingBitsToBeWritten); // min not required
            }

            if (arr->currentBit == 8) {
                (arr->currentIndex)++;
                arr->currentBit = 0;
            }

            remainingBitsInCurrentArrayByte = 8 - arr->currentBit;
        }
    }

    /* printf("current bit: %ld current index: %ld\n", arr->currentBit, arr->currentIndex);
    printIntBits(&(arr->items[0]), sizeof(u8));
    printIntBits(&(arr->items[1]), sizeof(u8));
    printIntBits(&(arr->items[2]), sizeof(u8)); */

    return OPERATION_SUCCESS;
}

STATUS_CODE bitarrayAppendPackedNormalized(bitarray *arr, u32 item, u32 occupiedBits, u32 minNumberOfBits) {
    if (arr == NULL)
        return OPERATION_FAILED;

    if (minNumberOfBits + 1 < occupiedBits) {
        printf("LESS THAN\n");
        return OPERATION_FAILED;
    }

    if (arr->currentIndex + 1 >= arr->size) {
        // realloc
    }


    u32 remainingBitsInCurrentArrayByte = 8 - arr->currentBit;

    if (remainingBitsInCurrentArrayByte >= minNumberOfBits) {
        u8 currentItem = arr->items[arr->currentIndex];

        arr->items[arr->currentIndex] = currentItem | (item << (remainingBitsInCurrentArrayByte - minNumberOfBits));
        arr->currentBit += minNumberOfBits;
        
        if (arr->currentBit == 8) {
            arr->currentIndex++;
            arr->currentBit = 0;
        }
    } else {
        u32 remainingBitsToBeWritten = minNumberOfBits;
        while (remainingBitsToBeWritten > 0) {
            u8 currentItem = arr->items[arr->currentIndex];

            if (remainingBitsToBeWritten > remainingBitsInCurrentArrayByte) {
                u32 leftOverBits = (remainingBitsToBeWritten - remainingBitsInCurrentArrayByte);
                
                arr->items[arr->currentIndex] = currentItem | (item >> leftOverBits);
                arr->currentBit += remainingBitsToBeWritten - leftOverBits;
                
                remainingBitsToBeWritten -= remainingBitsInCurrentArrayByte;
            } else { 
                arr->items[arr->currentIndex] = currentItem | (item << (remainingBitsInCurrentArrayByte - remainingBitsToBeWritten));
                arr->currentBit += remainingBitsToBeWritten;
                
                remainingBitsToBeWritten -= min(8, remainingBitsToBeWritten); // min not required
            }

            if (arr->currentBit == 8) {
                (arr->currentIndex)++;
                arr->currentBit = 0;
            }

            remainingBitsInCurrentArrayByte = 8 - arr->currentBit;
        }
    }

    /* printf("current bit: %ld current index: %ld\n", arr->currentBit, arr->currentIndex);
    printIntBits(&(arr->items[0]), sizeof(u8));
    printIntBits(&(arr->items[1]), sizeof(u8));
    printIntBits(&(arr->items[2]), sizeof(u8));
    printIntBits(&(arr->items[3]), sizeof(u8));
    printIntBits(&(arr->items[4]), sizeof(u8)); */

    return OPERATION_SUCCESS;
}

void bitarrayPrint(bitarray *arr) {
    size_t index = 0;

    for (size_t i = 0; i < arr->currentIndex; i++) {
        printf("%d ", arr->items[i]);
        index++;
        if (index % 24 == 0) {
            printf("\n");
        }
    }

    printf("\n");
}

/* int main() {
    STATUS_CODE status;
    
    bitarray *arr = bitarrayInit(100);
    status = bitarrayAppendPackedNormalized(arr, 4, 4);
    status = bitarrayAppendPackedNormalized(arr, 2, 5);
    status = bitarrayAppendPackedNormalized(arr, 4001, 13); //1111 1010 0001
    status = bitarrayAppendPackedNormalized(arr, 1, 5);
    status = bitarrayAppendPackedNormalized(arr, 5, 3);
} */