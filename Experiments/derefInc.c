#include <stdio.h>
#include <stdlib.h>

void incTest(int *test) {
    *test += 1;
}

int main() { 
    int test = 0;
    printf("%d\n", test);

    incTest(&test);
    printf("%d\n", test);
}
