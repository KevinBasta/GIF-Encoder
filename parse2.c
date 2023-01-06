#include <stdio.h>
#include <stdlib.h>

int main() { 
    FILE *video = fopen("op.mp4", "rb");
    size_t chunksread;

    int readmore = 1;
    while (readmore != 0) { 
        // reading and storing 64 bits (8 bytes * 8 bits)
        char *header = (char*) malloc(8);
        chunksread = fread_s(&header[0], 8, 8, 1, video); //printf("%zu\n", chunksread);
        
        // taking the first 32 bits (4 bytes) and checking
        // if they are 0, 1, or a different number
        unsigned int sizeCheck = 0;
        
        for (int headerByte = 0; headerByte < 4; headerByte++) {
            for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
                //x = x | (((temp[j] >> i) & 1) << (((4 - j)*7) + (i - 7)));

                /*
                    first part (header[headerByte] >> bitInHeaderByte) & 1)
                    shifts the desired bit to the lsb and masks it with the binary
                    representation of 1

                    second part << (((3 - headerByte)*7) + (bitInHeaderByte - 7))
                    shifts the first part into the appropriate position in the integer
                    bit pattern. ((4 - headerByte)*7) gets the current byte in the integer.
                    Example: if headerByte is 0, we are taking the first byte of the 4 byte
                    binary. (3-0)*7 ensurs that the first part is being shifted left 
                    enought to get to the first byte position in the integer binary.
                    + bitInHeaderByte just adds the bit number to the position so far. 
                    Example cont: if bitInHeaderByte is 4 then (3-0)*7 + 4 will give

                */
                sizeCheck = sizeCheck | ((header[headerByte] >> bitInHeaderByte) << (((3-headerByte)*8) + bitInHeaderByte) );
                printf("%d", (sizeCheck >> (((3-headerByte)*8) + bitInHeaderByte)) & 1);
            }
            printf(" ");
        }
        printf("\n");
        int realSize = sizeCheck - 8;
        printf("%d\n", sizeCheck);
        
        //char *extendedHeader = (char*) malloc(16);
        char *boxBody = (char*) malloc(realSize);
        chunksread = fread(boxBody, realSize, 1, video);
        free(boxBody);

        scanf("%d", &readmore);
        if (readmore == 2) { 
            break;
        }
    }

}