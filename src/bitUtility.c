#ifndef MPEG_HEAD
    #define MPEG_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>

    #include "headers/types.h"
    #include "headers/printUtility.h"
#endif



// the following conversion functions will be cleaned up when it is clearer what is needed. //
/**
 *  Converts a 4 byte char array's binary to an unsigned int
 *  
 *  Important notes and investigation:
 *  INACCURATE NAME, this is due to the fact that the integers
 *  are stored in the normal integer bitpattern in the file. Reading them
 *  as chars is just a convinience. The chars read are flipped because 
 *  the way the files are written is so that the integers fill up the top 
 *  bits first and leave the rest of their field space empty.
 *  It may be not the way the files are written, but simply the way they are
 *  read by parseMPEG-4. Both propositions turned out to be false. printBits
 *  was simply misleading in printing the bits backwards when char and forwards when int.
 *  this behavior has to do with how integers are stored as bits in memory
 *  After investigation, this is simply the bits being sotred as little-endian in memory
 *
 *  what this function really does is take a char array that has an integer
 *  in it's top bits and put those bits in the bottom bits of an int.
 * 
 *  chunksread = fread(&headerSize[3], 1, 1, video);
 *  chunksread = fread(&headerSize[2], 1, 1, video);
 *  chunksread = fread(&headerSize[1], 1, 1, video);
 *  chunksread = fread(&headerSize[0], 1, 1, video);
 *  something like this could read the bytes in the right order 
 *  intead of the one liner used in readMainBoxes, a modified version
 *  of this function would still be needed to just map the bits to 
 *  an u32
 *
 *  u32 *headerSize = (u32*) malloc(BOX_HEADER_HALF_SIZE);
 *  chunksread = fread(&headerSize[3], BOX_HEADER_HALF_SIZE, 1, video);
 *  chunksread = fread(&headerSize[2], BOX_HEADER_HALF_SIZE, 1, video);
 *  chunksread = fread(&headerSize[1], BOX_HEADER_HALF_SIZE, 1, video);
 *  chunksread = fread(&headerSize[0], BOX_HEADER_HALF_SIZE, 1, video);
 *  printBits(4, headerSize);
 *  Although the bit manipulation in this function is interesting, upon 
 *  further investigation, the integers stored in the file can 
 *  simply be read straight into an u32 as shown. A future refactor
 *  will be needed. Additionally, the current bit manipulation may be 
 *  unecessary anyways if the bytes of char and int can simply be matched. 
 *
 *
 *  RESOLUTION:
 *  this fucntion basically converts a big endian integer bit pattern
 *  stored in a char array to little endian integer bit pattern stored
 *  in an unsiged integer. 
 *  integer values are stored as big endian in the MPEG-4 Files.
 *  they are then read into a char heap array which is given 
 *  to this function. they are then converted into an integer and returned. 
 *  A much simplier way to do the conversion is discussed above.
 *  @param *integerAsCharArray:     the 4 byte character array
 */
u32 *charToUnsignedInt(u8 *integerAsCharArray) { 
    u32 *fullBoxSize = (u32*) malloc(sizeof(u32));
    *fullBoxSize = 0;

    for (u32 headerByte = 0; headerByte < 4; headerByte++) {
        for (u32 bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            /*
                currentBit (integerAsCharArray[headerByte] >> bitInHeaderByte) & 1
                shifts the desired bit to the lsb and masks it with the binary
                representation of 1

                bitOffset (((3-headerByte)*8) + bitInHeaderByte) maps the position
                of a bit in a byte to it's position in a 4 byte binary representation

                if the currentBit is 1, then will set that bit in the integer number
            */
            i32 currentBit = (integerAsCharArray[headerByte] >> bitInHeaderByte) & 1;
            i32 bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                *fullBoxSize = *fullBoxSize | (currentBit << bitOffset);
            }
        }
    }

    return fullBoxSize;
}


i32 *charToInt(u8 *integerAsCharArray) { 
    i32 *fullBoxSize = (u32*) malloc(sizeof(u32));
    *fullBoxSize = 0;

    for (u32 headerByte = 0; headerByte < 4; headerByte++) {
        for (u32 bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            i32 currentBit = (integerAsCharArray[headerByte] >> bitInHeaderByte) & 1;
            i32 bitOffset = (((3-headerByte)*8) + bitInHeaderByte);

            if (currentBit == 1) {
                *fullBoxSize = *fullBoxSize | (currentBit << bitOffset);
            }
        }
    }

    return fullBoxSize;
}









i32 bigEndianCharToLittleEndianInt(u8 *bigEndianCharArray) { 
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

    return littleEndianInt;
}

i32 *bigEndianCharToLittleEndianIntHeap(u8 *bigEndianCharArray) { 
    i32 *littleEndianInt = (i32*) malloc(sizeof(i32));

    for (u32 headerByte = 0; headerByte < 4; headerByte++) {
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

    for (u32 headerByte = 0; headerByte < 4; headerByte++) {
        for (u32 bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            i32 currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            i32 bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                *littleEndianUnsignedInt = *littleEndianUnsignedInt | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianUnsignedInt;
}


i32 bigEndianCharToLittleEndianGeneralized(u8 *bigEndianCharArray, i32 numberOfBytes) { 
    i32 littleEndianInt = 0;

    for (u32 headerByte = 0; headerByte < numberOfBytes; headerByte++) {
        for (u32 bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            i32 currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            i32 bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                littleEndianInt = littleEndianInt | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianInt;
}


i32 *bigEndianCharToLittleEndianGeneralizedHeap(u8 *bigEndianCharArray, i32 numberOfBytes) { 
    i32 *littleEndianInt = (u32*) malloc(sizeof(u32));

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

    return (void*) littleEndianInt;
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

short *bigEndianCharToLittleEndianShort(u8 *bigEndianCharArray) { 
    short *littleEndianShort = (short*) malloc(sizeof(short));

    for (u32 headerByte = 0; headerByte < 2; headerByte++) {
        for (u32 bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            i32 currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            i32 bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                *littleEndianShort = *littleEndianShort | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianShort;
}

// for testing purposes
unsigned short *bigEndianCharToLittleEndianUnsignedShort(u8 *bigEndianCharArray) { 
    unsigned short *littleEndianUnsignedShort = (unsigned short*) malloc(sizeof(unsigned short));

    for (u32 headerByte = 0; headerByte < 2; headerByte++) {
        for (u32 bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            i32 currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            i32 bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                *littleEndianUnsignedShort = *littleEndianUnsignedShort | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianUnsignedShort;
}


/* u32 *generalizedCharToInt(u8 *integerAsCharArray) { 
    for reading more than 4 bytes for the purposes of longer boxes
 }*/
////////////////////////////////////DELETE THE ABOVE//////////////////////////////////////////




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