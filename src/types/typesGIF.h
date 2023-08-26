
#ifndef GIF_TYPES_H
#define GIF_TYPES_H

#include <stdint.h>
#include "main.h"
#include "HashMap.h"
#include "GIFEncode.h"

typedef struct RGB {
    u8 red;
    u8 green;
    u8 blue;
} RGB;

typedef struct colorTable {
    u32 size;
    RGB *arr;
} colorTable;

typedef struct codeTable {
    u32 index;
    HashMap *map;
} codeTable;

#endif // GIF_TYPES_H
