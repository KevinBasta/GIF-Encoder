#include <stdio.h>
#include <stdlib.h>

#define BUFFERSIZE 8
#define TRUE 1

typedef struct box { 
    int size;
    char name[4];
} box;

int main() { 
    FILE *video = fopen("op.mp4", "rb"); // read binary
    //FILE *output = fopen("output.mp4", "wb"); // write binary
    //box* boxes = (box*) calloc(1000, sizeof(box));
    size_t bytesread;

    int sizeBuffer = 0;
    unsigned char *typeBuffer = (unsigned char*) malloc(4); 
    int cont = 1;
    while (cont != 0) { 
        char *temp;
        unsigned int x = 0;
        unsigned long int y = 0;

        if (cont == 1) {
            temp = (char*) malloc(4);
            fread(temp, 1, 4, video);

            // prints binary of a box size
            for (int j = 0; j < 4; j++) {
                for (int i = 7; i >= 0; i--) {
                    printf("%d", (temp[j] >> i) & 1);
                }
                printf(" ");
            }
            printf("\n");
            
            // converts box size binary to int
            for (int j = 0; j < 4; j++) {
                for (int i = 7; i >= 0; i--) {
                    x = x | (((temp[j] >> i) & 1) << (((4 - j)*7) + (i - 7)));
                }
            }
            printf("%d\n", x);

        
            // reads type of a box
            fread(typeBuffer, 4, 1, video);
            for (int i = 0; i < 4; i++) { 
                printf("%c", typeBuffer[i]);
            }
            printf("\n");


            // reads body of a box
            char *dumpBuffer = (char*) malloc(x - 4);
            bytesread = fread(dumpBuffer, x - 4, 1, video);
            free(dumpBuffer);
            scanf("%d", &cont);

        } else if (cont == 2) { 
            temp = (char*) malloc(8);
            fread(temp, 1, 8, video);

            // prints binary of a box size
            for (int j = 0; j < 8; j++) {
                for (int i = 15; i >= 0; i--) {
                    printf("%d", (temp[j] >> i) & 1);
                }
                printf(" ");
            }
            printf("\n");
            
            // converts box size binary to int
            for (int j = 0; j < 8; j++) {
                for (int i = 15; i >= 0; i--) {
                    y = y | (((temp[j] >> i) & 1) << (((8 - j)*15) + (i - 15)));
                }
            }
            printf("%ld\n", y);

            // reads type of a box
            fread(typeBuffer, 8, 1, video);
            for (int i = 0; i < 8; i++) { 
                printf("%c", typeBuffer[i]);
            }
            printf("\n");


            // reads body of a box
            char *dumpBuffer = (char*) malloc(y);
            bytesread = fread(dumpBuffer, y, 1, video);
            free(dumpBuffer);
            scanf("%d", &cont);
        }

        


        
    }
}