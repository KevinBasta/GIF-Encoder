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


u32 bigEndianCharBitsToLittleEndianGeneralized(u8 *bigEndianCharArray, u32 startingBit, u32 numberOfBits) {
    //printBits(bigEndianCharArray, 4);

    u32 littleEndianInt = 0;
    u32 numberOfBytes;

    if (numberOfBits < 8) { 
        numberOfBytes = ceil( (numberOfBits) / 8.0 );
        if (startingBit % 8 > 0) { 
            numberOfBytes += 1;
            //printf("added\n");
        }
    } else { 
        numberOfBytes = ceil( (numberOfBits) / 8.0 );
    }


    u32 numberOfBytesMinusOne = numberOfBytes - 1;
    //printf("%d numb bytes\n", numberOfBytes);

    u32 firstBit = 0;
    u32 lastBit = 7;

    u32 bitOffsetAdjust = startingBit % 5;
    //printf("%d bit offset adjsut \n", bitOffsetAdjust);
    u32 bitsTaken = numberOfBits;

    for (u32 headerByte = 0; headerByte < numberOfBytes; headerByte++) {
        //printf("header byte: %d\n", headerByte);
        firstBit = 0;
        lastBit = 7;
        
        if (headerByte == 0) { 
            firstBit = startingBit % 8;
            //printf("%d start\n", firstBit);
        }

        if (headerByte == numberOfBytesMinusOne) { 
            lastBit = (((numberOfBits - (8 - (startingBit) % 8) - ((numberOfBytes - 2) * 8))) % 8) - 1;
            //printf("%d %d end\n", lastBit, firstBit);
        }

        for (u32 bitInHeaderByte = firstBit; bitInHeaderByte <= lastBit; bitInHeaderByte++) {
            //printIntBits(&littleEndianInt, 4);
            i32 currentBit = (bigEndianCharArray[headerByte] >> (8 - bitInHeaderByte - 1)) & 1;
            //printIntBits(&currentBit, 4);
            
            i32 bitOffset = 0; 

            if (headerByte == 0) {
                bitOffset = (8 - (bitInHeaderByte)) + bitOffsetAdjust;
            } else {
                bitOffset = (((numberOfBytesMinusOne-headerByte)*8) + (bitInHeaderByte) - bitOffsetAdjust);
            }
            //printf("%d offset \n", bitOffset);
            /**
                1 - 0 * 8 = 8
                + 7 = 7
                - 2
                = 18
            */
            if (currentBit == 1) {
                littleEndianInt = littleEndianInt | (currentBit << bitOffset);
            }

            //bitsTaken--;
        }
    }

    //printf("final\n");
    //printIntBits(&littleEndianInt, 4);
    return littleEndianInt;
}


/**
 * @brief u64val is where the littleEndian int is writte.
 * all types can be read for type conversion.
 */
union littleEndianInteger { 
    u8  u8val;
    u16 u16val;
    u32 u32val;
    u64 u64val;

    i8  i8val;
    i16 i16val;
    i32 i32val;
    i64 i64val;
} littleEndianInteger;

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