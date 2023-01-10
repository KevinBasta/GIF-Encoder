#include <stdio.h>
#include <stdlib.h>

// The following definitions are in bytes
#define BOX_HEADER_SIZE 8
#define BOX_HEADER_HALF_SIZE 4


// function signatures

void printBits(size_t const size, void const * const ptr);
void printCharArrayBits(char *bitPattern);
void printNBytes(char *string, int bytesToPrint, char prefixString[], char postfixString[]);
void printHexNBytes(char *string, int bytesToPrint);

int boxTypeEqual(char *boxType, char stringType[]);

Node *readMainBoxes(char fileName[]);
void ftypReadBox(box *ftypBox);
void moovReadBox(box *moovBox);


// general box struct
typedef struct box { 
    unsigned int *boxSize;
    char *boxType;
    char *boxData;
} box;

// box node struct
typedef struct Node { 
    struct box *currentBox;
    struct Node *nextBoxNode;
} Node;



int main() { 
    Node *headNode = readMainBoxes("op.mp4");

    printf("traversing:\n");
    for (Node *traverseNode = headNode; traverseNode != NULL; traverseNode = traverseNode->nextBoxNode) {
        printNBytes(traverseNode->currentBox->boxType, 4, "box type: ", "\t");
        printf("box size: %10u\n", *(traverseNode->currentBox->boxSize));
        if (boxTypeEqual(traverseNode->currentBox->boxType, "moov") == 1) {
            moovReadBox(traverseNode->currentBox);
        }
    }

    //printf("%d\n", boxTypeEqual(headNode->currentBox->boxType, "ftyp"));
    //ftypReadBox(headNode->currentBox);
    
    printf("end of script\n");
}


/**
 *  reads a moov box's first layer boxes
 *  @param *moovBox:    a pointer to a moov box struct
 */
void moovReadBox(box *moovBox) {
    unsigned int boxSize = *(moovBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = moovBox->boxData;

    unsigned int bytesRead = 0;
    while (bytesRead < boxDataSize) { 
        char *childBoxHeaderSize = (char*) malloc(BOX_HEADER_HALF_SIZE);
        for (int i = 0; i < BOX_HEADER_HALF_SIZE; i++) {
            childBoxHeaderSize[i] = boxData[bytesRead];
            bytesRead += 1;
        }

        char *childBoxHeaderType = (char*) malloc(BOX_HEADER_HALF_SIZE);
        for (int i = 0; i < BOX_HEADER_HALF_SIZE; i++) {
            childBoxHeaderType[i] = boxData[bytesRead];
            bytesRead += 1;
        }

        unsigned int *fullChildBoxSize = (unsigned int*) malloc(sizeof(unsigned int));
        *fullChildBoxSize = 0;
        for (int headerByte = 0; headerByte < 4; headerByte++) {
            for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
                int currentBit = (childBoxHeaderSize[headerByte] >> bitInHeaderByte) & 1;
                int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);

                if (currentBit == 1) {
                    *fullChildBoxSize = *fullChildBoxSize | (currentBit << bitOffset);
                }
            }
        }
        free(childBoxHeaderSize);

        printf("%u\t", *fullChildBoxSize);
        printNBytes(childBoxHeaderType, 4,"", "\n");

        int childBoxDataSize = *fullChildBoxSize - 8;
        char *childBoxBody = (char*) malloc(childBoxDataSize);
        for (int i = 0; i < childBoxDataSize; i++) {
            childBoxBody[i] = boxData[bytesRead];
            bytesRead += 1;
        }
    }
}


/**
 *  reads a ftyp box's major brand, minor version, and compatible brands
 *  @param *ftypBox:    a pointer to a ftyp box struct
 */
void ftypReadBox(box *ftypBox) { 
    unsigned int boxSize = *(ftypBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = ftypBox->boxData;

    /*
        reading the ftyp fields
        majorBrand: 4 bytes the preferred brand or best use
        minorVersion: 4 bytes indicates the file format specification version
        compatibleBrands[]: an array of 4 byte compatible file formats
    */
    unsigned int bytesRead = 0;
    char *majorBrand = (char*) malloc(4);
    for (int i = 0; i < 4; i++) {
        majorBrand[i] = boxData[bytesRead];
        bytesRead += 1;
    }
    printNBytes(majorBrand, 4, "", "\n");

    char *minorVersion = (char*) malloc(4);
    for (int i = 0; i < 4; i++) {
        minorVersion[i] = boxData[bytesRead];
        bytesRead += 1;
    }
    printHexNBytes(minorVersion, 4);

    unsigned int compatibleBrandsSize = boxDataSize - (bytesRead + 1);
    char *compatibleBrands = (char*) malloc(compatibleBrandsSize);
    
    int compatibleBrandsByte = 0;
    while (bytesRead < boxDataSize) {
        compatibleBrands[compatibleBrandsByte] = boxData[bytesRead];
        bytesRead += 1;
        compatibleBrandsByte += 1;
    }

    for (int i = 0; i < compatibleBrandsSize; i+=4) {
        printNBytes(&compatibleBrands[i], 4, "", "\n");
    }
}


/* void readChildBox(box *Box) { 
    unsigned int boxSize = *(Box->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char boxName[] = {Box->boxType[0], Box->boxType[1], Box->boxType[2], Box->boxType[3], '\0'};
    char *boxData = Box->boxData;

    unsigned int bytesRead = 0;
    while (bytesRead <= boxDataSize) { 
        char *childBoxHeaderSize = (char*) malloc(BOX_HEADER_HALF_SIZE);
        for (int i = 0; i < BOX_HEADER_HALF_SIZE; i++) {
             childBoxHeaderSize[i] = boxData[bytesRead];
             bytesRead += 1;
        }

        char *childBoxHeaderType = (char*) malloc(BOX_HEADER_HALF_SIZE);
        for (int i = 0; i < BOX_HEADER_HALF_SIZE; i++) {
             childBoxHeaderType[i] = boxData[bytesRead];
             bytesRead += 1;
        }

        unsigned int *fullBoxSize = (unsigned int*) malloc(sizeof(unsigned int));
        *fullBoxSize = 0;
        for (int headerByte = 0; headerByte < 4; headerByte++) {
            for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
                int currentBit = (childBoxHeaderSize[headerByte] >> bitInHeaderByte) & 1;
                int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);

                if (currentBit == 1) {
                    *fullBoxSize = *fullBoxSize | (currentBit << bitOffset);
                }
            }
        }
        free(childBoxHeaderSize);

        printf("%u\n", *fullBoxSize);
        printNBytes(childBoxHeaderType, 4,"", "");
    } 

} */


/**
 *  reads the top level boxes in an MPEG-4 binary file format
 *  @param fileName:    a character array (includes '\0'), a valid 
 *                      path to an mp4 file that exists
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


/**
 *  prints n bytes from a char array.
 *  mainly used to print the 4 bytes of a box type since '\0' is not stored
 *  @param *string:         a pointer to first element in a character array
 *  @param bytesToPrint:    the number of bytes to print from a char array
 *  @param prefixString:    a char array to print before
 *  @param postfixString:   a char array to print after
 */
void printNBytes(char *string, int bytesToPrint, char prefixString[], char postfixString[]) { 
    printf(prefixString);
    for (int i = 0; i < bytesToPrint; i++) { 
        printf("%c", string[i]);
    }
    printf(postfixString);
}


/**
 *  prints hex representation of an n byte char array
 *  @param *string:         a pointer to first element in a character array
 *  @param bytesToPrint:    the number of bytes to print from a char array
 */
void printHexNBytes(char *string, int bytesToPrint) { 
    // 15 == (0000 1111), masks the low 4 bits in a byte
    for (int i = 0; i < bytesToPrint; i++) {
        printf("%X", (string[i] >> 4) & 15);
        printf("%X ", string[i] & 15);
    }
    printf("\n");
}


/**
 *  compares the 4 byte box type to a string
 *  @param *boxType:    4 byte char array pointer
 *  @param stringType:  5 byte char array (includes '\0')
 */
int boxTypeEqual(char *boxType, char stringType[]) { 
    //char boxTypeStringArray[] = {boxType[0], boxType[1], boxType[2], boxType[3], '\0'};
    
    int isEqual = 1;
    for (int i = 0; i < 4; i++) {
        if (boxType[i] != stringType[i]) {
            isEqual = 0;
            break;
        }
    }

    return isEqual;
}


/**
 *  prints binary bits of a 4 byte char array
 *  @param bitPattern:  a 4 byte char array
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


/**
 *  prints the binary bits for any type passed
 *  @param size:    the number of bytes to print
 *  @param ptr:     a pointer to an element of any type
 */
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