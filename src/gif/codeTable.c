#ifndef COMMON_HEAD
    #define COMMON_HEAD
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <math.h>
    #include <time.h>
    #include "main.h"
#endif //COMMON_HEAD

#ifndef COMMON_TYPES
    #define COMMON_TYPES
    #include "typesMPEG-4.h"
    #include "typesAVC.h"
    #include "typesGIF.h"
    #include "typesUtility.h"
#endif //COMMON_TYPES

#ifndef DATA_STRUCTURES
    #define DATA_STRUCTURES
    #include "linkedList.h"
    #include "hashmap.h"
    #include "array.h"
    #include "typesStorage.h"
#endif //DATA_STRUCTURES

#ifndef COMMON_UTIL
    #define COMMON_UTIL
    #include "bitUtility.h"
    #include "endianUtility.h"
    #include "printUtility.h"
    #include "memoryManagement.h"
#endif //COMMON_UTIL


codeTable* initCodeTable(colorTable *clrTable) {
    codeTable *table = calloc(1, sizeof(codeTable));
    HashMap *map = initHashMap(293);
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