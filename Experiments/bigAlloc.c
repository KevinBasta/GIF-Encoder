#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main() {
    uint8_t *test = calloc(1000000000, sizeof(uint8_t));
    for (int i = 0; i < 1000000000; i++) {
        test[i] = (uint8_t) i;
    }

    free(test);

    while(1) {

    }
}