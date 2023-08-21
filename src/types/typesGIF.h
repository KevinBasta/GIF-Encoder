#ifndef DATA_STRUCTURES
    #define DATA_STRUCTURES
    #include "linkedList.h"
    #include "hashmap.h"
    #include "array.h"
    #include "typesStorage.h"
#endif //DATA_STRUCTURES

typedef struct RGB {
    u8 red;
    u8 green;
    u8 blue;
} RGB;

typedef struct colorTable {
    u32 size;
    RGB *arr;
} colorTable;

typedef struct codeTable {
    u32 index;
    HashMap *map;
} codeTable;