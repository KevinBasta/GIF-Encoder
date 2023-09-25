
#ifndef GIF_ENCODER_HASHMAP_H
#define GIF_ENCODER_HASHMAP_H

#include <stdint.h>
#include "main.h"

typedef struct gif_HashMapEntry {
    void *key;
    void *value;
} gif_HashMapEntry;

typedef struct gif_HashMap {
    size_t size;
    size_t currentCount;
    gif_HashMapEntry **entries;
} gif_HashMap;

gif_HashMap *gif_hashmapInit(size_t size);

STATUS_CODE gif_hashmapInsert(gif_HashMap *map, char *key, void *value);
void *gif_hashmapSearch(gif_HashMap *map, char *key);
char *gif_hashmapSearchWithIntKey(gif_HashMap *map, size_t key);
STATUS_CODE gif_hashmapSearchConvert(gif_HashMap *map, char *key, u32 *value);

char *gif_hashmapCreateKey(char *str, u32 length);
void gif_hashmapPrint(gif_HashMap *map);
void gif_hashmapSearchPrint(gif_HashMap *map, char *key);

void gif_freeHashMapEntry(gif_HashMapEntry *entry);
void gif_freeHashMap(gif_HashMap *map);

#endif // GIF_ENCODER_HASHMAP_H