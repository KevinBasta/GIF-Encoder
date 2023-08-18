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


size_t hashFunction(char *str, size_t size) {
    size_t i = 0;

    for (int j = 0; str[j]; j++)
        i += str[j];
    
    return i % size;
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
    map->entries = (HashMapEntry**) calloc(size, sizeof(HashMapEntry*));

    return map;
}


STATUS_CODE hashMapInsert(HashMap *map, char *key, char *value) {
    size_t index = hashFunction(key, map->size);
    HashMapEntry *newEntry = hashMapCreateEntry(key, value);
    
    // collision resolution: Open addressing
    while (map->entries[index] != NULL && index < map->size) {
        index++;
    }
    
    HashMapEntry *currentIndexItem = map->entries[index];
    if (currentIndexItem == NULL) {
        if (map->currentCount == map->size) {
            printf("hashmap insert fail\n");
            freeHashMapEntry(newEntry);
            return OPERATION_FAILED;
        }

        map->entries[index] = newEntry;
        map->currentCount++; 
    } else {
        return OPERATION_FAILED;
    }

    return OPERATION_SUCCESS;
}

char *hashMapSearch(HashMap *map, char *key) {
    int index = hashFunction(key, map->size);
    HashMapEntry *entry = map->entries[index];

    while (map->entries[index] != NULL &&
           index < map->size &&
           strncmp(entry->key, key, min(sizeof(entry->key), key) != 0)) {
        index++;
        entry = map->entries[index];
    }

    if (strncmp(entry->key, key, min(sizeof(entry->key), key) == 0))
        return entry->value;

    return NULL;
}


void printHashMap(HashMap *map) {
    for (size_t i = 0; i < map->size; i++) {
        if (map->entries[i] != NULL) {
            printf("key: %s \t value: %s \n", map->entries[i]->key, map->entries[i]->value);
        }
    }
}

void printHashMapSearch(HashMap *map, char *key) {
    char *value = hashMapSearch(map, key);

    if (value == NULL) {
        printf("Key [%s] not found\n", key);
    } else {
        printf("Key: [%s] \t Value: [%s]\n", key, value);
    }
}


void freeHashMapEntry(HashMapEntry *entry) {
    free(entry->key);
    free(entry->value);
}

void freeHashMap(HashMap *map) {
    for (size_t i = 0; i < map->size; i++) {
        HashMapEntry *entry = map->entries[i];

        if (entry != NULL) {
            freeHashMapEntry(entry);
        }
    }

    free(map->entries);
    free(map);
}
