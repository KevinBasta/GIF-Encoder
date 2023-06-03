#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint32_t u32;

typedef struct type1 { 
    int var1;
} type1;

typedef struct type2 { 
   int var1;
   unsigned int var2;
} type2;

void generalFree(void **arr) {
  int i = 0; 
  while (arr[i] != NULL) {
    free(arr[i]);
    i++;
  }
  free(arr);
}

/* u32 binarySearchGen(u32 sampleNumber, void **table, u32 totalEntries, ) { 
    u32 low = 0;
    u32 high = syncSampleTableEntries;
    u32 mid; 
    
    while (low <= high) { 
        mid = (low + high) / 2;

        if (sampleNumber == syncSampleTable[mid]->number) { 
            return mid;
        } else if (sampleNumber < syncSampleTable[mid]->number) { 
            high = mid - 1;
        } else if (sampleNumber > syncSampleTable[mid]->number) { 
            low = mid + 1;
        }

        //printf("%d %d %d %d %d\n", low, high, mid, sampleNumber, syncSampleTable[mid]->number);
    }

    return low; // since the preceeding key frame is required
} */

int main() { 
    type1 **arr1 = (type1**) calloc(2, sizeof(type1*));
    type1 *entryarr1 = malloc(sizeof(type1));
    arr1[0] = entryarr1;
    arr1[1] = NULL;

    type2 **arr2 = (type2**) calloc(2, sizeof(type2*));
    type2 *entryarr2 = malloc(sizeof(type2));
    arr2[0] = entryarr2;
    arr2[1] = NULL;

  generalFree((void**) arr1); // <-- will this know how to free without knowing type??
  generalFree((void**) arr2);

  //printf("%ld\n", sizeof(type2*));
}

// the anwer is yes
// https://stackoverflow.com/questions/5134891/how-do-i-use-valgrind-to-find-memory-leaks
// valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./a.out
