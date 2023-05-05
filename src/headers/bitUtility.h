unsigned int *charToInt(char *integerAsCharArray);
unsigned short *bigEndianCharToLittleEndianUnsignedShort(char *bigEndianCharArray);
unsigned int *bigEndianCharToLittleEndianUnsignedInt(char *bigEndianCharArray, int numberOfBytes);
short *bigEndianCharToLittleEndianShort(char *bigEndianCharArray);
int *bigEndianCharToLittleEndianInt(char *bigEndianCharArray);
int compareNBytes(char *firstItem, char secondItem[], int numberOfBytes);
char *copyNBytes(int numberOfBytes, char *originalData, unsigned int *byteOffset);
char *referenceNBytes(int numberOfBytes, char *originalData, unsigned int *byteOffset);
