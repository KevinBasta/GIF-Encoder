
void readMainBoxes(char fileName[], linkedList *list);
void parseChildBoxes(box *parentBox, linkedList *list);
void parseNestedChildBoxes(char *boxData, unsigned int *bytesRead, unsigned int endIndex, linkedList *list);
box *parseSingleNestedChildBox(char *boxData, unsigned int *bytesRead);
void ftypParseBox(box *ftypBox);
void mvhdParseBox(box *mvhdBox, MPEG_Data *videoData);
void tkhdParseBox(box *trakBox, MPEG_Data *videoData);
void elstParseBox(box *elstBox, MPEG_Data *videoData);
void edtsParseBox(box *edtsBox, MPEG_Data *videoData);
void mdhdParseBox(box *mdhdBox, MPEG_Data *videoData);
char *hdlrParseBox(box *hdlrBox);
void vmhdParseBox(box *vmhdBox);
void drefParseBox(box *drefBox, MPEG_Data *videoData);
void dinfParseBox(box *dinfBox, MPEG_Data *videoData);
box *getVideTrak(linkedList *moovLL);

void stsdParseBox(box *stsdBox);
void stszParseBox(box *stszBox, MPEG_Data *videoData);
void stscParseBox(box *stscBox, MPEG_Data *videoData);
void stcoParseBox(box *stcoBox, MPEG_Data *videoData);
void sttsParseBox(box *sttsBox, MPEG_Data *videoData);
void stssParseBox(box *stssBox, MPEG_Data *videoData);
void cttsParseBox(box *cttsBox, MPEG_Data *videoData);
void videoMediaBox();