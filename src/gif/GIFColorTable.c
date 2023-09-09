
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include "main.h"

#include "GIFColorTable.h"

colorTable *colortableInit(u32 size) {
    colorTable *table = calloc(1, sizeof(colorTable));
    
    table->size = getLastColorIndex(size) + 1;
    table->currentIndex = 0;
    table->items = calloc(table->size, sizeof(RGB));

    for (u32 i = 0; i < table->size; i++) {
        // When colors occupy less than the total size
        // they need to be filled with fake (or 0) values
        table->items[i].red     = 0;
        table->items[i].green   = 0;
        table->items[i].blue    = 0;
    }

    return table;
}

STATUS_CODE colortableAppendRGB(colorTable *table, u8 red, u8 green, u8 blue) {
    COLOR_TABLE_NULL_CHECK(table);

    table->items[table->currentIndex].red   = red;
    table->items[table->currentIndex].green = green;
    table->items[table->currentIndex].blue  = blue;

    table->currentIndex++;

    return OPERATION_SUCCESS;
}

void freeColorTable(colorTable *table) {
    free(table->items);
    free(table);
}

static u16 colortableSizeToCodeTableEntry(char *request, u32 colorTableLastIndex) {
    //
    // 0: LWZMinCodeSize 
    // 1: ClearCode
    // 2: EOICode
    //
    u16 LWZ_CC_EOI[3] = {0, 0, 0};

    if (colorTableLastIndex <= 3) {
        LWZ_CC_EOI[0] = 2; LWZ_CC_EOI[1] = 4; LWZ_CC_EOI[2] = 5;
    } else if (colorTableLastIndex <= 7) {
        LWZ_CC_EOI[0] = 3; LWZ_CC_EOI[1] = 8; LWZ_CC_EOI[2] = 9;
    } else if (colorTableLastIndex <= 15) {
        LWZ_CC_EOI[0] = 4; LWZ_CC_EOI[1] = 16; LWZ_CC_EOI[2] = 17;
    } else if (colorTableLastIndex <= 31) {
        LWZ_CC_EOI[0] = 5; LWZ_CC_EOI[1] = 32; LWZ_CC_EOI[2] = 33;
    } else if (colorTableLastIndex <= 63) {
        LWZ_CC_EOI[0] = 6; LWZ_CC_EOI[1] = 64; LWZ_CC_EOI[2] = 65;
    } else if (colorTableLastIndex <= 127) {
        LWZ_CC_EOI[0] = 7; LWZ_CC_EOI[1] = 128; LWZ_CC_EOI[2] = 129;
    } else if (colorTableLastIndex <= 255) {
        LWZ_CC_EOI[0] = 8; LWZ_CC_EOI[1] = 256; LWZ_CC_EOI[2] = 257;
    } else {
        return 0;
    }

    if (strncmp(request, "LWZ", 3) == 0) {
        return LWZ_CC_EOI[0];
    } else if (strncmp(request, "CC", 3) == 0) {
        return LWZ_CC_EOI[1];
    } else if (strncmp(request, "EOI", 3) == 0) {
        return LWZ_CC_EOI[2];
    }
}

/**
 * @brief Get the last index number in the code table
 * @param colorTableInputSize the size given by interface to create a color table
 * @return The last index number of the color table
 */
u16 getLastColorIndex(u32 colorTableInputSize) {
    // Subtracting one from the clear code to get the last index
    return colortableSizeToCodeTableEntry("CC", colorTableInputSize - 1) - 1;
}

// The following functions take in the size, and use -1 for the index
u8 getLWZMinCodeSize(u32 colorTableSize) {
    return colortableSizeToCodeTableEntry("LWZ", colorTableSize - 1);
}


u16 getClearCodeValue(u32 colorTableSize) {
    return colortableSizeToCodeTableEntry("CC", colorTableSize - 1);
}

u16 getEOICodeValue(u32 colorTableSize) {
    return colortableSizeToCodeTableEntry("EOI", colorTableSize - 1);
}
