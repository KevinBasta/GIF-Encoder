
#ifndef COLOR_TABLE_H
#define COLOR_TABLE_H

#include <stdint.h>
#include "main.h"

typedef struct RGB {
    u8 red;
    u8 green;
    u8 blue;
} RGB;

typedef struct colorTable {
    u32 size;
    u32 currentIndex;
    RGB *items;
} colorTable;

colorTable *colortableInit(u32 size);
STATUS_CODE colortableAppendRGB(colorTable *table, u8 red, u8 green, u8 blue);
void freeColorTable(colorTable *table);

u16 getLastColorIndex(u32 colorTableInputSize);
u8 getLWZMinCodeSize(u32 colorTableSize);
u16 getClearCodeValue(u32 colorTableSize);
u16 getEOICodeValue(u32 colorTableSize);

#endif // COLOR_TABLE_H