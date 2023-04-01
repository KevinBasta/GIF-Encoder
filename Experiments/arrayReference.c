/*
    testing to see if referencing a different memory allocation is viable
    instead of copying the information from the original array
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() { 
    int orgSize = 12;
    char *originalData = (char*) malloc(orgSize);
    originalData = "hello world";

    char *referenceData;
    referenceData = &(originalData[4]); 
        
    for (int i = 0; i < orgSize; i++) {
            //printf("ref: %p\t%c\n", &(referenceData[i]), referenceData[i]); 
            printf("org: %p\t%c\n", &(originalData[i]), originalData[i]); 
            //assert(&(referenceData[i]) == &(originalData[i]));
            //assert(referenceData[i] == originalData[i]);
    }

    printf("\n");

    for (int i = 0; i < 4; i++) {
        printf("ref: %p\t%c\n", &(referenceData[i]), referenceData[i]);
    }


}