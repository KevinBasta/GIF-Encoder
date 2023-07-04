
void parseAVCSample(sampleInfo *sampleData, MPEG_Data *videoData);
void parseNALUnit(u32 NALUnitLength, u8 *NALUnit);
void picParameterSetRbsp(NALUnitInfo *NALUnit);
u32 getUnsignedNBits(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 numberOfBits);
u32 getCodeNum(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 dataLength);
