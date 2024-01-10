
#ifndef GIF_ENCODER_HASHMAP_H
#define GIF_ENCODER_HASHMAP_H

#include <stdint.h>
#include "main.h"
#include "array.h"

typedef struct gif_HashMapEntry {
    gif_array *key;
    u32 value;
} gif_HashMapEntry;

typedef struct gif_HashMap {
    size_t size;
    size_t currentCount;
    gif_HashMapEntry **entries;
} gif_HashMap;

gif_HashMap *gif_hashmapInit(size_t size);

STATUS_CODE gif_hashmapInsert(gif_HashMap *map, gif_array *key, u32 value);
STATUS_CODE gif_hashmapSearch(gif_HashMap *map, gif_array *key, u32 *value);

void gif_hashmapPrint(gif_HashMap *map);
void gif_hashmapSearchPrint(gif_HashMap *map, char *key);

void gif_freeHashMapEntry(gif_HashMapEntry *entry);
void gif_freeHashMap(gif_HashMap *map);

#endif // GIF_ENCODER_HASHMAP_H