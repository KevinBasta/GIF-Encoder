
typedef struct array {
    size_t totalEntries;
    size_t currentIndex;
    u32 *items;
} array;

array *initArray(size_t totalEntries);

STATUS_CODE appendItemArray(array *arr, u32 item);
void freeArray(array *arr);
