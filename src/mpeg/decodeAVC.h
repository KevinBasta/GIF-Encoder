
#ifndef DECODE_AVC_H
#define DECODE_AVC_H

void parseAVCSample(sampleInfo *sampleData, MPEG_Data *videoData);
NALUnitInfo *parseNALUnit(u32 NALUnitDataLength, u8 *NALDataStream);

seqParameterSet *seqParameterSetRbspDecode(u32 NALUnitDataLength, u8 *NALDataStream);
picParameterSet *picParameterSetRbspDecode(u32 NALUnitDataLength, u8 *NALDataStream);
void sliceHeaderDecode(u32 NALUnitDataLength, u8 *NALDataStream, sampleInfo *sample, MPEG_Data *videoData);

u8 byteAligned(u32 *bitsRead);
void rbspTrailingBits(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 numberOfBits);


u32 u(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 numberOfBits);
u32 getUnsignedNBits(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 numberOfBits);

u32 ue(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 dataLength);
i32 se(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 dataLength);
u32 getCodeNum(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 dataLength);

#endif // DECODE_AVC_H