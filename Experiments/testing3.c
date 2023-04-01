#include <stdio.h>
#include <stdlib.h>

void printChar(char test, short size) { 
    for (short i = size; i >= 0; i--) { 
        printf("%d", (test >> i) & 1);
    }
    printf("\n");
}

void printReverseChar(char test, short size) { 
    for (short i = 0; i <= size; i++) { 
        printf("%d", (test >> i) & 1);
    }
    printf("\n");
}

int main() { 
    char test = 'a';
    printChar(test, 7);
    test = 'A';
    printChar(test, 7);
    


    char *arrTest = (char*) malloc(1);
    arrTest = "a";
    printChar(*arrTest, 7);

    // doesn't work because it's just printing the first array index
    char *arrTestTwo = (char*) malloc(2);
    arrTestTwo = "aA";
    printChar(*arrTestTwo, 7*2);

    // accessing the individual indices works
    char *arrTestThree = (char*) malloc(2);
    arrTestTwo = "aA";
    printChar(arrTestTwo[0], 7);
    printChar(arrTestTwo[1], 7);


}

/*
Observations
- ensure 0-7 for iteration
- iterate backwards
- access one index at a time
*/