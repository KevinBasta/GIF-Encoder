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
    char *typeBuffer = (char*) malloc(4); 
    int cont = 1;
    while (cont == 1) { 
        char *temp = (char*) malloc(4);
        int x;
        fread(&temp[3], 1, 1, video);
        fread(&temp[2], 1, 1, video);
        fread(&temp[1], 1, 1, video);
        fread(&temp[0], 1, 1, video);
        for (int i = 32; i >= 0; i--) { 
            printf("%d", (*temp >> i) & 1);
        }
        printf("\n");
        
        for (int i = 32; i >= 0; i--) { 
            x = x | (((*temp >> i) & 1) << i);
        }
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