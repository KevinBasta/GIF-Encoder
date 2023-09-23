
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <stdint.h>
#include "main.h"

#include "bitUtility.h"

/**
 * @brief Compares n bytes of u8 types
 * @param *firstItem    -   n byte u8 array pointer
 * @param secondItem    -   m byte u8 array (includes '\0')
 * @param numberOfBytes -   the number of bytes to compare
 * @return true or false
 */
bool compareNBytes(u8 *firstItem, u8 secondItem[], u32 numberOfBytes) {     
    bool isEqual = true;
    for (u32 i = 0; i < numberOfBytes; i++) {
        if (firstItem[i] != secondItem[i]) {
            isEqual = false;
            break;
        }
    }

    return isEqual;
}

/**
 * @brief Compute the min number of bits it takes
 * (with no leading zeros) to represent the integer
 * @note For GIF flexible code sizes
 * @param item integer getting the bitsize of
 * @return number of bits the item occupies
 */
u32 getOccupiedBits(u32 item) {
    // count bits from left side to first non zero bit
    u32 leadingBits = 0;

    for (u32 i = 0; i < sizeof(item) * 8; i++) {
        bool isZero = (item & (1 << ((sizeof(item) * 8) - i - 1))) == 0;

        if (isZero == false) {
            break;
        } else {
            leadingBits++;
        }
    }

    u32 itemSize = (sizeof(item) * 8) - leadingBits;
    //PRINTF("leading bits: %d\n", leadingBits);
    //PRINTF("final size: %d\n", itemSize);
    
    return itemSize;
}

/**
 * @brief get a range of bits shifted to the least segnificant bit. bits numbered 0-7
 * @param startBit the starting bit in the range of 0-7 to include in final u8 bit pattern
 * @param endBit the last bit in the range of 0-7 to include in the final u8 bit pattern.
 * this is the least segnificant bit
 * @param data the data to get the bits from
 * @return u8 with the bit range in the lsb
 */
u8 getNBits(u32 startBit, u32 endBit, u8 data) { 
    if (startBit < 0 || startBit > 7 || endBit > 7 || endBit < 0 || endBit < startBit) { 
        return 0; // considar changing to -1. might end up with other bugs due to i8 in shifting
    }

    u8 leftShift = startBit;
    u8 rightShift = (startBit) + (7 - endBit);

    return (u8) (data << leftShift) >> rightShift;
    // or mask the range startBit to endBit and then shift right
}

u8 getNthBit(u32 bitNumber, u8 data) { 
    if (bitNumber < 0 || bitNumber > 7) { 
        return 0; 
    }

    u8 leftShift = bitNumber;
    u8 rightShift = (bitNumber) + (7 - bitNumber);

    return (u8) (data << leftShift) >> rightShift;
    // or mask the range startBit to endBit and then shift right
}
