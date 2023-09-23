
#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

/**
 *  prints n bytes from a char array.
 *  mainly used to print the 4 bytes of a box type since '\0' is not stored
 *  @param *string:         a pointer to first element in a character array
 *  @param bytesToPrint:    the number of bytes to print from a char array
 *  @param prefixString:    a char array to print before
 *  @param postfixString:   a char array to print after
 */
void printNBytes(u8 *string, u32 bytesToPrint, u8 prefixString[], u8 postfixString[]) { 
    #ifdef PRINT_ENABLE
    
    PRINTF("%s", prefixString);
    for (u32 i = 0; i < bytesToPrint; i++) { 
        PRINTF("%c", string[i]);
    }
    PRINTF("%s", postfixString);

    #endif // PRINT_ENABLE
}

/**
 *  prints hex representation of an n byte char array
 *  @param *string:         a pointer to first element in a character array
 *  @param bytesToPrint:    the number of bytes to print from a char array
 */
void printHexNBytes(u8 *string, u32 bytesToPrint) { 
    #ifdef PRINT_ENABLE

    // 15 == (0000 1111), masks the low 4 bits in a byte
    for (u32 i = 0; i < bytesToPrint; i++) {
        PRINTF("%X", (string[i] >> 4) & 15);
        PRINTF("%X ", string[i] & 15);
    }
    PRINTF("\n");

    #endif // PRINT_ENABLE
}

/**
 *  prints binary bits of a 4 byte char array
 *  @param bitPattern:  a 4 byte char array
 */
void printCharArrayBits(u8 *bitPattern) { 
    #ifdef PRINT_ENABLE

    for (u32 j = 0; j < 4; j++) {
        for (u32 i = 7; i >= 0; i--) {
            PRINTF("%d", (bitPattern[j] >> i) & 1);
        }
        PRINTF(" ");
    }
    PRINTF("\n");

    #endif // PRINT_ENABLE
}

/**
 *  prints the binary bits for any type passed
 *  @param size:    the number of bytes to print
 *  @param ptr:     a pointer to an element of any type
 */
void printIntBits(void const * const ptr, size_t const size) {
    #ifdef PRINT_ENABLE

    u8 *b = (u8*) ptr;
    u8 byte;
    i32 i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            PRINTF("%u", byte);
        }
        PRINTF(" ");
    }
    puts("");

    #endif // PRINT_ENABLE
}

void printBits(void const * const ptr, size_t const size) {
    #ifdef PRINT_ENABLE

    u8 *b = (u8*) ptr;
    u8 byte;
    i32 i, j;
    
    for (i = 0; i < size; i++) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            PRINTF("%u", byte);
        }
        PRINTF(" ");
    }
    puts("");

    #endif // PRINT_ENABLE
}

char* intToString(size_t i, u32 maxStringLength) {
    char *str     = calloc(6, sizeof(u8));
    if (str == NULL)
        return NULL;

    sprintf(str, "%ld", i);
    
    return str;
}
