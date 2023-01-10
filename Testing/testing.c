#include <stdio.h>

#define BUFFERSIZE 8

typedef struct box { 
    int size;
    char name[4];
} box;


int main() { 
    FILE *video = fopen("op.mp4", "rb");
    FILE *output = fopen("output.mp4", "wb");

    for(;;) { 
        char buffer[BUFFERSIZE];
        size_t bytesread = fread(buffer, 1, sizeof buffer, video);

        for (int i = 0; buffer[i] != '\0'; i++) {
            buffer[i] = buffer[i];
        }

        if (bytesread == 0) { 
            break;
        }

        fwrite(buffer, bytesread, 1, output);
    }
    
}