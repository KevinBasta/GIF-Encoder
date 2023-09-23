
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <stdint.h>
#include "main.h"

#include "hashmap.h"
#include "printUtility.h"

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
        index += pow( (u8) key[j] + 2, 3);
    
    index = index % size;

    return index;
}

/**
 * @brief creates a hashmap entry
 * @param key   string
 * @param value any pointer
 * @return HashMapEntry 
 */
HashMapEntry *hashmapCreateEntry(char *key, void *value) {
    if (key == NULL || value == NULL)
        return NULL;

    HashMapEntry *entry = (HashMapEntry*) calloc(1, sizeof(HashMapEntry));
    entry->key   = key;
    entry->value = value;

    return entry;
}

/**
 * @brief creates a hashmap
 * @param size total number of entries map can hold
 * @return HashMap
 */
HashMap *hashmapInit(size_t size) {
    HashMap *map = (HashMap*) calloc(1, sizeof(HashMap));
    if (map == NULL)
        return NULL;

    map->entries = (HashMapEntry**) calloc(size, sizeof(HashMapEntry*));
    if (map->entries == NULL) {
        free(map);
        return NULL;
    }
    
    map->currentCount = 0;
    map->size = size;

    for (size_t i = 0; i < map->size; i++) {
        map->entries[i] = NULL;
    }

    return map;
}

/**
 * @brief insert a key-value pair in a hashmap
 * @param map   the map to insert in
 * @param key   the key of the new entry
 * @param value the value of the new entry
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE hashmapInsert(HashMap *map, char *key, void *value) {
    HASH_MAP_NULL_CHECK(map);
    if (key == NULL || value == NULL)
        return HASH_MAP_KEY_VAL_NULL;

    if (map->currentCount == map->size) {
        return HASH_MAP_FULL;
    }

    size_t index           = hashFunction(key, map->size);
    HashMapEntry *newEntry = hashmapCreateEntry(key, value);

    // collision resolution: Open addressing, quadratic probing
    size_t k = 0;
    while (map->entries[index] != NULL) {
        k++;
        index = (index + (k*k)) % map->size;

        // terminate if probing takes too long
        if (k >= map->size) {
            return HASH_MAP_INSERT_TIMEOUT;
        }
    }

    map->entries[index] = newEntry;
    map->currentCount++;
    //PRINTF("FINAL INDEX: %ld\n", index);

    return OPERATION_SUCCESS;
}

/**
 * @brief search hashmap by key
 * @param map hashmap to search in
 * @param key the key to search for
 * @return the value or null
 */
void *hashmapSearch(HashMap *map, char *key) {
    if (map == NULL || key == NULL)
        return NULL;

    size_t index        = hashFunction(key, map->size);
    HashMapEntry *entry = map->entries[index];
    void *value         = NULL;

    size_t k = 0;
    while (entry != NULL) {
        if (k >= map->size) {
            return NULL;
        }

        if (strncmp(entry->key, key, strlen(key)) == 0) {
            //PRINTF("entry key: %s \n      key: %s\n", entry->key, key);
            value = entry->value;
            //PRINTF("search found: %s\n", value);
            break;
        }

        k++;
        index = (index + (k*k)) % map->size;
        entry = map->entries[index];
    }

    return value;
}

/**
 * @brief Given an integer key, convert it to a string
 * search the hashmap, then free the string
 * @param map hashmap to search in
 * @param key int key to search for
 * @return the value or NULL
 */
char *hashmapSearchWithIntKey(HashMap *map, size_t key) {
    if (map == NULL)
        return NULL;

    char *strKey = intToString(key, 100);

    char *value = hashmapSearch(map, strKey);

    free(strKey);
    return value;
}

/**
 * @brief search hashmap by key
 * @param map   hashmap to search in
 * @param key   the key to search for
 * @param value the value returning
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE hashmapSearchConvert(HashMap *map, char *key, u32 *value) {
    HASH_MAP_NULL_CHECK(map);

    char *valueSearched = hashmapSearch(map, key);
    CHECK_NULL(valueSearched);

    *value = atoi(valueSearched);

    return OPERATION_SUCCESS;
}

/**
 * @brief Create a char array key for a hashmap
 * @param str       String literal
 * @param length    length of string literal
 * @return char array/string
 */
char *hashmapCreateKey(char *str, u32 length) {
    char *key = calloc(1, sizeof(length + 1));
    strncpy(key, str, length);
    key[length] = '\0';

    return key;
}

// Print all non-null hashmap entries
void hashmapPrint(HashMap *map) {
    #ifdef PRINT_ENABLE

    for (size_t i = 0; i < map->size; i++) {
        if (map->entries[i] != NULL) {
            PRINTF("key: %10s   value: %10s \n", (char*) map->entries[i]->key, (char*) map->entries[i]->value);
        }
    }

    #endif // PRINT_ENABLE
}

// Print the key value pair if key is found in hashmap
void hashmapSearchPrint(HashMap *map, char *key) {
    #ifdef PRINT_ENABLE

    char *value = hashmapSearch(map, key);

    if (value == NULL) {
        PRINTF("Key [%s] not found\n", key);
    } else {
        PRINTF("Key: [%s] \t Value: [%s]\n", key, value);
    }

    #endif // PRINT_ENABLE
}


void freeHashMapEntry(HashMapEntry *entry) {
    if (entry != NULL) {
        if (entry->key != NULL)
            free(entry->key);

        if (entry->value != NULL)
            free(entry->value);
    }
}

void freeHashMap(HashMap *map) {
    if (map != NULL) {
        if (map->entries != NULL) {
            for (size_t i = 0; i < map->size; i++) {
                HashMapEntry *entry = map->entries[i];

                if (entry != NULL) {
                    freeHashMapEntry(entry);
                }
                
                free(entry);
            }

            free(map->entries);
        }

        free(map);
    }
}
