
u32 realTimeToMediaTime(u32 time, u32 newTimeScale);
u32 mediaTimeToSampleNumber(u32 mediaTime, timeToSampleTableEntry **timeToSampleTable);
u32 sampleNumberToChunkNumber(sampleInfo *sample, sampleToChunkTableEntry **sampleToChunkTable, u32 numberOfSamples, sampleSizeTableEntry **sampleSizeTable, u32 sampleSizeDefault);
u32 sampleNumberToSampleSize(u32 sampleNumber, u32 sampleSizeDefault, sampleSizeTableEntry **sampleSizeTable);
u32 getSampleOffsetInChunk(sampleInfo *sample, u32 sampleSizeDefault, sampleSizeTableEntry **sampleSizeTable);
u32 offsetDataToSampleMdatOffset(u32 chunkOffset, u32 sampleOffsetInChunk, u32 mdatOffsetInFile);

void sampleRealTimeToMediaTime(sampleInfo *sample, MPEG_Data *videoData);
void sampleMediaTimeToSampleNumber(sampleInfo *sample, MPEG_Data *videoData);
void sampleSampleNumberToChunkNumber(sampleInfo *sample, MPEG_Data *videoData);
void sampleChunkNumberToChunkOffset(sampleInfo *sample, MPEG_Data *videoData);
void sampleSampleNumberToSampleSize(sampleInfo *sample, MPEG_Data *videoData);
void sampleOffsetDataToSampleMdatOffset(sampleInfo *sample, MPEG_Data *videoData);

void getVideoDataRange(u32 startTime, u32 endTime, MPEG_Data *videoData);
sampleInfo *sampleSearchByTime(u32 time, MPEG_Data *videoData);
sampleInfo *sampleSearchBySampleNumber(u32 sampleNumber, MPEG_Data *videoData);
sampleInfo *keyFrameSearch(u32 time, MPEG_Data *videoData);