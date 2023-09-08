
#include <stdint.h>
#include "main.h"

#include "GIFColorTable.h"

void addRGBArrayEntry(RGB *table, u32 index, u8 red, u8 green, u8 blue) {
    table[index].red = red;
    table[index].green = green;
    table[index].blue = blue;
}