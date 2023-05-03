#include <stdio.h>

int main(int argc, char **argv) { 
    // results in two different values on the stack
    int x = 10;
    int y = x;

    printf("%d, %p\n", x, &x);
    printf("%d, %p\n", y, &y);

}