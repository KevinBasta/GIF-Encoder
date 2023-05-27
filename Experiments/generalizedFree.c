#include <stdio.h>
#include <stdlib.h>

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