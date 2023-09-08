
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

HashMap *hashmapInit(size_t size);

STATUS_CODE hashmapInsert(HashMap *map, char *key, char *value);
char *hashmapSearch(HashMap *map, char *key); // !!TODO - REFACTOR
STATUS_CODE hashmapSearchConvert(HashMap *map, char *key, u32 *value);

char *hashmapCreateKey(char *str, u32 length);
void hashmapPrint(HashMap *map);
void hashmapSearchPrint(HashMap *map, char *key);

void freeHashMapEntry(HashMapEntry *entry);
void freeHashMap(HashMap *map);

#endif // HASHMAP_H