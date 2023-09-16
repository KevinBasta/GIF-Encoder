
#ifndef LETTERS_AND_NUMBERS_H
#define LETTERS_AND_NUMBERS_H

#include <stdint.h>
#include "main.h"

#define MAX_LETTER_PIXEL_WIDTH      10
#define ROW_HEIGHT_IN_PIXELS        11

typedef struct letterPattern {
    size_t width;
    size_t height;
    size_t baseline;
    u8 *pattern;
} letterPattern;

letterPattern *getLetterOrNumber(char letterOrNumber);
u32 calculateWordPixelWidth(char *word, u32 wordLength);

#endif // LETTERS_AND_NUMBERS_H