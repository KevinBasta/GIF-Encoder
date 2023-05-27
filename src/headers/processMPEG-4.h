
unsigned int realTimeToMediaTime(unsigned int time, unsigned int newTimeScale);
unsigned int mediaTimeToSampleNumber(unsigned int mediaTime, timeToSampleTableEntry **timeToSampleTable);
unsigned int sampleNumberToChunkNumber(sampleInfo *sample, sampleToChunkTableEntry **sampleToChunkTable, unsigned int numberOfSamples, sampleSizeTableEntry **sampleSizeTable, unsigned int sampleSizeDefault);
unsigned int sampleNumberToSampleSize(unsigned int sampleNumber, unsigned int sampleSizeDefault, sampleSizeTableEntry **sampleSizeTable);
unsigned int getSampleOffsetInChunk(sampleInfo *sample, unsigned int sampleSizeDefault, sampleSizeTableEntry **sampleSizeTable);
unsigned int offsetDataToSampleMdatOffset(unsigned int chunkOffset, unsigned int sampleOffsetInChunk, unsigned int mdatOffsetInFile);

void sampleRealTimeToMediaTime(sampleInfo *sample, MPEG_Data *videoData);
void sampleMediaTimeToSampleNumber(sampleInfo *sample, MPEG_Data *videoData);
void sampleSampleNumberToChunkNumber(sampleInfo *sample, MPEG_Data *videoData);
void sampleChunkNumberToChunkOffset(sampleInfo *sample, MPEG_Data *videoData);
void sampleSampleNumberToSampleSize(sampleInfo *sample, MPEG_Data *videoData);
void sampleOffsetDataToSampleMdatOffset(sampleInfo *sample, MPEG_Data *videoData);

void getVideoDataRange(unsigned int startTime, unsigned int endTime, MPEG_Data *videoData);
sampleInfo *sampleSearchByTime(unsigned int time, MPEG_Data *videoData);
sampleInfo *sampleSearchBySampleNumber(unsigned int sampleNumber, MPEG_Data *videoData);
sampleInfo *keyFrameSearch(unsigned int time, MPEG_Data *videoData);