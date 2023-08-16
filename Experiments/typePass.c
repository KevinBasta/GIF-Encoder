#include <stdio.h>
#include <stdlib.h>

typedef enum {
    type1,
    type2,
} testType;

typedef struct test {
    void *t;
} test; 

test *makeTest(testType tp) {
    test *obj = malloc(sizeof(test));

    
}


int main() { 

}