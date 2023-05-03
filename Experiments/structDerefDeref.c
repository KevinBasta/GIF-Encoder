#include <stdlib.h>
#include <stdio.h>

typedef struct box { 
    unsigned int *size; 
} box;

int main() { 
    box *testBox = (box*) malloc(sizeof(box));
    testBox->size = (unsigned int*) malloc(sizeof(unsigned int));
    *(testBox->size) = 1; 

    printf("%d\n", testBox->size);
    printf("%d\n", *(testBox->size));
}