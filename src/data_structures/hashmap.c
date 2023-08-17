#ifndef COMMON_HEAD
    #define COMMON_HEAD
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <math.h>
    #include <time.h>
#endif //COMMON_HEAD

#ifndef COMMON_TYPES
    #define COMMON_TYPES
    #include "main.h"
    #include "hashmap.h"
    #include "typesUtility.h"
#endif //COMMON_TYPES

#ifndef COMMON_UTIL
    #define COMMON_UTIL
    #include "printUtility.h"
#endif //COMMON_UTIL


size_t hashFunction(char *str) {
    size_t i = 0;

    for (int j = 0; str[j]; j++)
        i+= str[j];
    
    return i % 10;
}

HashMapEntry *hashMapCreateEntry(char *key, char *value) {
    HashMapEntry *entry = (HashMapEntry*) calloc(1, sizeof(HashMapEntry));
    entry->key   = (char*) malloc(strlen(key) + 1);
    entry->value = (char*) malloc(strlen(value) + 1);

    strcpy(entry->key, key);
    strcpy(entry->value, value);

    return entry;
}

HashMap *hashMapInit(size_t size) {
    HashMap *map = (HashMap*) calloc(1, sizeof(HashMap));

    map->size = size;
    map->currentCount = 0;
    map->items = (HashMapEntry**) calloc(size, sizeof(HashMapEntry*));

    return map;
}



void freeHashMapEntry(HashMapEntry *entry) {
    free(entry->key);
    free(entry->value);
}

void freeHashMap(HashMap *map) {
    for (int i = 0; i < map->size; i++) {
        HashMapEntry *entry = map->items[i]

        if (entry != NULL) {
            freeHashMapEntry(entry);
        }
    }

    free(map->items);
    free(map);
}