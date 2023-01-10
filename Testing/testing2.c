#include <stdio.h>
#include <stdlib.h>

#define BUFFERSIZE 8
#define TRUE 1

typedef struct box { 
    int size;
    char name[4];
} box;

int main() { 
    printf("HELLO\n");
    FILE *video = fopen("op.mp4", "rb"); // read binary
    //FILE *output = fopen("output.mp4", "wb"); // write binary
    //box* boxes = (box*) calloc(1000, sizeof(box));
    size_t bytesread;

    int sizeBuffer = 0;
    unsigned char *typeBuffer = (unsigned char*) malloc(4); 
    int cont = 1;
    while (cont == 1) { 
        char *temp = (char*) malloc(4);
        unsigned int x;
        fread(temp, 1, 4, video);

        for (int j = 0; j < 4; j++) {
            for (int i = 7; i >= 0; i--) {
                printf("%d", (temp[j] >> i) & 1);
            }
            printf(" ");
        }
        
        printf("\n");
        
        for (int j = 0; j < 4; j++) {
            for (int i = 7; i >= 0; i--) {
                x = x | (((temp[j] >> i) & 1) << (((4 - j)*7) + (i - 7)));
            }
        }

        /* int store;
        fread(&store, 4, 1, video);
        //fread_s(&store, sizeof(int), 4, 1, video);
        printf("%d\n", store);
        for (int i = 0; i < 32; i++) { 
            printf("%d", (store >> i) & 1);
        }
        printf("\n");

        int reverse = 0; 
        for (int i = 31; i >= 0; i--) { 
            printf("%d", (store >> i) & 1);
        }
        //reverse = reverse | (((store >> i) & 1) << (31 - i));
        printf("\n%d\n", reverse); */

        printf("%d\n", x);

        fread(typeBuffer, 4, 1, video);
        for (int i = 0; i < 4; i++) { 
            printf("%c", typeBuffer[i]);
        }
        printf("\n");

        char *dumpBuffer = (char*) malloc(x);
        bytesread = fread(dumpBuffer, x, 1, video);
        free(dumpBuffer);
        scanf("%d", &cont);
    }


    //sizeBuffer = (sizeBuffer ) ^ (*temp << (2*4));

    
    //printf("%d", sizeBuffer);
    //printf("\n");

    
    
    //fwrite(buffer, bytesread, 1, output);
    
}