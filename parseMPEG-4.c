#include <stdio.h>
#include <stdlib.h>


// Assumes little endian
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}


/*
    prints binary representation of a 4 byte char array

    bitPattern: a 4 byte char array
*/
void printCharArrayBits(char *bitPattern) { 
    for (int j = 0; j < 4; j++) {
        for (int i = 7; i >= 0; i--) {
            printf("%d", (bitPattern[j] >> i) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

int main() { 
    FILE *video = fopen("op.mp4", "rb");
    size_t chunksread;

    int readmore = 1;
    while (readmore != 0) { 
        // reading and storing the size of a box
        char *headerSize = (char*) malloc(4);
        chunksread = fread_s(&headerSize[0], 4, 4, 1, video);
        printCharArrayBits(headerSize);

        // reading and storing the type of a box
        char *headerType = (char*) malloc(4);
        chunksread = fread_s(&headerType[0], 4, 4, 1, video);
                    
        // translating the headerSize binary into an integer
        unsigned int sizeCheck = 0;        
        for (int headerByte = 0; headerByte < 4; headerByte++) {
            for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
                /*
                    currentBit (headerSize[headerByte] >> bitInHeaderByte) & 1
                    shifts the desired bit to the lsb and masks it with the binary
                    representation of 1

                    bitOffset (((3-headerByte)*8) + bitInHeaderByte) maps the position
                    of a bit in a byte to it's position in a 4 byte binary representation

                    if the currentBit is 1, then will set that bit in the integer number
                */
                int currentBit = (headerSize[headerByte] >> bitInHeaderByte) & 1;
                int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);

                // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

                if (currentBit == 1) {
                    sizeCheck = sizeCheck | (currentBit << bitOffset);
                }
            }
        }
        printBits(sizeof(int), &sizeCheck);

        int realSize = sizeCheck - 8;
        printf("%u\n", sizeCheck);
        printf("%c", headerType[0]);
        printf("%c", headerType[1]);
        printf("%c", headerType[2]);
        printf("%c\n", headerType[3]);
        
        
        //char *extendedHeader = (char*) malloc(16);
        char *boxBody = (char*) malloc(realSize);
        chunksread = fread(boxBody, realSize, 1, video);
        free(boxBody);

        if (feof(video)) {
            printf("end of file reached\n");
            break;
        }


        scanf("%d", &readmore);
        if (readmore == 2) { 
            break;
        }
        
    }

}