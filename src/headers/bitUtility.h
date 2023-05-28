
u32 bigEndianCharToLittleEndianUnsignedInt(u8 *bigEndianCharArray);
u32 *bigEndianCharToLittleEndianUnsignedIntHeap(u8 *bigEndianCharArray);

i32 bigEndianCharToLittleEndianInt(u8 *bigEndianCharArray);
i32 *bigEndianCharToLittleEndianIntHeap(u8 *bigEndianCharArray);

//void bigEndianCharToLittleEndianGeneralized(u8 *bigEndianCharArray, u32 numberOfBytes);
//void *bigEndianCharToLittleEndianGeneralizedHeap(u8 *bigEndianCharArray, u32 numberOfBytes);

//unsigned short *bigEndianCharToLittleEndianUnsignedShort(u8 *bigEndianCharArray);
//i32 *bigEndianCharToLittleEndianBytedInt(u8 *bigEndianCharArray, u32 numberOfBytes);
//float *bigEndianCharToLittleEndianFloat(u8 *bigEndianCharArray);
//short *bigEndianCharToLittleEndianShort(u8 *bigEndianCharArray);


i32 compareNBytes(u8 *firstItem, u8 secondItem[], u32 numberOfBytes);
u8 *copyNBytes(u32 numberOfBytes, u8 *originalData, u32 *byteOffset);
u8 *referenceNBytes(u32 numberOfBytes, u8 *originalData, u32 *byteOffset);

u32 *charToUnsignedInt(u8 *integerAsCharArray);
i32 *charToInt(u8 *integerAsCharArray);