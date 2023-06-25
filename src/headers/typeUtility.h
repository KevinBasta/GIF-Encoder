
u32 binarySearch(u32 target, u32 *table, u32 totalEntries, i32 (*comp) (const void *, const void *) );
i32 compi32(const void *a, const void *b);
i32 compu32(const void *a, const void *b);
void freeTable(void **table);
void freeDataReferenceTable(dataReferenceTableEntry **dataReferenceTable);
void quickSort(u32 *arr1, u32 *arr2, u32 low, u32 high);
void bubbleSort(u32 *arr1, u32* arr2, int n);
void freeMpegData(MPEG_Data *videoData);
void safeFree(void *data);