
typedef struct array {
    size_t size;
    size_t currentIndex;
    u32 *items;
} array;

array *initArray(size_t size);
STATUS_CODE appendArray(array *arr, u32 item);
STATUS_CODE popArray(array *arr);
u32 getItemArray(array *arr, size_t index);
void resetArray(array *arr);
char* concatArray(array *arr, char entrySeparator);

void printArray(array *arr);
void freeArray (array *arr);
