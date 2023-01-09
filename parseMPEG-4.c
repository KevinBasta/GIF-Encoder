#include <stdio.h>
#include <stdlib.h>

// The following definitions are in bytes
#define BOX_HEADER_SIZE 8
#define BOX_HEADER_HALF_SIZE 4


// structures for storing information
typedef struct box { 
    unsigned int *boxSize;
    char *boxType;
    char *boxData;
} box;

typedef struct Node { 
    struct box *currentBox;
    struct Node *nextBoxNode;
} Node;


// function signatures
void printBits(size_t const size, void const * const ptr);
void printCharArrayBits(char *bitPattern);
Node *readMainBoxes(char fileName[]);


int main() { 
    Node *headNode = readMainBoxes("op.mp4");

    printf("traversing:\n");
    for (Node *traverseNode = headNode; traverseNode != NULL; traverseNode = traverseNode->nextBoxNode) {
        printf("box type: %5s\t", traverseNode->currentBox->boxType);
        printf("box size: %10u\n", *(traverseNode->currentBox->boxSize));
    }

    printf("end of script\n");
}


/*
    reads the top level boxes in an MPEG-4 binary file format

    fileName: a character array (includes '\0'), a valid path to 
              an mp4 file that exists
*/
Node *readMainBoxes(char fileName[]) { 
    Node *headNode = (Node*) malloc(sizeof(Node));
    Node *currentNode = headNode;
    Node *lastNode;
    
    FILE *video = fopen(fileName, "rb");
    size_t chunksread;
    while (!feof(video)) { 
        // reading and storing the size of a box
        char *headerSize = (char*) malloc(BOX_HEADER_HALF_SIZE);
        chunksread = fread_s(&headerSize[0], BOX_HEADER_HALF_SIZE, BOX_HEADER_HALF_SIZE, 1, video);
        // DEBUG printCharArrayBits(headerSize);

        /*
            Checking if at the end of file after reading the size part of a box header
            this ensures that only 4 extra bytes are read if already at end of file
        */
        if (feof(video)) { // can also use if chunksread == 0
            free(headerSize);
            free(currentNode);
            lastNode->nextBoxNode = NULL;
            //DEBUG printf("end of file reached\n");
            break;
        }

        // reading and storing the type of a box
        char *headerType = (char*) malloc(BOX_HEADER_HALF_SIZE);
        chunksread = fread_s(&headerType[0], BOX_HEADER_HALF_SIZE, BOX_HEADER_HALF_SIZE, 1, video);
        
        // translating the headerSize binary into an integer
        unsigned int *fullBoxSize = (unsigned int*) malloc(sizeof(unsigned int));
        *fullBoxSize = 0;

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
                    *fullBoxSize = *fullBoxSize | (currentBit << bitOffset);
                }
            }
        }
        free(headerSize); // freeing the char malloc of size
        
        
        // DEBUG printBits(sizeof(int), fullBoxSize);
        // DEBUG printf("%u\n", *fullBoxSize);
        // DEBUG printf("%s\n", headerType);
        /* 
            FOR ADDING SUPPORT FOR BIGGER FILE SIZES WHEN THE 
            fullBoxSize is equal to 1, there is a field after the 
            type in the header that gives 8 bytes to store size
            char *extendedHeaderSize = (char*) malloc(8); 
        */

       
        /*
            fullBoxSize includes the size of the header 
            (box header: 8 bytes {[size -> 4 bytes] [type -> 4 bytes]})
            needs to be subtracted from fullBoxSize before the body of the box is read
        */
        int boxDataSize = *fullBoxSize - 8;
        char *boxBody = (char*) malloc(boxDataSize);
        chunksread = fread(boxBody, boxDataSize, 1, video);
        

        // Creating a box struct to store current box
        box *currentBoxRead = (box*) malloc(sizeof(box));
        currentBoxRead->boxSize = fullBoxSize;
        currentBoxRead->boxType = headerType;
        currentBoxRead->boxData = boxBody;

        /*
            Storing the current box in the current Node and allocating memory for the next box
            setting the last node to the current node 
            (this is for setting it's nextBoxNode to null incase there is no next node)
        */
        currentNode->currentBox = currentBoxRead;
        currentNode->nextBoxNode = (Node*) malloc(sizeof(Node));
        lastNode = currentNode;
        currentNode = currentNode->nextBoxNode;
    }

    return headNode;
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


// Assumes little endian
void printBits(size_t const size, void const * const ptr) {
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