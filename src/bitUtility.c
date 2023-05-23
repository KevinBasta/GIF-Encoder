#ifndef MPEG_HEAD
    #define MPEG_HEAD
    #include <stdio.h>
    #include <stdlib.h>

    #include "headers/printUtility.h"
    #include "headers/types.h"
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
 *  an unsigned int
 *
 *  unsigned int *headerSize = (unsigned int*) malloc(BOX_HEADER_HALF_SIZE);
 *  chunksread = fread(&headerSize[3], BOX_HEADER_HALF_SIZE, 1, video);
 *  chunksread = fread(&headerSize[2], BOX_HEADER_HALF_SIZE, 1, video);
 *  chunksread = fread(&headerSize[1], BOX_HEADER_HALF_SIZE, 1, video);
 *  chunksread = fread(&headerSize[0], BOX_HEADER_HALF_SIZE, 1, video);
 *  printBits(4, headerSize);
 *  Although the bit manipulation in this function is interesting, upon 
 *  further investigation, the integers stored in the file can 
 *  simply be read straight into an unsigned int as shown. A future refactor
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
unsigned int *charToUnsignedInt(char *integerAsCharArray) { 
    unsigned int *fullBoxSize = (unsigned int*) malloc(sizeof(unsigned int));
    *fullBoxSize = 0;

    for (int headerByte = 0; headerByte < 4; headerByte++) {
        for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            /*
                currentBit (integerAsCharArray[headerByte] >> bitInHeaderByte) & 1
                shifts the desired bit to the lsb and masks it with the binary
                representation of 1

                bitOffset (((3-headerByte)*8) + bitInHeaderByte) maps the position
                of a bit in a byte to it's position in a 4 byte binary representation

                if the currentBit is 1, then will set that bit in the integer number
            */
            int currentBit = (integerAsCharArray[headerByte] >> bitInHeaderByte) & 1;
            int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                *fullBoxSize = *fullBoxSize | (currentBit << bitOffset);
            }
        }
    }

    return fullBoxSize;
}


int *charToInt(char *integerAsCharArray) { 
    int *fullBoxSize = (unsigned int*) malloc(sizeof(unsigned int));
    *fullBoxSize = 0;

    for (int headerByte = 0; headerByte < 4; headerByte++) {
        for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            int currentBit = (integerAsCharArray[headerByte] >> bitInHeaderByte) & 1;
            int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);

            if (currentBit == 1) {
                *fullBoxSize = *fullBoxSize | (currentBit << bitOffset);
            }
        }
    }

    return fullBoxSize;
}









int bigEndianCharToLittleEndianInt(char *bigEndianCharArray) { 
    unsigned int littleEndianInt = 0;

    for (int headerByte = 0; headerByte < 4; headerByte++) {
        for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            int currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                littleEndianInt = littleEndianInt | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianInt;
}

int *bigEndianCharToLittleEndianIntHeap(char *bigEndianCharArray) { 
    int *littleEndianInt = (int*) malloc(sizeof(int));

    for (int headerByte = 0; headerByte < 4; headerByte++) {
        for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            int currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                *littleEndianInt = *littleEndianInt | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianInt;
}




unsigned int bigEndianCharToLittleEndianUnsignedInt(char *bigEndianCharArray) { 
    unsigned int littleEndianUnsignedInt = 0;

    for (int headerByte = 0; headerByte < 4; headerByte++) {
        for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            int currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                littleEndianUnsignedInt = littleEndianUnsignedInt | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianUnsignedInt;
}

unsigned int *bigEndianCharToLittleEndianUnsignedIntHeap(char *bigEndianCharArray) { 
    unsigned int *littleEndianUnsignedInt = (unsigned int*) malloc(sizeof(unsigned int));

    for (int headerByte = 0; headerByte < 4; headerByte++) {
        for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            int currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                *littleEndianUnsignedInt = *littleEndianUnsignedInt | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianUnsignedInt;
}


int bigEndianCharToLittleEndianGeneralized(char *bigEndianCharArray, int numberOfBytes) { 
    int littleEndianInt = 0;

    for (int headerByte = 0; headerByte < numberOfBytes; headerByte++) {
        for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            int currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                littleEndianInt = littleEndianInt | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianInt;
}


int *bigEndianCharToLittleEndianGeneralizedHeap(char *bigEndianCharArray, int numberOfBytes) { 
    int *littleEndianInt = (unsigned int*) malloc(sizeof(unsigned int));

    for (int headerByte = 0; headerByte < numberOfBytes; headerByte++) {
        for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            int currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                *littleEndianInt = *littleEndianInt | (currentBit << bitOffset);
            }
        }
    }

    return (void*) littleEndianInt;
}


///////////////////////////////// DELETE THE FOLLOWING //////////////////////////////
int *bigEndianCharToLittleEndianBytedInt(char *bigEndianCharArray, int numberOfBytes) { 
    int *littleEndianInt = (int*) malloc(sizeof(int));

    for (int headerByte = 0; headerByte < numberOfBytes; headerByte++) {
        for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            int currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
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

float *bigEndianCharToLittleEndianFloat(char *bigEndianCharArray) { 
    unsigned int littleEndianInt = 0;

    for (int headerByte = 0; headerByte < 4; headerByte++) {
        for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            int currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
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

short *bigEndianCharToLittleEndianShort(char *bigEndianCharArray) { 
    short *littleEndianShort = (short*) malloc(sizeof(short));

    for (int headerByte = 0; headerByte < 2; headerByte++) {
        for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            int currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                *littleEndianShort = *littleEndianShort | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianShort;
}

// for testing purposes
unsigned short *bigEndianCharToLittleEndianUnsignedShort(char *bigEndianCharArray) { 
    unsigned short *littleEndianUnsignedShort = (unsigned short*) malloc(sizeof(unsigned short));

    for (int headerByte = 0; headerByte < 2; headerByte++) {
        for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            int currentBit = (bigEndianCharArray[headerByte] >> bitInHeaderByte) & 1;
            int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                *littleEndianUnsignedShort = *littleEndianUnsignedShort | (currentBit << bitOffset);
            }
        }
    }

    return littleEndianUnsignedShort;
}


/* unsigned int *generalizedCharToInt(char *integerAsCharArray) { 
    for reading more than 4 bytes for the purposes of longer boxes
 }*/
////////////////////////////////////DELETE THE ABOVE//////////////////////////////////////////




/**
 * @brief Compares n bytes of char types. Intended for comparing the 
 * types of MPEG boxes.
 * @param *firstItem    -   n byte char array pointer
 * @param secondItem    -   m byte char array (includes '\0')
 * @param numberOfBytes -   the number of bytes to compare
 * @return TRUE or FALSE
 */
int compareNBytes(char *firstItem, char secondItem[], int numberOfBytes) {     
    int isEqual = TRUE;
    for (int i = 0; i < numberOfBytes; i++) {
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
char *copyNBytes(int numberOfBytes, char *originalData, unsigned int *byteOffset) {
    char *infoCopy = (char*) malloc(numberOfBytes);
    for (int i = 0; i < numberOfBytes; i++) {
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
char *referenceNBytes(int numberOfBytes, char *originalData, unsigned int *byteOffset) {
    char *infoReference = &(originalData[*byteOffset]);
    *byteOffset += numberOfBytes;

    // checking if referenced properly
    /* for (int i = 0; i < numberOfBytes; i++) { 
        assert(&(infoReference[i]) == &(originalData[(*byteOffset) - numberOfBytes + i]));
        assert(infoReference[i] == originalData[(*byteOffset) - numberOfBytes + i]);
    } */

    return infoReference;
}