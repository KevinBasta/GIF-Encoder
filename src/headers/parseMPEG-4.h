
void readMainBoxes(u8 fileName[], linkedList *list);
void parseChildBoxes(box *parentBox, linkedList *list);
void parseNestedChildBoxes(u8 *boxData, u32 *bytesRead, u32 endIndex, linkedList *list);
box *parseSingleNestedChildBox(u8 *boxData, u32 *bytesRead);
void ftypParseBox(box *ftypBox);
void mvhdParseBox(box *mvhdBox, MPEG_Data *videoData);
void tkhdParseBox(box *trakBox, MPEG_Data *videoData);
void elstParseBox(box *elstBox, MPEG_Data *videoData);
void edtsParseBox(box *edtsBox, MPEG_Data *videoData);
void mdhdParseBox(box *mdhdBox, MPEG_Data *videoData);
u8 *hdlrParseBox(box *hdlrBox);
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

void paspParseBox(box *paspBox);
void btrtParseBox(box *btrtBox);
void colrParseBox(box *colrBox);
void avccParseBox(box *avccBox);