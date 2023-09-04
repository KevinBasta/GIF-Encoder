
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

#include "hashmap.h"
#include "typesGIF.h"
#include "printUtility.h"

u32 colorTableSizeToCodeTableEntry(char *request, u32 colorTableSize) {
    // 0: LWZMinCodeSize 1: ClearCode 2: EOICode
    u16 LWZ_CC_EOI[3] = {0, 0, 0};

    if (colorTableSize <= 3) {
        LWZ_CC_EOI[0] = 2; LWZ_CC_EOI[1] = 4; LWZ_CC_EOI[2] = 5;
    } else if (colorTableSize <= 7) {
        LWZ_CC_EOI[0] = 3; LWZ_CC_EOI[1] = 8; LWZ_CC_EOI[2] = 9;
    } else if (colorTableSize <= 15) {
        LWZ_CC_EOI[0] = 4; LWZ_CC_EOI[1] = 16; LWZ_CC_EOI[2] = 17;
    } else if (colorTableSize <= 31) {
        LWZ_CC_EOI[0] = 5; LWZ_CC_EOI[1] = 32; LWZ_CC_EOI[2] = 33;
    } else if (colorTableSize <= 63) {
        LWZ_CC_EOI[0] = 6; LWZ_CC_EOI[1] = 64; LWZ_CC_EOI[2] = 65;
    } else if (colorTableSize <= 127) {
        LWZ_CC_EOI[0] = 7; LWZ_CC_EOI[1] = 128; LWZ_CC_EOI[2] = 129;
    } else if (colorTableSize <= 255) {
        LWZ_CC_EOI[0] = 8; LWZ_CC_EOI[1] = 256; LWZ_CC_EOI[2] = 257;
    } else {
        return 0;
    }

    if (strcmp(request, "LWZ") == 0) {
        return LWZ_CC_EOI[0];
    } else if (strcmp(request, "CC") == 0) {
        return LWZ_CC_EOI[1];
    } else if (strcmp(request, "EOI") == 0) {
        return LWZ_CC_EOI[2];
    }
}

u8 getLWZMinCodeSize(u32 colorTableSize) {
    return colorTableSizeToCodeTableEntry("LWZ", colorTableSize);
}

u16 getClearCodeValue(u32 colorTableSize) {
    return colorTableSizeToCodeTableEntry("CC", colorTableSize);
}

u16 getEOICodeValue(u32 colorTableSize) {
    return colorTableSizeToCodeTableEntry("EOI", colorTableSize);
}

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

    hashmapInsert(map, hashmapCreateKey("cc", 2), intToString(getClearCodeValue(clrTable->size - 1)));
    hashmapInsert(map, hashmapCreateKey("eoi", 3), intToString(getEOICodeValue(clrTable->size - 1)));
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

void freeColorTable(colorTable *table) {
    free(table->arr);
    free(table);
}