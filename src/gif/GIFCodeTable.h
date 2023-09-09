
#ifndef CODE_TABLE_H
#define CODE_TABLE_H

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
void freeCodeTable(codeTable *table);

#endif // CODE_TABLE_H