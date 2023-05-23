
unsigned int bigEndianCharToLittleEndianUnsignedInt(char *bigEndianCharArray);
unsigned int *bigEndianCharToLittleEndianUnsignedIntHeap(char *bigEndianCharArray);

int bigEndianCharToLittleEndianInt(char *bigEndianCharArray);
int *bigEndianCharToLittleEndianIntHeap(char *bigEndianCharArray);

//void bigEndianCharToLittleEndianGeneralized(char *bigEndianCharArray, int numberOfBytes);
//void *bigEndianCharToLittleEndianGeneralizedHeap(char *bigEndianCharArray, int numberOfBytes);

//unsigned short *bigEndianCharToLittleEndianUnsignedShort(char *bigEndianCharArray);
//int *bigEndianCharToLittleEndianBytedInt(char *bigEndianCharArray, int numberOfBytes);
//float *bigEndianCharToLittleEndianFloat(char *bigEndianCharArray);
//short *bigEndianCharToLittleEndianShort(char *bigEndianCharArray);


int compareNBytes(char *firstItem, char secondItem[], int numberOfBytes);
char *copyNBytes(int numberOfBytes, char *originalData, unsigned int *byteOffset);
char *referenceNBytes(int numberOfBytes, char *originalData, unsigned int *byteOffset);

unsigned int *charToUnsignedInt(char *integerAsCharArray);
int *charToInt(char *integerAsCharArray);