
#ifndef GIF_LETTERS_AND_NUMBERS_H
#define GIF_LETTERS_AND_NUMBERS_H

#include <stdint.h>
#include "main.h"

#define MAX_LETTER_PIXEL_WIDTH      10
#define ROW_HEIGHT_IN_PIXELS        11

typedef struct gif_letterPattern {
    size_t width;
    size_t height;
    size_t baseline;
    u8 *pattern;
} gif_letterPattern;

gif_letterPattern *gif_getLetterOrNumber(char letterOrNumber);
u32 gif_calculateWordPixelWidth(char *word, u32 wordLength);

#endif // GIF_LETTERS_AND_NUMBERS_H