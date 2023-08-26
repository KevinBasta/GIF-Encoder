
#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include "main.h"

typedef struct HashMapEntry {
    char *key;
    char *value;
} HashMapEntry;

typedef struct HashMap {
    size_t size;
    size_t currentCount;
    HashMapEntry **entries;
} HashMap;

HashMap *initHashMap(size_t size);

STATUS_CODE insertHashMap(HashMap *map, char *key, char *value);
char *searchHashMap(HashMap *map, char *key);

char *createKeyHashMap(char *str, u32 length);
void printHashMap(HashMap *map);
void printHashMapSearch(HashMap *map, char *key);

void freeHashMapEntry(HashMapEntry *entry);
void freeHashMap(HashMap *map);

#endif // HASHMAP_H