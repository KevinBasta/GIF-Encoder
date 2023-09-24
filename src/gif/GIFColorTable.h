
#ifndef GIF_ENCODER_COLOR_TABLE_H
#define GIF_ENCODER_COLOR_TABLE_H

#include <stdint.h>
#include "main.h"

#define MAX_COLOR_TABLE_ENTRIES 256
#define MAX_COLOR_TABLE_INDEX   (MAX_COLOR_TABLE_ENTRIES - 1)

typedef struct RGB {
    u8 red;
    u8 green;
    u8 blue;
} RGB;

typedef struct colorTable {
    u32 lastIndex;
    u32 currentIndex;
    RGB *items;
} colorTable;

colorTable *colortableInit();
STATUS_CODE colortableAppendRGB(colorTable *table, u8 red, u8 green, u8 blue);
void freeColorTable(colorTable *table);

u16 getLastColorIndex(i32 colorTableInputSize);
u8 getLWZMinCodeSize(i32 colorTableSize);
u16 getClearCodeValue(i32 colorTableSize);
u16 getEOICodeValue(i32 colorTableSize);

#endif // GIF_ENCODER_COLOR_TABLE_H