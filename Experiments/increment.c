#include <stdlib.h>
#include <stdio.h>

int main() {
    int x = 0;
    int y = ++x;
    printf("%d %d\n", x, y); // 1 1

    int i = 0;
    int j = i++;
    printf("%d %d\n", i, j); // 1 0

    return 0;
}