
#ifndef GIF_CODE_TABLE_H
#define GIF_CODE_TABLE_H

#include <stdint.h>
#include "main.h"

#include "hashmap.h"
#include "GIFCodeTable.h"
#include "GIFColorTable.h"

typedef struct codeTable {
    u32 index;
    HashMap *map;
} codeTable;

codeTable* codetableInit(colorTable *clrTable);
u32 codetableGetNextIndex(codeTable *table);
u32 getCurrentIndexCodeTable(codeTable *table);
u8 getLWZMinCodeSize(u32 colorTableSize);
u16 getClearCodeValue(u32 colorTableSize);
void freeCodeTable(codeTable *table);
void freeColorTable(colorTable *table);

#endif // GIF_CODE_TABLE_H