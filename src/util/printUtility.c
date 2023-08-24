#ifndef COMMON_HEAD
    #define COMMON_HEAD
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <math.h>
    #include <time.h>
#endif //COMMON_HEAD

#ifndef COMMON_TYPES
    #define COMMON_TYPES
    #include "main.h"
    #include "typesMPEG-4.h"
    #include "typesAVC.h"
    #include "linkedList.h"
    #include "typesStorage.h"
    #include "typesUtility.h"
#endif //COMMON_TYPES

#ifndef COMMON_UTIL
    #define COMMON_UTIL
    #include "bitUtility.h"
    #include "endianUtility.h"
    #include "printUtility.h"
    #include "memoryManagement.h"
#endif //COMMON_UTIL

/**
 *  prints n bytes from a char array.
 *  mainly used to print the 4 bytes of a box type since '\0' is not stored
 *  @param *string:         a pointer to first element in a character array
 *  @param bytesToPrint:    the number of bytes to print from a char array
 *  @param prefixString:    a char array to print before
 *  @param postfixString:   a char array to print after
 */
void printNBytes(u8 *string, u32 bytesToPrint, u8 prefixString[], u8 postfixString[]) { 
    printf("%s", prefixString);
    for (u32 i = 0; i < bytesToPrint; i++) { 
        printf("%c", string[i]);
    }
    printf("%s", postfixString);
}


/**
 *  prints hex representation of an n byte char array
 *  @param *string:         a pointer to first element in a character array
 *  @param bytesToPrint:    the number of bytes to print from a char array
 */
void printHexNBytes(u8 *string, u32 bytesToPrint) { 
    // 15 == (0000 1111), masks the low 4 bits in a byte
    for (u32 i = 0; i < bytesToPrint; i++) {
        printf("%X", (string[i] >> 4) & 15);
        printf("%X ", string[i] & 15);
    }
    printf("\n");
}

/**
 *  prints binary bits of a 4 byte char array
 *  @param bitPattern:  a 4 byte char array
 */
void printCharArrayBits(u8 *bitPattern) { 
    for (u32 j = 0; j < 4; j++) {
        for (u32 i = 7; i >= 0; i--) {
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
void printIntBits(void const * const ptr, size_t const size) {
    u8 *b = (u8*) ptr;
    u8 byte;
    i32 i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}

void printBits(void const * const ptr, size_t const size) {
    u8 *b = (u8*) ptr;
    u8 byte;
    i32 i, j;
    
    for (i = 0; i < size; i++) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}

char* intToString(size_t i) {
    //u32 intStrLen = snprintf(NULL, 0, "%ld", i);
    char *str     = malloc( 100 );
    //snprintf(str, intStrLen, "%ld", i);
    sprintf(str, "%ld", i);
    printf("%s\n", str);
    
    return str;
}
