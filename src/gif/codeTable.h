
#ifndef CODE_TABLE_H
#define CODE_TABLE_H

codeTable* initCodeTable(colorTable *clrTable);
u32 getNextIndexCodeTable(codeTable *table);
void freeCodeTable(codeTable *table);

#endif // CODE_TABLE_H