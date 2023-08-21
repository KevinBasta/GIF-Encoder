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

void printHashMap(HashMap *map);
void printHashMapSearch(HashMap *map, char *key);

void freeHashMapEntry(HashMapEntry *entry);
void freeHashMap(HashMap *map);