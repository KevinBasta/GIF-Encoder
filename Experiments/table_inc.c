#include <stdio.h>
#include <stdlib.h>


typedef struct test { 
    int a;
    char b;
    short c;
} test; 

int main() { 
    int entries = 3;

    test **table = calloc(entries + 1, sizeof(test*));
    for (int i = 0; i < entries; i++) { 
        table[i] = malloc(sizeof(test));
        table[i]->a = i;
        table[i]->c = i + 5;
    }
    table[entries + 1] = NULL;

    int j = 0;
    for (test *i = table[j]; i != NULL; i = table[(++j)]) { 
        printf("%d\n", j);
        printf("a: %d c: %d\n", i->a, i->c);
    }

    // resutls: this is a good example of use of j++ vs ++j 
    // post increment would result in the first increment
    // to happen after the table value is set to the current j
    // while pre increment would increment j then the table value is 
    // set with it
}