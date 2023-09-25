
#ifndef GIF_ENCODER_CODE_TABLE_H
#define GIF_ENCODER_CODE_TABLE_H

#include <stdint.h>
#include "main.h"

#include "hashmap.h"
#include "GIFCodeTable.h"
#include "GIFColorTable.h"

typedef struct gif_codeTable {
    u32 index;
    gif_HashMap *map;
} gif_codeTable;

gif_codeTable* gif_codetableInit(gif_colorTable *clrTable);
u32 gif_codetableGetNextIndex(gif_codeTable *table);
u32 gif_codetableGetCurrentIndex(gif_codeTable *table);
void gif_freeCodeTable(gif_codeTable *table);

#endif // GIF_ENCODER_CODE_TABLE_H