
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <stdint.h>
#include "main.h"

#include "endianUtility.h"
#include "printUtility.h"

// endian check
const int endiannessCheck = 1;
#define bigEndianPlatform() ( (*(char *) &endiannessCheck) == 0 )

/**
 * @brief u64val is where the converted int is writte.
 * all types can be read for type conversion.
 */
union endianSwappedInt { 
    u8  u8val;
    u16 u16val;
    u32 u32val;
    u64 u64val;

    i8  i8val;
    i16 i16val;
    i32 i32val;
    i64 i64val;
};

//////////////////// Convert value to Little Endian ////////////////////

/**
 * @note FOR USE BY THE GIF ENCODER ONLY.
 * @brief This function and it's associated littleEndianUX interface
 * is for use in soley ensuring the converstion of the program values
 * in platform endian to little endian values for the GIF encoding process.
 * @param arr               - integer type cast to u8* by interface
 * @param numberOfBytes     - number of bytes to work with
 * @return a union value to allow for multiple type return from interface
 */
static union endianSwappedInt convertToLittleEndian(u8 *arr, u32 numberOfBytes) { 
    union endianSwappedInt intUnion;
    intUnion.u64val = 0;
    char *p = (char *)&intUnion;

    if (bigEndianPlatform()) {
        for (int i = 0; i < numberOfBytes; i++)
            p[i] = arr[numberOfBytes - i - 1];
    } else {
        for (int i = 0; i < numberOfBytes; i++)
            p[i] = arr[i];
    }

    return intUnion;
}

// u8-u64 stack ints interface
u8 littleEndianU8(u8 val) { 
    return convertToLittleEndian((char*) &val, sizeof(u8)).u8val;
}

u16 littleEndianU16(u16 val) { 
    return convertToLittleEndian((char*) &val, sizeof(u16)).u16val;
}

u32 littleEndianU32(u32 val) { 
    return convertToLittleEndian((char*) &val, sizeof(u32)).u32val;
}

u64 littleEndianU64(u64 val) { 
    return convertToLittleEndian((char*) &val, sizeof(u64)).u64val;
}

// i8-i64 stack ints interface
i8 littleEndianI8(i8 val) { 
    return convertToLittleEndian((char*) &val, sizeof(i8)).i8val;
}

i16 littleEndianI16(i16 val) { 
    return convertToLittleEndian((char*) &val, sizeof(i16)).i16val;
}

i32 littleEndianI32(i32 val) { 
    return convertToLittleEndian((char*) &val, sizeof(i32)).i32val;
}

i64 littleEndianI64(i64 val) { 
    return convertToLittleEndian((char*) &val, sizeof(i64)).i64val;
}


/**
 *  @brief converts a big endian integer bit pattern
 *  stored in a char array to little endian integer bit pattern stored
 *  in the spesified type. 
 *  integer values are stored as big endian in the MPEG-4 Files.
 *  they are then read into a char heap array which is given 
 *  to this function. they are then converted into an integer and returned. 
 *  @param *bigEndianCharArray  - the 4 byte character array
 *  @param numberOfBytes        - number of bytes the convert from char array
 *  @return u32 value
 */
static union endianSwappedInt bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(u8 *bigEndianCharArray, u32 numberOfBytes) { 
    union endianSwappedInt intUnion;
    intUnion.u64val = 0;
    u32 byteNumb = numberOfBytes - 1;

    for (u32 headerByte = 0; headerByte < numberOfBytes; headerByte++) {
        for (u32 bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            /*
                currentBit (integerAsCharArray[headerByte] >> bitInHeaderByte) & 1
                shifts the desired bit to the lsb and masks it with the binary
                representation of 1

                bitOffset (((3-headerByte)*8) + bitInHeaderByte) maps the position
                of a bit in a byte to it's position in a 4 byte binary representation

                if the currentBit is 1, then will set that bit in the integer number
            */
            i32 currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            i32 bitOffset = (((byteNumb-headerByte)*8) + bitInHeaderByte);
            // DEBUG PRINTF("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                intUnion.u64val = intUnion.u64val | (currentBit << bitOffset);
            }
        }
    }

    return intUnion;
}
