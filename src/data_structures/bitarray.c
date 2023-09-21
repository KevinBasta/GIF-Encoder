
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <stdint.h>
#include "main.h"

#include "bitarray.h"
#include "bitUtility.h"
#include "printUtility.h"

/**
 * @brief Insert the intervalIntervalInsertItem at markIndex,
 * append an entry for the next insert item, and update markIndex
 * @param arr   Bitarray struct
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE setMarkValueAndMarkNewLocation(bitarray *arr) { 
    STATUS_CODE status;

    BIT_ARRAY_NULL_CHECK(arr);

    if (arr->intervalInsertFlag == true) {
        //
        // NOTE: (arr->currentIndex - 1) because this function is
        // expected to be called after every current index increment
        //
        if ((arr->currentIndex - 1) % arr->intervalInsertBoundry == 0) {
            status = bitarraySetBookMarkValue(arr, arr->intervalInsertItem);
            CHECKSTATUS(status);
            
            status = bitarrayBookMark(arr, 0);
            CHECKSTATUS(status);
            status = bitarrayAppend(arr, 0);
            CHECKSTATUS(status);
        }
    }

    return OPERATION_SUCCESS;
}

/**
 * @brief Set the markIndex field of a bitarray to the 
 * bitarray's current index plus the offset
 * @param arr       Bitarray struct
 * @param offset    Offset from current index
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE bitarrayBookMark(bitarray *arr, u32 offset) {
    BIT_ARRAY_NULL_CHECK(arr);

    if (arr->currentIndex + offset < 0 || arr->currentIndex + offset > arr->size)
        return BIT_ARRAY_BOOKMARK_FAILED;

    arr->markIndex = arr->currentIndex + offset;

    return OPERATION_SUCCESS;
}

/**
 * @brief Set the value of the currently bookmarked item
 * @param arr   Bitarray struct
 * @param item  Item to put at current markIndex
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE bitarraySetBookMarkValue(bitarray *arr, u8 item) {
    BIT_ARRAY_NULL_CHECK(arr);

    if (arr->markIndex != -1) {
        arr->items[arr->markIndex] = item;
        printf("%d %d\n", arr->items[arr->markIndex], item);
    }

    return OPERATION_SUCCESS;
}

/**
 * @brief Set a rule to insert an item at a set interval
 * @param arr           Bitarray struct
 * @param boundry       Interval to append the item
 * @param insertItem    Item to append at interval
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE bitarraySetIntervalInsertRule(bitarray *arr, size_t boundry, u8 insertItem) {
    BIT_ARRAY_NULL_CHECK(arr);
    
    arr->intervalInsertFlag    = true;
    arr->intervalInsertBoundry = boundry;
    arr->intervalInsertItem    = insertItem;

    return OPERATION_SUCCESS;
}



/**
 * @brief Set the remaining bits to 0 in the pointer
 * @param item          u8 integer to modify
 * @param currentBit    Bit to start zeroing from 
 */
static void zeroRestOfByte(u8 *item, u8 currentBit) {
    for (u8 i = currentBit; i < 8; i++) {
        *item &= ~(1 << i);
    }
}

/**
 * @brief Decide a new size for an array in the context of realloc 
 * @param arr Bitarray to return new size for
 * @return New size (number of entries) for the array
 */
static size_t bitarrayNewSize(bitarray *arr) {
    return arr->size + (arr->size / 2);
}

/**
 * @brief Expand a bitarray by using the realloc stdlib function
 * @param arr       Bitarray to expand
 * @param newSize   The new size (number of entries) to expand to
 * @return OPERATION_SUCCESS or error code
 */
static STATUS_CODE bitarrayRealloc(bitarray *arr, size_t newTotalEntries) {
    u8 *status = realloc(arr->items, newTotalEntries * sizeof(u8));
    
    if (status == NULL) {
        freeBitArray(arr);
        return BIT_ARRAY_REALLOC_FAILED;
    }
    
    arr->items = status;
    arr->size  = newTotalEntries;

    return OPERATION_SUCCESS;
}

/**
 * @brief Increment CurrentIndex, set currentBit to 0, set update bookmark if needed
 * @param arr The bitarray to perform on
 * @return OPERATION_SUCCESS or error code
 */
static STATUS_CODE incrementCurrentIndex(bitarray *arr) {
    STATUS_CODE status;

    if (arr->currentIndex + 1 >= arr->size) {
        status = bitarrayRealloc(arr, bitarrayNewSize(arr));
        CHECKSTATUS(status);
    }
    
    arr->currentIndex++;
    arr->currentBit = 0;
    status = setMarkValueAndMarkNewLocation(arr);

    return status;
}



/**
 * @brief Init a bit array struct
 * @param size Total entries to initally allocate space for
 * @return Bitarray pointer or NULL
 */
bitarray *bitarrayInit(size_t size) {
    bitarray *arr = calloc(1, sizeof(bitarray));
    if (arr == NULL)
        return NULL;

    arr->items = calloc(size, sizeof(u8));
    if (arr->items == NULL) {
        free(arr);
        return NULL;
    }

    arr->size = size;
    arr->currentIndex = 0;
    arr->currentBit   = 0;
    
    arr->intervalInsertFlag = false;
    arr->intervalInsertBoundry = 0;
    arr->intervalInsertItem    = 0;
    arr->markIndex             = -1;
    
    return arr;
}

/**
 * @brief Append a u8 item to a bitarray
 * @param arr   Array in question
 * @param item  Item to append
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE bitarrayAppend(bitarray *arr, u8 item) {
    STATUS_CODE status;
    
    BIT_ARRAY_NULL_CHECK(arr);
    
    if (arr->currentBit != 0) {
        zeroRestOfByte(&(arr->items[arr->currentIndex]), arr->currentBit);
        status = incrementCurrentIndex(arr);
        CHECKSTATUS(status);
    }

    arr->items[arr->currentIndex] = item;
    status = incrementCurrentIndex(arr);
    CHECKSTATUS(status);
    
    return OPERATION_SUCCESS;
}

/**
 * @brief Append the least amount of bits needed to express item or minNumberOfBits
 * packing to the left (least significant bit) of each byte
 * @param arr               Bitarray to append to
 * @param item              Item to append
 * @param minNumberOfBits   The minimum number of bits this item should take up
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE bitarrayAppendPackedLeft(bitarray *arr, u32 item, u32 minNumberOfBits) {
    STATUS_CODE status;
    BIT_ARRAY_NULL_CHECK(arr);

    u32 occupiedBits = getOccupiedBits(item);
    if (minNumberOfBits < occupiedBits) {
        return BIT_ARRAY_PACK_OCCUPIED_GREATER_THAN_MIN;
    }

    u32 remainingBitsInCurrentArrayByte = 8 - arr->currentBit;

    if (remainingBitsInCurrentArrayByte >= minNumberOfBits) {
        u8 currentItem = arr->items[arr->currentIndex];

        arr->items[arr->currentIndex] = currentItem | (item << (remainingBitsInCurrentArrayByte - minNumberOfBits));
        arr->currentBit += minNumberOfBits;
        
        if (arr->currentBit == 8) {
            status = incrementCurrentIndex(arr);
            CHECKSTATUS(status);
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
                status = incrementCurrentIndex(arr);
                CHECKSTATUS(status);
            }

            remainingBitsInCurrentArrayByte = 8 - arr->currentBit;
        }
    }

    return OPERATION_SUCCESS;
}

/**
 * @brief Append the least amount of bits needed to express item or minNumberOfBits
 * packing to the right (most significant bit) of each byte
 * @param arr               Bitarray to append to
 * @param item              Item to append
 * @param minNumberOfBits   The minimum number of bits this item should take up
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE bitarrayAppendPackedRight(bitarray *arr, u32 item, u32 minNumberOfBits) {
    STATUS_CODE status;
    
    BIT_ARRAY_NULL_CHECK(arr);

    u32 occupiedBits = getOccupiedBits(item);
    if (minNumberOfBits < occupiedBits) {
        return BIT_ARRAY_PACK_OCCUPIED_GREATER_THAN_MIN;
    }

    u32 remainingBitsInCurrentArrayByte = 8 - arr->currentBit;

    if (remainingBitsInCurrentArrayByte >= minNumberOfBits) {
        u8 currentItem = arr->items[arr->currentIndex];

        arr->items[arr->currentIndex] = currentItem | (item << (arr->currentBit));
        arr->currentBit += minNumberOfBits;
        
        if (arr->currentBit == 8) {
            status = incrementCurrentIndex(arr);
            CHECKSTATUS(status);
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
                status = incrementCurrentIndex(arr);
                CHECKSTATUS(status);
            }

            remainingBitsInCurrentArrayByte = 8 - arr->currentBit;
        }
    }

    return OPERATION_SUCCESS;
}

// Print bitarray up to last occupied byte
void bitarrayPrint(bitarray *arr) {
    size_t index = 0;

    for (size_t i = 0; i < arr->currentIndex + 1; i++) {
        printf("%X ", arr->items[i]);
        index++;
        if (index % 24 == 0) {
            printf("\n");
        }
    }

    printf("\n");
}

// Free the struct and its pointer fields
void freeBitArray(bitarray *arr) {
    if (arr != NULL) {
        if (arr->items != NULL)
            free(arr->items);

        free(arr);
    }
}


/* int main() {
    STATUS_CODE status;
    
    bitarray *arr = bitarrayInit(100);
    status = bitarrayAppendPackedRight(arr, 4, 3, 4);
    status = bitarrayAppendPackedRight(arr, 3, 2, 8);
    status = bitarrayAppendPackedRight(arr, 501, 9, 9);
    status = bitarrayAppendPackedRight(arr, 502, 9, 9);
    status = bitarrayAppendPackedRight(arr, 7, 3, 4);
    status = bitarrayAppendPackedRight(arr, 1, 1, 3);
    status = bitarrayAppendPackedRight(arr, 4001, 12, 13); //1111 1010 0001
    status = bitarrayAppendPackedRight(arr, 1, 1, 5);
    status = bitarrayAppendPackedRight(arr, 5, 3, 3);
} */