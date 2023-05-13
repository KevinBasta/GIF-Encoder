unsigned int *charToInt(char *integerAsCharArray);
unsigned short *bigEndianCharToLittleEndianUnsignedShort(char *bigEndianCharArray);
unsigned int *bigEndianCharToLittleEndianUnsignedInt(char *bigEndianCharArray, int numberOfBytes);
int *bigEndianCharToLittleEndianBytedInt(char *bigEndianCharArray, int numberOfBytes);
float *bigEndianCharToLittleEndianFloat(char *bigEndianCharArray);
short *bigEndianCharToLittleEndianShort(char *bigEndianCharArray);
int *bigEndianCharToLittleEndianInt(char *bigEndianCharArray);
int compareNBytes(char *firstItem, char secondItem[], int numberOfBytes);
char *copyNBytes(int numberOfBytes, char *originalData, unsigned int *byteOffset);
char *referenceNBytes(int numberOfBytes, char *originalData, unsigned int *byteOffset);
