
typedef struct array {
    size_t size;
    size_t currentIndex;
    u32 *items;
} array;

array *arrayInit(size_t size);
STATUS_CODE arrayAppend(array *arr, u32 item);

void printArray(array *arr);
void freeArray (array *arr);
