
#ifndef GIF_CODE_TABLE_H
#define GIF_CODE_TABLE_H

#include <stdint.h>
#include "main.h"
#include "typesGIF.h"

codeTable* initCodeTable(colorTable *clrTable);
u32 getNextIndexCodeTable(codeTable *table);
void freeCodeTable(codeTable *table);

#endif // GIF_CODE_TABLE_H