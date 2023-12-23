
#ifndef GIF_PRINT_UTIL_H
#define GIF_PRINT_UTIL_H

#include <stdint.h>
#include "main.h"

void gif_printNBytes(u8 *string, u32 bytesToPrint, u8 prefixString[], u8 postfixString[]);
void gif_printHexNBytes(u8 *string, u32 bytesToPrint);
void gif_printCharArrayBits(u8 *bitPattern);
void gif_printBits(void const * const ptr, size_t const size);
void gif_printIntBits(void const * const ptr, size_t const size);
char *gif_intToString(long i, u32 maxStringLength);

#endif // GIF_PRINT_UTIL_H