
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

STATUS_CODE bitarrayBookMark(bitarray *arr, u32 offset) {
    if (arr == NULL)
        return OPERATION_FAILED;

    arr->markIndex = arr->currentIndex + offset;

    return OPERATION_SUCCESS;
}

STATUS_CODE bitarraySetBookMarkValue(bitarray *arr, u8 item) {
    if (arr == NULL)
        return OPERATION_FAILED;

    if (arr->markIndex != -1) {
        arr->items[arr->markIndex] = item;
        printf("%d %d\n", arr->items[arr->markIndex], item);
    }

    return OPERATION_SUCCESS;
}


void setMarkValueAndMarkNewLocation(bitarray *arr) {
    if (arr->intervalInsertFlag == true) {
        //
        // NOTE: (arr->currentIndex - 1) because this function is
        // expected to be called after every current index increment
        //
        if ((arr->currentIndex - 1) % arr->intervalInsertBoundry == 0) {
            bitarraySetBookMarkValue(arr, arr->intervalInsertItem);
            
            bitarrayBookMark(arr, 0);
            bitarrayAppend(arr, 0);
        }
    }
}


bitarray *bitarrayInit(size_t size) {
    bitarray *arr = calloc(1, sizeof(bitarray));

    arr->items = calloc(size, sizeof(u8));
    arr->size = size;
    arr->currentIndex = 0;
    arr->currentBit   = 0;
    
    arr->intervalInsertFlag = false;
    arr->intervalInsertBoundry = 0;
    arr->intervalInsertItem    = 0;
    arr->markIndex             = -1;
    
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
        (arr->currentIndex)++;
        setMarkValueAndMarkNewLocation(arr);
    }

    arr->items[arr->currentIndex] = item;
    (arr->currentIndex)++;
    setMarkValueAndMarkNewLocation(arr);
    
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
            (arr->currentIndex)++;
            arr->currentBit = 0;
            setMarkValueAndMarkNewLocation(arr);
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
                setMarkValueAndMarkNewLocation(arr);
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

STATUS_CODE bitarrayAppendPackedNormalizedLeft(bitarray *arr, u32 item, u32 occupiedBits, u32 minNumberOfBits) {
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
            (arr->currentIndex)++;
            arr->currentBit = 0;
            setMarkValueAndMarkNewLocation(arr);
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
                setMarkValueAndMarkNewLocation(arr);
            }

            remainingBitsInCurrentArrayByte = 8 - arr->currentBit;
        }
    }

   /*  printf("current bit: %ld current index: %ld\n", arr->currentBit, arr->currentIndex);
    printIntBits(&(arr->items[0]), sizeof(u8));
    printIntBits(&(arr->items[1]), sizeof(u8));
    printIntBits(&(arr->items[2]), sizeof(u8));
    printIntBits(&(arr->items[3]), sizeof(u8));
    printIntBits(&(arr->items[4]), sizeof(u8)); */

    return OPERATION_SUCCESS;
}


STATUS_CODE bitarraySetIntervalInsertRule(bitarray *arr, size_t boundry, u8 insertItem) {
    if (arr == NULL)
        return OPERATION_FAILED;
    
    arr->intervalInsertFlag    = true;
    arr->intervalInsertBoundry = boundry;
    arr->intervalInsertItem    = insertItem;

    return OPERATION_SUCCESS;
}

STATUS_CODE bitarrayAppendPackedNormalizedRight(bitarray *arr, u32 item, u32 minNumberOfBits) {
    if (arr == NULL)
        return OPERATION_FAILED;

    u32 occupiedBits = getOccupiedBits(item);
    if (minNumberOfBits < occupiedBits) {
        printf("LESS THAN\n");
        //return OPERATION_FAILED;
    }

    if (arr->currentIndex + 1 >= arr->size) {
        // realloc
    }


    u32 remainingBitsInCurrentArrayByte = 8 - arr->currentBit;

    if (remainingBitsInCurrentArrayByte >= minNumberOfBits) {
        u8 currentItem = arr->items[arr->currentIndex];

        arr->items[arr->currentIndex] = currentItem | (item << (arr->currentBit));
        arr->currentBit += minNumberOfBits;
        
        if (arr->currentBit == 8) {
            (arr->currentIndex)++;
            arr->currentBit = 0;
            setMarkValueAndMarkNewLocation(arr);
        }
    } else {
        u32 remainingBitsToBeWritten = minNumberOfBits;
        u32 bitsWrittenSoFar = 0;
        while (remainingBitsToBeWritten > 0) {
            u8 currentItem = arr->items[arr->currentIndex];

            if (remainingBitsToBeWritten > remainingBitsInCurrentArrayByte) {
                u32 leftOverBits = (remainingBitsToBeWritten - remainingBitsInCurrentArrayByte);
                
                //printIntBits(&item, sizeof(u8));
                u32 itemShifted = (item >> bitsWrittenSoFar) << arr->currentBit;
                //printIntBits(&itemShifted, sizeof(u8));
                arr->items[arr->currentIndex] = currentItem | itemShifted;
                arr->currentBit += remainingBitsToBeWritten - leftOverBits;
                bitsWrittenSoFar += remainingBitsToBeWritten - leftOverBits;
                
                remainingBitsToBeWritten -= remainingBitsInCurrentArrayByte;
            } else { 
                arr->items[arr->currentIndex] = currentItem | ((item >> bitsWrittenSoFar) << (arr->currentBit));
                arr->currentBit += remainingBitsToBeWritten;
                bitsWrittenSoFar += remainingBitsToBeWritten;
                
                remainingBitsToBeWritten -= min(8, remainingBitsToBeWritten); // min not required
            }

            if (arr->currentBit == 8) {
                (arr->currentIndex)++;
                arr->currentBit = 0;
                setMarkValueAndMarkNewLocation(arr);
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
        printf("%X ", arr->items[i]);
        index++;
        if (index % 24 == 0) {
            printf("\n");
        }
    }

    printf("\n");
}

void freeBitArray(bitarray *arr) {
    free(arr->items);
    free(arr);
}


/* int main() {
    STATUS_CODE status;
    
    bitarray *arr = bitarrayInit(100);
    status = bitarrayAppendPackedNormalizedRight(arr, 4, 3, 4);
    status = bitarrayAppendPackedNormalizedRight(arr, 3, 2, 8);
    status = bitarrayAppendPackedNormalizedRight(arr, 501, 9, 9);
    status = bitarrayAppendPackedNormalizedRight(arr, 502, 9, 9);
    status = bitarrayAppendPackedNormalizedRight(arr, 7, 3, 4);
    status = bitarrayAppendPackedNormalizedRight(arr, 1, 1, 3);
    status = bitarrayAppendPackedNormalizedRight(arr, 4001, 12, 13); //1111 1010 0001
    status = bitarrayAppendPackedNormalizedRight(arr, 1, 1, 5);
    status = bitarrayAppendPackedNormalizedRight(arr, 5, 3, 3);
} */