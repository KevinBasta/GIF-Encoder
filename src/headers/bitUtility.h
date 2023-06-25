
i32 compareNBytes(u8 *firstItem, u8 secondItem[], u32 numberOfBytes);
u8 *copyNBytes(u32 numberOfBytes, u8 *originalData, u32 *byteOffset);
u8 *referenceNBytes(u32 numberOfBytes, u8 *originalData, u32 *byteOffset);
u8 *checkNextNBytes(u32 numberOfBytes, u8 *originalData, u32 byteOffset);
u8 getNBits(u32 startBit, u32 endBit, u8 data);
u32 countBitsToFirstNonZero(u8 *originalData, u32 boundry);
