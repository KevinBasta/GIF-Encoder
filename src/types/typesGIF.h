#ifndef COMMON_TYPES
    #define COMMON_TYPES
    #include "main.h"
    #include "typesMPEG-4.h"
    #include "typesAVC.h"
    #include "linkedList.h"
    #include "typesStorage.h"
    #include "typesUtility.h"
#endif //COMMON_TYPES

typedef struct RGB {
    u8 red;
    u8 green;
    u8 blue;
} RGB;

typedef struct colorTable {
    u32 numberOfEntries;
    RGB *arr;
} colorTable;