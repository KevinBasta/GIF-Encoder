
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "hashmap.h"
#include "typesGIF.h"
#include "printUtility.h"

codeTable* initCodeTable(colorTable *clrTable) {
    codeTable *table = calloc(1, sizeof(codeTable));
    
    // Max code table entries is 4095 as defined by the GIF format
    // To allow for a max load factor <= 0.5, the closest prime to (4095 * 2) is used
    HashMap *map = initHashMap(8191);
    table->map = map;

    for (size_t i = 0; i < clrTable->size; i++) {
        char *str = intToString(i);
        insertHashMap(map, str, str);

        table->index = i;
    }

    insertHashMap(map, createKeyHashMap("cc", 2), intToString(3));
    insertHashMap(map, createKeyHashMap("eoi", 3), intToString(4));
    printHashMap(map);
    table->index = table->index + 3;

    return table;
}

u32 getNextIndexCodeTable(codeTable *table) {
    table->index++;

    return table->index;
}

void freeCodeTable(codeTable *table) {
    freeHashMap(table->map);
    free(table);
}