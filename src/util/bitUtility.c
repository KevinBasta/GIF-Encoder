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




// The following functions are for AVC and NAL decoding

/**
 * @brief same as referenceNBytes but does not increment byteOffset. 
 * for checking emulation prevention three byte at the end of a NAL unit.
 * this function is for abstracting the line &(originalData[byteOffset]).
 * @param numberOfBytes     - number of bytes to check at current offset
 * @param originalData      - array to return pointer to
 * @param byteOffset        - offset in array
 * @return pointer to array at current byte offset
 */
u8 *checkNextNBytes(u32 numberOfBytes, u8 *originalData, u32 byteOffset) {
    u8 *infoReference = &(originalData[byteOffset]);

    return infoReference;
}


/**
 * @brief gives a pointer to the byte that contains the starting bit
 * @param numberOfBits  - for incrementing offset
 * @param originalData  - for returning pointer
 * @param bitOffset     - for calculating current byte offset
 * @return array byte pointer containing starting bit
 */
u8 *referenceNBits(u32 numberOfBits, u8 *originalData, u32 *bitOffset, u32 *byteOffset) {
    u8 *infoReference = &(originalData[(u32) floor( *bitOffset / 8.0 )]);
    *bitOffset += numberOfBits;
    *byteOffset += (u32) floor(numberOfBits / 8.0);
    
    // checking if referenced properly
    /* for (u32 i = 0; i < numberOfBytes; i++) { 
        assert(&(infoReference[i]) == &(originalData[(*byteOffset) - numberOfBytes + i]));
        assert(infoReference[i] == originalData[(*byteOffset) - numberOfBytes + i]);
    } */

    return infoReference;
}

/**
 * @brief count zero bits + first non-zero bit from startingBitInPointer
 * @param originalData          - array pointer
 * @param startingBitInPointer  - 0-7 
 * @param byteBoundary          - last array byte
 * @return number of 0 bits
 */
u32 countBitsToFirstNonZero(u8 *originalData, u32 *bitOffset, u32 *byteOffset, u32 byteBoundaryPlusOne) { 
    u8 bitStart;

    u32 startingBitInPointer = (*bitOffset % 8);
    if (startingBitInPointer < 0 || startingBitInPointer > 7) { 
        bitStart = 0;
    } else { 
        bitStart = startingBitInPointer;
    }

    u32 leadingZeroBits = 0;

    while (TRUE) {
        for (u32 j = bitStart; j < 8; j++) {
            *bitOffset += 1;
            if (getNthBit(j, originalData[*byteOffset]) != 0) { 
                return leadingZeroBits;
            } else {
                leadingZeroBits++;
            }
        }
        
        bitStart = 0;

        if (*byteOffset > byteBoundaryPlusOne - 1) { 
            return 0;
        } else {
            *byteOffset += 1;
        }
    }

    return 0;
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