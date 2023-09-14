
#ifndef LETTERS_AND_NUMBERS_H
#define LETTERS_AND_NUMBERS_H

#include <stdint.h>
#include "main.h"

#define LETTER_PIXEL_WIDTH      5
#define LETTER_PIXEL_HEIGHT     7

typedef struct letterPattern {
    size_t width;
    u8 *pattern;
} letterPattern;

u8 *getLetterOrNumber(char letterOrNumber);

#endif // LETTERS_AND_NUMBERS_H