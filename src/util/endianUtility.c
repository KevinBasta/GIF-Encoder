#ifndef MPEG_HEAD
    #define MPEG_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <math.h>

    #include "types.h"
    #include "printUtility.h"
    #include "endianUtility.h"
    #include "bitUtility.h"
#endif


/**
 *  @brief converts a big endian integer bit pattern
 *  stored in a char array to little endian integer bit pattern stored
 *  in the spesified type. 
 *  integer values are stored as big endian in the MPEG-4 Files.
 *  they are then read into a char heap array which is given 
 *  to this function. they are then converted into an integer and returned. 
 *  @param *bigEndianCharArray:     the 4 byte character array
 *  @return u32 value
 */
u32 bigEndianCharToLittleEndianUnsignedInt(u8 *bigEndianCharArray) { 
    u32 littleEndianUnsignedInt = 0;

    for (u32 headerByte = 0; headerByte < 4; headerByte++) {
        for (u32 bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            i32 currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            i32 bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                littleEndianUnsignedInt = littleEndianUnsignedInt | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianUnsignedInt;
}

u32 *bigEndianCharToLittleEndianUnsignedIntHeap(u8 *bigEndianCharArray) { 
    u32 *littleEndianUnsignedInt = (u32*) malloc(sizeof(u32));
    *littleEndianUnsignedInt = bigEndianCharToLittleEndianUnsignedInt(bigEndianCharArray);

    return littleEndianUnsignedInt;
}

i32 bigEndianCharToLittleEndianInt(u8 *bigEndianCharArray) { 
    u32 littleEndianInt = 0;

    for (u32 headerByte = 0; headerByte < 4; headerByte++) {
        for (u32 bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            i32 currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            i32 bitOffset = (((3-headerByte)*8) + bitInHeaderByte);

            if (currentBit == 1) {
                littleEndianInt = littleEndianInt | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianInt;
}

i32 *bigEndianCharToLittleEndianIntHeap(u8 *bigEndianCharArray) { 
    i32 *littleEndianInt = (i32*) malloc(sizeof(i32));
    *littleEndianInt = bigEndianCharToLittleEndianInt(bigEndianCharArray);

    return littleEndianInt;
}

i32 bigEndianCharToLittleEndianGeneralized(u8 *bigEndianCharArray, u32 numberOfBytes) { 
    i32 littleEndianInt = 0;
    u32 byteNumb = numberOfBytes - 1;

    for (u32 headerByte = 0; headerByte < numberOfBytes; headerByte++) {
        for (u32 bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            i32 currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            i32 bitOffset = (((byteNumb-headerByte)*8) + bitInHeaderByte);

            if (currentBit == 1) {
                littleEndianInt = littleEndianInt | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianInt;
}

i32 *bigEndianCharToLittleEndianGeneralizedHeap(u8 *bigEndianCharArray, u32 numberOfBytes) { 
    i32 *littleEndianInt = (u32*) malloc(sizeof(u32));
    *littleEndianInt = bigEndianCharToLittleEndianGeneralized(bigEndianCharArray, numberOfBytes);

    return (void*) littleEndianInt;
}


u64 bigEndianCharBitsToLittleEndianGeneralized(u8 *bigEndianCharArray, u32 startingBit, u32 numberOfBits) {
    u64 littleEndianInt = 0;
    u32 bitToStartAtInFirstByte = (startingBit % 8);

    u32 middleBytes = floor(numberOfBits / 8.0);
    u32 preAndPostBits = numberOfBits - (middleBytes * 8);    
    i32 byteCountHelper = preAndPostBits - (7 - bitToStartAtInFirstByte); // -2 abs to make it lastBit?

    u32 numberOfBytes = 0;
    if (byteCountHelper <= 0) { 
        numberOfBytes = middleBytes + 1;
    } else if (byteCountHelper > 0) { 
        numberOfBytes = middleBytes + 2;
    }

    u32 numberOfBytesMinusOne = numberOfBytes - 1;

    u32 firstBit = 0;
    u32 lastBit = 7;

    u32 bitOffsetAdjust = (((numberOfBits - (8 - (startingBit) % 8) - ((numberOfBytes - 2) * 8))) % 8) - 1;
    u32 bitsTaken = numberOfBits;

    for (u32 headerByte = 0; headerByte < numberOfBytes; headerByte++) {
        firstBit = 0;
        lastBit = 7;
        
        if (headerByte == 0) { 
            firstBit = startingBit % 8;
        }

        if (headerByte == numberOfBytesMinusOne) { 
            lastBit = abs((((numberOfBits - (8 - (startingBit) % 8) - ((numberOfBytes - 2) * 8))) % 8) - 1);
        }

        for (u32 bitInHeaderByte = firstBit; bitInHeaderByte <= lastBit; bitInHeaderByte++) {
            i32 currentBit = (bigEndianCharArray[headerByte] >> (8 - bitInHeaderByte - 1)) & 1;

            i32 bitOffset = 0; 
            if (headerByte == 0) {
                bitOffset = (8 - (bitInHeaderByte)) + bitOffsetAdjust;
            } else {
                bitOffset = abs(((numberOfBytesMinusOne-headerByte)*8) + (bitInHeaderByte) - abs(bitOffsetAdjust));
            }

            if (currentBit == 1) {
                littleEndianInt = littleEndianInt | (currentBit << bitOffset);
            }
        }
    }
    printIntBits(&littleEndianInt, 8);
    printf("%lx\n", littleEndianInt);
    return littleEndianInt;
}


/**
 * @brief takes in starting bit and ending bit as total offset in a bytestream, creates an integer
 * with that bit range
 * @param bigEndianCharArray    - bytestream where data to get int from is stored
 * @param startingBit           - a total offset in the bytestream
 * @param endingBit             - a total offset in the bytestream
 * @param numberOfBits          - amount of bits between the start and end // can get rid of
 * @return u64 parsed int
 */
u64 simpleBigEndianToLittleEndianBits(u8 *bigEndianCharArray, u32 startingBit, u32 endingBit, u32 numberOfBits) {
    if (endingBit < startingBit) { 
        return 0;
    }
    
    //printBits(bigEndianCharArray, 8);
    u32 bitToStartAtInFirstByte = (startingBit % 8);
    u32 bitToEndAtInLastByte = (endingBit % 8);
    //printf("start %d end %d total %d\n", startingBit, endingBit, numberOfBits);

    i32 middleBytesHelper = (numberOfBits - bitToStartAtInFirstByte - bitToEndAtInLastByte - 1);
    u32 middleBytes;
    if (middleBytesHelper < 0) { 
        middleBytes = 0;
    } else { 
        middleBytes = floor(abs(numberOfBits - bitToStartAtInFirstByte - bitToEndAtInLastByte -1) / 8.0);
    }

    u32 preAndPostBits = numberOfBits - (middleBytes * 8);
    u32 postBits = bitToEndAtInLastByte + 1;
    u32 inversePostBits = 8 - postBits;

    //printf("real %d fake %d\n", bitToEndAtInLastByte, postBits);
    //printf("middleBytes %d\n", middleBytes);
    //printf("preAndPostBits %d\n", preAndPostBits);
    
    u32 numberOfBytes = middleBytes;

    if (&(bigEndianCharArray[(u32) floor(startingBit / 8.0)]) == &(bigEndianCharArray[(u32) floor(endingBit / 8.0)])) { 
        numberOfBytes += 1;
    } else { 
        numberOfBytes += 1; // for last byte    
        if (startingBit % 8 != 0) { // because would be included in middle bytes?
            numberOfBytes += 1;    
        }
    }

    
    //printf("%x %x\n", &(bigEndianCharArray[(u32) floor(startingBit / 8.0)]), &(bigEndianCharArray[(u32) floor(endingBit / 8.0)]));
    
    // This block has not been verified 100%
    /* u32 preBits = 8 - abs(preAndPostBits - postBits); // inaccurate def
    if (preBits != bitToStartAtInFirstByte) { 
        middleBytes = floor(abs(numberOfBits - 8) / 8.0);
        preAndPostBits = numberOfBits - (middleBytes * 8);
        postBits = abs(preAndPostBits - (8 - bitToStartAtInFirstByte));
        inversePostBits = 8 - postBits;
    } */

    /* printf("middleBytes %d\n", middleBytes);
    printf("bitToStartAtInFirstByte %d\n", bitToStartAtInFirstByte);
    printf("bitToEndAtInLastByte %d\n", bitToEndAtInLastByte);
    printf("post %d\n", postBits);
    printf("preAndPostBits %d\n", preAndPostBits);
    printf("bytes: %d\n", numberOfBytes); */

    u64 out = (u64) bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianCharArray, numberOfBytes).u64val;
    u32 leftShift = ((8 - numberOfBytes) * 8) + bitToStartAtInFirstByte;
    
    u32 rightShift = 0;
    if (postBits == 0) { 
        rightShift = leftShift;
    } else { 
        rightShift = leftShift + inversePostBits;
    }

    //printBits(bigEndianCharArray, 8);
    //printIntBits(&out, 8);
    out = (out << leftShift);
    //printf("left shift %d\n", leftShift);
    //printIntBits(&out, 8);
    out = (out >> rightShift);
    //printf("right shift %d\n", rightShift);
    //printIntBits(&out, 8);
    //printf("\n\n\n");
    return out;
}


union littleEndianInteger bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(u8 *bigEndianCharArray, u32 numberOfBytes) { 
    union littleEndianInteger intUnion;
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
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                intUnion.u64val = intUnion.u64val | (currentBit << bitOffset);
            }
        }
    }

    return intUnion;
}


// u8-u64 stack ints
u8 bigEndianU8ArrToLittleEndianU8(u8 *bigEndianU8Arr) { 
    return bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(u8)).u8val;
}

u16 bigEndianU8ArrToLittleEndianU16(u8 *bigEndianU8Arr) { 
    return bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(u16)).u16val;
}

u32 bigEndianU8ArrToLittleEndianU32(u8 *bigEndianU8Arr) { 
    return bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(u32)).u32val;
}

u64 bigEndianU8ArrToLittleEndianU64(u8 *bigEndianU8Arr) { 
    return bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(u64)).u64val;
}



// u8-u64 heap ints
u8 *bigEndianU8ArrToLittleEndianU8Heap(u8 *bigEndianU8Arr) { 
    u8 *result = malloc(sizeof(u8));
    *result = bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(u8)).u8val;
    return result;
}

u16 *bigEndianU8ArrToLittleEndianU16Heap(u8 *bigEndianU8Arr) { 
    u16 *result = malloc(sizeof(u16));
    *result = bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(u16)).u16val;
    return result;
}

u32 *bigEndianU8ArrToLittleEndianU32Heap(u8 *bigEndianU8Arr) { 
    u32 *result = malloc(sizeof(u32));
    *result = bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(u32)).u32val;
    return result;
}

u64 *bigEndianU8ArrToLittleEndianU64Heap(u8 *bigEndianU8Arr) { 
    u64 *result = malloc(sizeof(u64));
    *result = bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(u64)).u64val;
    return result;
}



// i8-i64 stack ints
i8 bigEndianU8ArrToLittleEndianI8(u8 *bigEndianU8Arr) { 
    return bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(i8)).i8val;
}

i16 bigEndianU8ArrToLittleEndianI16(u8 *bigEndianU8Arr) { 
    return bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(i16)).i16val;
}

i32 bigEndianU8ArrToLittleEndianI32(u8 *bigEndianU8Arr) { 
    return bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(i32)).i32val;
}

i64 bigEndianU8ArrToLittleEndianI64(u8 *bigEndianU8Arr) { 
    return bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(i64)).i64val;
}



// i8-i64 heap ints
i8 *bigEndianU8ArrToLittleEndianI8Heap(u8 *bigEndianU8Arr) { 
    i8 *result = malloc(sizeof(i8));
    *result = bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(i8)).i8val;
    return result;
}

i16 *bigEndianU8ArrToLittleEndianI16Heap(u8 *bigEndianU8Arr) { 
    i16 *result = malloc(sizeof(i16));
    *result = bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(i16)).i16val;
    return result;
}

i32 *bigEndianU8ArrToLittleEndianI32Heap(u8 *bigEndianU8Arr) { 
    i32 *result = malloc(sizeof(i32));
    *result = bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(i32)).i32val;
    return result;
}

i64 *bigEndianU8ArrToLittleEndianI64Heap(u8 *bigEndianU8Arr) { 
    i64 *result = malloc(sizeof(i64));
    *result = bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(bigEndianU8Arr, sizeof(i64)).i64val;
    return result;
}


///////////////////////////////// DELETE THE FOLLOWING //////////////////////////////
i32 *bigEndianCharToLittleEndianBytedInt(u8 *bigEndianCharArray, i32 numberOfBytes) { 
    i32 *littleEndianInt = (i32*) malloc(sizeof(i32));

    for (u32 headerByte = 0; headerByte < numberOfBytes; headerByte++) {
        for (u32 bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            i32 currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            i32 bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                *littleEndianInt = *littleEndianInt | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianInt;
}

union intToFloat
{
    unsigned int u32; // here_write_bits
    float    f32; // here_read_float
};

float *bigEndianCharToLittleEndianFloat(u8 *bigEndianCharArray) { 
    u32 littleEndianInt = 0;

    for (u32 headerByte = 0; headerByte < 4; headerByte++) {
        for (u32 bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            i32 currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            i32 bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                littleEndianInt = littleEndianInt | (currentBit << bitOffset);
            }
        }
    }

    float *littleEndianFloat = (float*) malloc(sizeof(float));
    union intToFloat x = {.u32 = littleEndianInt};
    *littleEndianFloat = x.f32;

    printf("%d\n", x.u32);
    printf("%f\n", (float) x.f32);
    
    return littleEndianFloat;
}

////////////////////////////////////DELETE THE ABOVE//////////////////////////////////////////