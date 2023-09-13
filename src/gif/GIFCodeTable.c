
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

#include "hashmap.h"
#include "GIFColorTable.h"
#include "GIFCodeTable.h"
#include "printUtility.h"


/**
 * @brief Create a code table out of a color table
 * @param clrTable color table to build from
 * @return codeTable pointer or NULL
 */
codeTable* codetableInit(colorTable *clrTable) {
    STATUS_CODE status;
    
    codeTable *table = calloc(1, sizeof(codeTable));
    if (table == NULL)
        return NULL;

    // Max code table entries is 4095 as defined by the GIF format
    // To allow for a max load factor <= 0.5, the closest prime to (4095 * 2) is used
    HashMap *map = hashmapInit(8191);
    if (map == NULL) {
        free(table);
        return NULL;
    }

    table->map = map;

    for (size_t i = 0; i <= clrTable->lastIndex; i++) {
        char *str1 = intToString(i, 6);
        char *str2 = intToString(i, 6);
        status = hashmapInsert(map, str1, str2);
        if (status != OPERATION_SUCCESS) {
            freeCodeTable(table);
            return NULL;
        }

        table->index = i;
    }

    status = hashmapInsert(map, hashmapCreateKey("cc", 2), intToString(getClearCodeValue(clrTable->lastIndex), 6));
    if (status != OPERATION_SUCCESS) {
        freeCodeTable(table);
        return NULL;
    }

    status = hashmapInsert(map, hashmapCreateKey("eoi", 3), intToString(getEOICodeValue(clrTable->lastIndex), 6));
    if (status != OPERATION_SUCCESS) {
        freeCodeTable(table);
        return NULL;
    }

    hashmapPrint(map);
    table->index += 2;

    return table;
}

// Return next code table index for LZW
u32 codetableGetNextIndex(codeTable *table) {
    table->index++;
    //printf("%d\n", table->map->currentCount);

    return table->index;
}

u32 codetableGetCurrentIndex(codeTable *table) {
    return table->index;
}

void freeCodeTable(codeTable *table) {
    if (table != NULL) {
        if (table->map != NULL)
            freeHashMap(table->map);

        free(table);
    }
}
