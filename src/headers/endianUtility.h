
u32 bigEndianCharToLittleEndianUnsignedInt(u8 *bigEndianCharArray);
u32 *bigEndianCharToLittleEndianUnsignedIntHeap(u8 *bigEndianCharArray);

i32 bigEndianCharToLittleEndianInt(u8 *bigEndianCharArray);
i32 *bigEndianCharToLittleEndianIntHeap(u8 *bigEndianCharArray);

i32 bigEndianCharToLittleEndianGeneralized(u8 *bigEndianCharArray, u32 numberOfBytes);
i32 *bigEndianCharToLittleEndianGeneralizedHeap(u8 *bigEndianCharArray, u32 numberOfBytes);

u64 bigEndianCharBitsToLittleEndianGeneralized(u8 *bigEndianCharArray, u32 startingBit, u32 numberOfBits);
u64 simpleBigEndianToLittleEndianBits(u8 *bigEndianCharArray, u32 startingBit, u32 endingBit, u32 numberOfBits);

/**
 * @brief u64val is where the littleEndian int is writte.
 * all types can be read for type conversion.
 */
union littleEndianInteger { 
    u8  u8val;
    u16 u16val;
    u32 u32val;
    u64 u64val;

    i8  i8val;
    i16 i16val;
    i32 i32val;
    i64 i64val;
};

union littleEndianInteger bigEndianIntegerStoredInCharArrayToLittleEndianGeneralizedInteger(u8 *bigEndianCharArray, u32 numberOfBytes);
// u8-u64 stack ints
u8 bigEndianU8ArrToLittleEndianU8(u8 *bigEndianU8Arr);
u16 bigEndianU8ArrToLittleEndianU16(u8 *bigEndianU8Arr);
u32 bigEndianU8ArrToLittleEndianU32(u8 *bigEndianU8Arr);
u64 bigEndianU8ArrToLittleEndianU64(u8 *bigEndianU8Arr);

// u8-u64 heap ints
u8 *bigEndianU8ArrToLittleEndianU8Heap(u8 *bigEndianU8Arr);
u16 *bigEndianU8ArrToLittleEndianU16Heap(u8 *bigEndianU8Arr);
u32 *bigEndianU8ArrToLittleEndianU32Heap(u8 *bigEndianU8Arr);
u64 *bigEndianU8ArrToLittleEndianU64Heap(u8 *bigEndianU8Arr);

// i8-i64 stack ints
i8 bigEndianU8ArrToLittleEndianI8(u8 *bigEndianU8Arr);
i16 bigEndianU8ArrToLittleEndianI16(u8 *bigEndianU8Arr);
i32 bigEndianU8ArrToLittleEndianI32(u8 *bigEndianU8Arr);
i64 bigEndianU8ArrToLittleEndianI64(u8 *bigEndianU8Arr);

// i8-i64 heap ints
i8 *bigEndianU8ArrToLittleEndianI8Heap(u8 *bigEndianU8Arr);
i16 *bigEndianU8ArrToLittleEndianI16Heap(u8 *bigEndianU8Arr);
i32 *bigEndianU8ArrToLittleEndianI32Heap(u8 *bigEndianU8Arr);
i64 *bigEndianU8ArrToLittleEndianI64Heap(u8 *bigEndianU8Arr);
