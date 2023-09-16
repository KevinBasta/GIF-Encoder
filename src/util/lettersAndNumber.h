
#ifndef LETTERS_AND_NUMBERS_H
#define LETTERS_AND_NUMBERS_H

#include <stdint.h>
#include "main.h"

#define MAX_LETTER_PIXEL_WIDTH 10

typedef struct letterPattern {
    size_t width;
    size_t height;
    size_t baseline;
    u8 *pattern;
} letterPattern;

letterPattern *getLetterOrNumber(char letterOrNumber);

#endif // LETTERS_AND_NUMBERS_H