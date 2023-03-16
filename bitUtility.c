#ifndef COMMON_HEAD
    #define COMMON_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    #include "printUtility.h"
    #define BOX_HEADER_SIZE 8
    #define BOX_HEADER_HALF_SIZE 4
    #define TRUE 1
    #define FALSE 0
#endif

/**
 *  Converts a 4 byte char array's binary to an unsigned int
 *  @param *integerAsCharArray:     the 4 byte character array
 */
unsigned int *charToInt(char *integerAsCharArray) { 
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


/**
 *  Compares n bytes of char types. Intended for comparing the 
 *  types of mpeg boxes.
 *  @param *firstItem:    n byte char array pointer
 *  @param secondItem:  m byte char array (includes '\0')
 */
int compareNBytes(char *firstItem, char secondItem[], int numberOfBytes) {     
    int isEqual = 1;
    for (int i = 0; i < numberOfBytes; i++) {
        if (firstItem[i] != secondItem[i]) {
            isEqual = 0;
            break;
        }
    }

    return isEqual;
}


/**
 *  Returns a char pointer to a new array. This new array contains n bytes
 *  of data from the originalData array 
 *  @param numberOfBytes:   amount of bytes to allocate for returned array
 *  @param *originalData:   pointer to an array
 *  @param *byteOffset:     the current array index of originalData
 *  @attention redundand memory usage if parent array presists
 *  @attention if using it might be benifitial to add a terminating char
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
 *  Returns a char pointer from an array and increments byteOffset by the 
 *  size of the data that the char pointer refers to
 *  @param numberOfBytes:   amount of bytes to add to byteOffset
 *  @param *originalData:   pointer to an array
 *  @param *byteOffset:     the current array index of originalData
 *  @attention if parent array is freed data will be lost
 *  @attention will need to manually determine where each reference array ends
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