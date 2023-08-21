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

/**
 * @brief converts key string to an index
 * @param key       a string representing the key 
 * @param size      size of the map 
 * @return hashed index of the key
 */
static size_t hashFunction(char *key, size_t size) {
    size_t index = 1;

    // create index out of key
    for (int j = 0; key[j] != '\0'; j++)
        index *= key[j];
    
    index = index % size;
    printf("index is: %ld\n", index);

    return index;
}

/**
 * @brief creates a hashmap entry
 * @param key   string
 * @param value string
 * @return HashMapEntry 
 */
HashMapEntry *createEntryHashMap(char *key, char *value) {
    HashMapEntry *entry = (HashMapEntry*) calloc(1, sizeof(HashMapEntry));
    entry->key   = (char*) malloc(strlen(key) + 1);
    entry->value = (char*) malloc(strlen(value) + 1);

    strcpy(entry->key, key);
    strcpy(entry->value, value);

    return entry;
}

/**
 * @brief creates a hashmap
 * @param size total number of entries map can hold
 * @return HashMap
 */
HashMap *initHashMap(size_t size) {
    HashMap *map = (HashMap*) calloc(1, sizeof(HashMap));

    map->size = size;
    map->currentCount = 0;
    map->entries = (HashMapEntry**) calloc(size, sizeof(HashMapEntry*));

    return map;
}

/**
 * @brief insert a key-value pair in a hashmap
 * @param map   the map to insert in
 * @param key   the key of the new entry
 * @param value the value of the new entry
 * @return OPERATION_SUCCESS or OPERATION_FAILED
 */
STATUS_CODE insertHashMap(HashMap *map, char *key, char *value) {
    if (map->currentCount == map->size) {
        printf("hashmap full\n");
        return OPERATION_FAILED;
    }

    size_t index           = hashFunction(key, map->size);
    HashMapEntry *newEntry = createEntryHashMap(key, value);
    map->entries[index] = newEntry;

    // collision resolution: Open addressing, quadratic probing
    size_t k = 0;
    while (map->entries[index] != NULL) {
        printf("=====================\n");
        printf("collision at index %ld\n", index);
        printf("key %s, key at index %s\n", key, map->entries[index]->key);
        k++;
        index = (index + (k*k)) % map->size;
        printf("new index %ld\n", index);
        printf("=====================\n");

        // terminate if probing takes too long
        if (k >= map->size) {
            return OPERATION_FAILED;
        }
    }


    return OPERATION_SUCCESS;
}

/**
 * @brief search hashmap by key
 * @param map hashmap to search in
 * @param key the key to search for
 * @return the value or null
 */
char *searchHashMap(HashMap *map, char *key) {
    size_t index        = hashFunction(key, map->size);
    HashMapEntry *entry = map->entries[index];

    while (map->entries[index] != NULL &&
           strncmp(entry->key, key, min(sizeof(entry->key), sizeof(key)) != 0)) {
        index++;
        entry = map->entries[index];
    }

    if (strncmp(entry->key, key, min(sizeof(entry->key), sizeof(key)) == 0))
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
    char *value = searchHashMap(map, key);

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
