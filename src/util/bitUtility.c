#ifndef MPEG_HEAD
    #define MPEG_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <math.h>

    #include "../headers/types.h"
    #include "../headers/printUtility.h"
    #include "../headers/endianUtility.h"
    #include "../headers/bitUtility.h"
#endif






/**
 * @brief Compares n bytes of u8 types. Intended for comparing the 
 * types of MPEG boxes.
 * @param *firstItem    -   n byte u8 array pointer
 * @param secondItem    -   m byte u8 array (includes '\0')
 * @param numberOfBytes -   the number of bytes to compare
 * @return TRUE or FALSE
 */
i32 compareNBytes(u8 *firstItem, u8 secondItem[], u32 numberOfBytes) {     
    i32 isEqual = TRUE;
    for (u32 i = 0; i < numberOfBytes; i++) {
        if (firstItem[i] != secondItem[i]) {
            isEqual = FALSE;
            break;
        }
    }

    return isEqual;
}


/**
 * @brief creates a new array containing n bytes of data from the originalData array 
 * @param numberOfBytes     -   amount of bytes to allocate for returned array
 * @param *originalData     -   pointer to an array
 * @param *byteOffset       -   the current array index of originalData
 * @return a new char array of size numberOfBytes that contains data from
 * originalData
 *
 * @note redundand memory usage if parent array persists.
 * @note it might be benifitial to add a terminating char.
 */
u8 *copyNBytes(u32 numberOfBytes, u8 *originalData, u32 *byteOffset) {
    u8 *infoCopy = (u8*) malloc(numberOfBytes);
    for (u32 i = 0; i < numberOfBytes; i++) {
        infoCopy[i] = originalData[*byteOffset];
        *byteOffset += 1;
    }

    return infoCopy;
}


/**
 * @brief returns a char pointer from an array and increments byteOffset by the 
 * size of the data that the char pointer refers to
 * @param numberOfBytes     -   amount of bytes to add to byteOffset
 * @param *originalData     -   pointer to an array
 * @param *byteOffset       -   the current array index of originalData
 * @return a pointer to the originalData at offset byteOffset
 *
 * @note if parent array is freed data will be lost.
 * @note will need to manually determine where each reference array ends.
 */
u8 *referenceNBytes(u32 numberOfBytes, u8 *originalData, u32 *byteOffset) {
    u8 *infoReference = &(originalData[*byteOffset]);
    *byteOffset += numberOfBytes;

    // checking if referenced properly
    /* for (u32 i = 0; i < numberOfBytes; i++) { 
        assert(&(infoReference[i]) == &(originalData[(*byteOffset) - numberOfBytes + i]));
        assert(infoReference[i] == originalData[(*byteOffset) - numberOfBytes + i]);
    } */

    return infoReference;
}


u8 *referenceNBits(u32 numberOfBits, u8 *originalData, u32 *bitOffset) {
    u8 *infoReference = &(originalData[(u32) floor( *bitOffset / 8.0 )]);
    *bitOffset += numberOfBits;

    // checking if referenced properly
    /* for (u32 i = 0; i < numberOfBytes; i++) { 
        assert(&(infoReference[i]) == &(originalData[(*byteOffset) - numberOfBytes + i]));
        assert(infoReference[i] == originalData[(*byteOffset) - numberOfBytes + i]);
    } */

    return infoReference;
}

/**
 * @brief same as referenceNBytes but does not increment byteOffset
 * @param numberOfBytes 
 * @param originalData 
 * @param byteOffset 
 * @return 
 */
u8 *checkNextNBytes(u32 numberOfBytes, u8 *originalData, u32 byteOffset) {
    u8 *infoReference = &(originalData[byteOffset]);

    return infoReference;
}


u32 countBitsToFirstNonZero(u8 *originalData, u32 boundry) { 
    u32 leadingZeroBits = -1;

    u32 byteOffset = 0;
    u32 bitOffset;

    while( 1 == 1 ) {
        bitOffset = 1;
        for (u32 j = 0; j < 8; j++) {
    
            if (getNBits(bitOffset, bitOffset, originalData[byteOffset]) != 0) { 
                return (leadingZeroBits + 1);
            } else {
                leadingZeroBits++;
            }            
    
           bitOffset++;
        }
        byteOffset++;
    }

    return 0;
}


/**
 * @brief get a range of bits shifted to the least segnificant bit. bits numbered 1-8
 * @param startBit the starting bit in the range of 1-8 to include in final u8 bit pattern
 * @param endBit the last bit in the range of 1-8 to include in the final u8 bit pattern.
 * this is the least segnificant bit
 * @param data the data to get the bits from
 * @return u8 with the bit range in the lsb
 */
u8 getNBits(u32 startBit, u32 endBit, u8 data) { 
    if (startBit < 1 || startBit > 8 || endBit > 8 || endBit < 1 || endBit < startBit) { 
        return 0;
    }

    u8 leftShift = startBit - 1;
    u8 rightShift = (startBit - 1) + (8 - endBit);

    return (u8) (data << leftShift) >> rightShift;
    // or mask the range startBit to endBit and then shift right
}
