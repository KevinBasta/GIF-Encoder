#ifndef COMMON_HEAD
    #define COMMON_HEAD
    #include <stdio.h>
    #include <stdlib.h>
#endif

/**
 *  prints n bytes from a char array.
 *  mainly used to print the 4 bytes of a box type since '\0' is not stored
 *  @param *string:         a pointer to first element in a character array
 *  @param bytesToPrint:    the number of bytes to print from a char array
 *  @param prefixString:    a char array to print before
 *  @param postfixString:   a char array to print after
 */
void printNBytes(char *string, int bytesToPrint, char prefixString[], char postfixString[]) { 
    printf("%s", prefixString);
    for (int i = 0; i < bytesToPrint; i++) { 
        printf("%c", string[i]);
    }
    printf("%s", postfixString);
}


/**
 *  prints hex representation of an n byte char array
 *  @param *string:         a pointer to first element in a character array
 *  @param bytesToPrint:    the number of bytes to print from a char array
 */
void printHexNBytes(char *string, int bytesToPrint) { 
    // 15 == (0000 1111), masks the low 4 bits in a byte
    for (int i = 0; i < bytesToPrint; i++) {
        printf("%X", (string[i] >> 4) & 15);
        printf("%X ", string[i] & 15);
    }
    printf("\n");
}

/**
 *  prints binary bits of a 4 byte char array
 *  @param bitPattern:  a 4 byte char array
 */
void printCharArrayBits(char *bitPattern) { 
    for (int j = 0; j < 4; j++) {
        for (int i = 7; i >= 0; i--) {
            printf("%d", (bitPattern[j] >> i) & 1);
        }
        printf(" ");
    }
    printf("\n");
}


/**
 *  prints the binary bits for any type passed
 *  @param size:    the number of bytes to print
 *  @param ptr:     a pointer to an element of any type
 */
void printBits(size_t const size, void const * const ptr) {
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}