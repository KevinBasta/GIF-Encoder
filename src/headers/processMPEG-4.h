
unsigned int realTimeToMediaTime(unsigned int time, unsigned int newTimeScale);
unsigned int mediaTimeToSampleNumber(unsigned int mediaTime, timeToSampleTableEntry **timeToSampleTable);
unsigned int sampleNumberToChunkNumber(sampleInfo *sample, MPEG_Data *videoData);
unsigned int sampleNumberToSampleSize(unsigned int sampleNumber, unsigned int sampleSizeDefault, sampleSizeTableEntry **sampleSizeTable);
unsigned int getSampleOffsetInChunk(sampleInfo *sample, unsigned int sampleSizeDefault, sampleSizeTableEntry **sampleSizeTable);
void sampleSearch(unsigned int time, MPEG_Data *videoData);
