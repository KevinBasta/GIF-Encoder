
#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include "main.h"

typedef struct HashMapEntry {
    void *key;
    void *value;
} HashMapEntry;

typedef struct HashMap {
    size_t size;
    size_t currentCount;
    HashMapEntry **entries;
} HashMap;

HashMap *hashmapInit(size_t size);

STATUS_CODE hashmapInsert(HashMap *map, char *key, void *value);
void *hashmapSearch(HashMap *map, char *key);
char *hashmapSearchWithIntKey(HashMap *map, size_t key);
STATUS_CODE hashmapSearchConvert(HashMap *map, char *key, u32 *value);

char *hashmapCreateKey(char *str, u32 length);
void hashmapPrint(HashMap *map);
void hashmapSearchPrint(HashMap *map, char *key);

void freeHashMapEntry(HashMapEntry *entry);
void freeHashMap(HashMap *map);

#endif // HASHMAP_H