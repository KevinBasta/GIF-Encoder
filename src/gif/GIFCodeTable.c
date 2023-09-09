
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

#include "hashmap.h"
#include "GIFColorTable.h"
#include "GIFCodeTable.h"
#include "printUtility.h"


codeTable* codetableInit(colorTable *clrTable) {
    codeTable *table = calloc(1, sizeof(codeTable));
    
    // Max code table entries is 4095 as defined by the GIF format
    // To allow for a max load factor <= 0.5, the closest prime to (4095 * 2) is used
    HashMap *map = hashmapInit(8191);
    table->map = map;

    for (size_t i = 0; i < clrTable->size; i++) {
        char *str1 = intToString(i);
        char *str2 = intToString(i);
        hashmapInsert(map, str1, str2);

        table->index = i;
    }

    hashmapInsert(map, hashmapCreateKey("cc", 2), intToString(getClearCodeValue(clrTable->size)));
    hashmapInsert(map, hashmapCreateKey("eoi", 3), intToString(getEOICodeValue(clrTable->size)));
    hashmapPrint(map);
    table->index += 2;

    return table;
}

u32 codetableGetNextIndex(codeTable *table) {
    table->index++;
    //printf("%d\n", table->map->currentCount);

    return table->index;
}

u32 getCurrentIndexCodeTable(codeTable *table) {
    return table->index;
}

void freeCodeTable(codeTable *table) {
    freeHashMap(table->map);
    free(table);
}
