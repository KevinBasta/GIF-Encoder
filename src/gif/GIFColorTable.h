
#ifndef GIF_ENCODER_COLOR_TABLE_H
#define GIF_ENCODER_COLOR_TABLE_H

#include <stdint.h>
#include "main.h"

#define MAX_COLOR_TABLE_ENTRIES 256
#define MAX_COLOR_TABLE_INDEX   (MAX_COLOR_TABLE_ENTRIES - 1)

typedef struct gif_RGB {
    u8 red;
    u8 green;
    u8 blue;
} gif_RGB;

typedef struct gif_colorTable {
    u32 lastIndex;
    u32 currentIndex;
    gif_RGB *items;
} gif_colorTable;

gif_colorTable *gif_colortableInit();
STATUS_CODE     gif_colortableAppendRGB(gif_colorTable *table, u8 red, u8 green, u8 blue);
void            gif_freeColorTable(gif_colorTable *table);

u16 gif_getLastColorIndex(i32 colorTableInputSize);
u8  gif_getLWZMinCodeSize(i32 colorTableSize);
u16 gif_getClearCodeValue(i32 colorTableSize);
u16 gif_getEOICodeValue(i32 colorTableSize);

#endif // GIF_ENCODER_COLOR_TABLE_H