
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

HashMap *HashMapInit(size_t size);

STATUS_CODE HashMapInsert(HashMap *map, char *key, char *value);
char *HashMapSearch(HashMap *map, char *key);

char *HashMapCreateKey(char *str, u32 length);
void HashMapPrint(HashMap *map);
void HashMapSearchPrint(HashMap *map, char *key);

void freeHashMapEntry(HashMapEntry *entry);
void freeHashMap(HashMap *map);

#endif // HASHMAP_H