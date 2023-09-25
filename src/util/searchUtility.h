
#ifndef GIF_SEARCH_UTIL_H
#define GIF_SEARCH_UTIL_H

#include <stdbool.h>
#include <stdint.h>
#include "main.h"

#include "GIFInterface.h"

bool gif_frameInArray(GIFFrame *target, GIFFrame **array, u32 totalEntries);

u32 gif_binarySearch(u32 target, u32 *table, u32 totalEntries, i32 (*comp) (const void *, const void *));
i32 gif_compi32(const void *a, const void *b);
i32 gif_compu32(const void *a, const void *b);

void gif_bubbleSort(u32 *arr1, u32* arr2, int n);
void gif_quickSort(u32 *arr1, u32 *arr2, u32 low, u32 high);

#endif // GIF_SEARCH_UTIL_H