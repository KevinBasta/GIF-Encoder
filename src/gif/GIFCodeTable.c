
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
gif_codeTable* gif_codetableInit(gif_colorTable *clrTable) {
    STATUS_CODE status;
    
    gif_codeTable *table = calloc(1, sizeof(gif_codeTable));
    if (table == NULL)
        return NULL;

    // Max code table entries is 4095 as defined by the GIF format
    // To allow for a max load factor <= 0.5, the closest prime to (4095 * 2) is used
    gif_HashMap *map = gif_hashmapInit(8191);
    if (map == NULL) {
        free(table);
        return NULL;
    }

    table->map = map;

    for (size_t i = 0; i <= clrTable->lastIndex + 2; i++) {
        gif_array *arr = gif_arrayInit(1);
        gif_arrayAppend(arr, i);

        status = gif_hashmapInsert(map, arr, i);
        if (status != OPERATION_SUCCESS) {
            gif_freeCodeTable(table);
            return NULL;
        }

        table->index = i;
    }

/* 
    u16 ccVal = gif_getClearCodeValue(clrTable->lastIndex);
    gif_array *ccArr = gif_arrayInit(1);
    gif_arrayAppend(ccArr, ccVal);

    status = gif_hashmapInsert(map, ccArr, ccVal);
    if (status != OPERATION_SUCCESS) {
        gif_freeCodeTable(table);
        return NULL;
    }

    u16 eoiVal = gif_getEOICodeValue(clrTable->lastIndex);
    gif_array *eoiArr = gif_arrayInit(1);
    gif_arrayAppend(eoiArr, eoiVal);

    status = gif_hashmapInsert(map, eoiArr, eoiVal);
    if (status != OPERATION_SUCCESS) {
        gif_freeCodeTable(table);
        return NULL;
    }
    table->index += 2; 
*/

    gif_hashmapPrint(map);

    return table;
}

// Return next code table index for LZW
u32 gif_codetableGetNextIndex(gif_codeTable *table) {
    table->index++;
    //PRINTF("%d\n", table->map->currentCount);

    return table->index;
}

u32 gif_codetableGetCurrentIndex(gif_codeTable *table) {
    return table->index;
}

void gif_freeCodeTable(gif_codeTable *table) {
    if (table != NULL) {
        if (table->map != NULL)
            gif_freeHashMap(table->map);

        free(table);
    }
}
