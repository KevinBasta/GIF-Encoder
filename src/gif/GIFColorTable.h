
#ifndef RGB_H
#define RGB_H

#include <stdint.h>
#include "main.h"

typedef struct RGB {
    u8 red;
    u8 green;
    u8 blue;
} RGB;

typedef struct colorTable {
    u32 size;
    RGB *arr;
} colorTable;

#endif // RGB_H