
void parseAVCSample(sampleInfo *sampleData, MPEG_Data *videoData);
NALUnitInfo *parseNALUnit(u32 NALUnitDataLength, u8 *NALDataStream);

seqParameterSet *seqParameterSetRbspDecode(u32 NALUnitDataLength, u8 *NALDataStream);
picParameterSet *picParameterSetRbspDecode(u32 NALUnitDataLength, u8 *NALDataStream);

u8 byteAligned(u32 *bitsRead);
void rbspTrailingBits(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 numberOfBits);


u32 u(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 numberOfBits);
u32 getUnsignedNBits(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 numberOfBits);

u32 ue(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 dataLength);
i32 se(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 dataLength);
u32 getCodeNum(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 dataLength);


