
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include "main.h"

#include "GIFColorTable.h"

/**
 * @brief Create a color table
 * @param size number of RGB entries
 * @return colorTable pointer or NULL
 */
colorTable *colortableInit() {
    colorTable *table = calloc(1, sizeof(colorTable));
    if (table == NULL)
        return NULL;

    table->lastIndex = getLastColorIndex(3);
    table->currentIndex = 0;
    table->items = calloc(MAX_COLOR_TABLE_ENTRIES, sizeof(RGB));
    if (table->items == NULL) {
        free(table);
        return NULL;
    }

    for (u32 i = 0; i < MAX_COLOR_TABLE_ENTRIES; i++) {
        // When colors occupy less than the total size
        // they need to be filled with fake (or 0) values
        table->items[i].red     = 0;
        table->items[i].green   = 0;
        table->items[i].blue    = 0;
    }

    return table;
}

// Append an RGB color to a color table
STATUS_CODE colortableAppendRGB(colorTable *table, u8 red, u8 green, u8 blue) {
    COLOR_TABLE_NULL_CHECK(table);

    if (table->currentIndex > 255)
        return COLOR_TABLE_OVERFLOW;

    table->items[table->currentIndex].red   = red;
    table->items[table->currentIndex].green = green;
    table->items[table->currentIndex].blue  = blue;

    table->lastIndex = getLastColorIndex(table->currentIndex);
    table->currentIndex++;

    return OPERATION_SUCCESS;
}

void freeColorTable(colorTable *table) {
    if (table != NULL) {
        if (table->items != NULL)
            free(table->items);

        free(table);
    }
}

// Returning color table value information based on last index
static u16 colortableSizeToCodeTableEntry(char *request, i32 colorTableLastIndex) {
    //
    // 0: LWZMinCodeSize 
    // 1: ClearCode
    // 2: EOICode
    //
    u16 LWZ_CC_EOI[3] = {0, 0, 0};

    if (colorTableLastIndex < 0 || colorTableLastIndex > 255) {
        return 0;
    } else if (colorTableLastIndex <= 3) {
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
        // This case should never run
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

// Get the last index number in the code table
u16 getLastColorIndex(i32 colorTableLastIndex) {
    // Subtracting one from the clear code to get the last index
    return colortableSizeToCodeTableEntry("CC", colorTableLastIndex) - 1;
}

// The following functions take in the size, and use -1 for the index
u8 getLWZMinCodeSize(i32 colorTableLastIndex) {
    return colortableSizeToCodeTableEntry("LWZ", colorTableLastIndex);
}

u16 getClearCodeValue(i32 colorTableLastIndex) {
    return colortableSizeToCodeTableEntry("CC", colorTableLastIndex);
}

u16 getEOICodeValue(i32 colorTableLastIndex) {
    return colortableSizeToCodeTableEntry("EOI", colorTableLastIndex);
}
