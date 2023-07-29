
// Preprocessor constants
#define BOX_HEADER_SIZE 8
#define BOX_HEADER_HALF_SIZE 4 // unused
#define BOX_SIZE_SIZE 4
#define BOX_LONG_SIZE_SIZE 8 // verify
#define BOX_TYPE_SIZE 4
#define BOX_VERSION_SIZE 1
#define BOX_FLAG_SIZE 3
#define TRUE 1
#define FALSE 0
#define DEBUG_PRINT TRUE


// renaming stdint types
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;


// General Structs //

/**
 * @brief Holds an MPEG-4 Box/Atom. 
 * @param *boxSize  -   4 Bytes for full size of Box/Atom including Header
 * @param *boxType  -   4 Bytes for 4 characters denoting type (no '\0' included)
 * @param *boxData  -   (boxSize - BOX_HEADER_SIZE) length char array for Box/Atom body
 *
 * @note "Header" refers to the boxSize and boxType of a Box/Atom.
 * @note boxData length is calculated with (boxSize - BOX_HEADER_SIZE).
 * @note boxType does NOT include the null terminator '\0'.
 */
typedef struct box { 
    u32 boxSize;
    u8 *boxType;
    u8 *boxData;
} box;


/**
 * @brief Holds a different type in currentItem and Node (same) type in nextNode
 * @param *currentItem  -   usually type of box
 * @param *nextNode     -   another node instance
 */
typedef struct Node { 
    void *currentItem;
    void *nextNode;
} Node;


/**
 * @brief Standard linked list implementation
 * @param *size     -   items in linkedlist
 * @param *head     -   first node
 * @param *tail     -   last node
 * @param *current  -   used for traversal to mimic "yeild"
 *
 * @note interface defined in linkedList.c/h
 */
typedef struct linkedList {
    u32 size;
    Node *head;
    Node *tail;
    Node *last;
    Node *current;
} linkedList;



// Specialized MPEG-4 Structs //



/**
 * @brief Sample boxes sub container
 */
typedef struct sampleDescription { 
    u32 size; 
    u8 *dataFormat;
    u8 *reserved;
    unsigned short dataReferenceIndex;
} sampleDescription;


/**
 * @brief unused. opted to have null terminated 
 * table entry struct arrays instead.
 */
/* typedef struct table { 
    u32 *numberOfEntries;
    void *tableArray;
} table; */

/**
 * @brief 
 * @param *trackDuration    -   duration of edit/track in movie timescale units
 * @param *mediaTime        -   the starting time within the media of the edit
 * in movie time scale units. this is -1 if the edit is empty. 
 * @param *mediaRate        -   realtive rate which to play the media corresponding
 * to this edit segment. Cannot be 0 or negative.
 */
typedef struct elstTable { 
    u32 totalEntries;
    u32 *trackDurationArr;
    i32 *mediaTimeArr;
    u32 *mediaRateArr;
} elstTable;



// Need to finalize the following types //
/**
 * @brief Dinf box sub container
 */
typedef struct dataReferenceTableEntry { 
    u32 size; 
    u8 *type;
    u8 *version;
    u8 *flags;
    u8 *data;
} dataReferenceTableEntry;


typedef struct sampleToChunkTable { 
    u32 totalEntries;
    u32 *firstChunkArr;
    u32 *samplesPerChunkArr;
    u32 *sampleDescriptionIdArr;
} sampleToChunkTable;


typedef struct chunkOffsetTable { 
    u32 totalEntries; 
    u32 *offsetArr;
} chunkOffsetTable;


typedef struct sampleSizeTable { 
    u32 totalEntries;
    u32 sampleSizeDefault;
    u32 *sizeArr;
} sampleSizeTable;



// Two main types of MPEG types //


// First type: Compressed Data
// Stored as array of type entries
typedef struct timeToSampleTableCompressed { 
    u32 totalEntries;
    u32 *sampleCountArr; 
    u32 *sampleDurationArr;
} timeToSampleTableCompressed;

typedef struct compositionOffsetTableCompressed { 
    u32 totalEntries;
    u32 *sampleCountArr;
    i32 *compositionOffsetArr;
} compositionOffsetTableCompressed;



// Second type: uncompressed Data
// Stored as a type containing arrays
typedef struct syncSampleTable { 
    u32 totalEntries;
    u32 *sampleNumberArr;
} syncSampleTable;


typedef struct timeToSampleTable { 
    u32 totalEntries;
    u32 *sampleDurationArr;
    u32 *sampleDeltaArr;
} timeToSampleTable;


typedef struct compositionOffsetTable { 
    u32 totalEntries;
    i32 *compositionOffsetArr;
} compositionOffsetTable;


typedef struct displayTimeToSampleTable { 
    u32 totalEntries;
    u32 *displayTimeArr;
    u32 *sampleNumberArr;
} displayTimeToSampleTable;






// AVC Structs //

typedef struct NALUnitInfo {
    u8 NALRefIdc;
    u8 NALUnitType;
    u32 NALUnitDataLength;
    u8 *NALUnitData;
} NALUnitInfo;

typedef struct picParameterSet {
    u32 picParameterSetId;
    u32 seqParameterSetId;
    u8 entropyCodingModeFlag;
    u8 picOrderPresentFlag;
    u32 numSliceGroupsMinus1;

    u32 sliceGroupMapType;
    u32 *runLengthMinus1;
    u32 *topLeft;
    u32 *bottomRight;
    u8 sliceGroupChangeDirectionFlag;
    u32 sliceGroupChangeRateMinus1;
    u32 picSizeInMapUnitsMinus1;
    u32 *sliceGroupId;

    u32 numRefIdxl0ActiveMinus1;
    u32 numRefIdxl1ActiveMinus1;
    u8 weightedPredFlag;
    u8 weightedBipredIdc;
    i32 picInitQpMinus26;
    i32 picInitQsMinus26;
    i32 chromaQpIndexOffset;
    u8 deblockingFilterVariablesPresentFlag;
    u8 constrainedIntraPredFlag;
    u8 redundantPicCntPresentFlag;
    u8 frameCroppingFlag;

    u32 frameCropLeftOffset;
    u32 frameCropRightOffset;
    u32 frameCropTopOffset;
    u32 frameCropBottomOffset;
} picParameterSet;


typedef struct seqParameterSet { 
    u8 profileIdc;
    u8 levelIdc;
    u8 moreThanOneSliceGroupAllowedFlag;
    u8 arbitrarySliceOrderAllowedFlag;
    u8 redundantPicturesAllowedFlag;
    u32 seqParameterSetId;
    u32 log2MaxFrameNumMinus4;
    u32 picOrderCntType;

    u32 log2MaxPicOrderCntLsbMinus4;
    u8 deltaPicOrderAlwaysZeroFlag;
    i32 offsetForNonRefPic;
    i32 offsetForTopToBottomField;
    u32 numRefFramesInPicOrderCntCycle;
    i32 *offsetForRefFrame;

    u32 numRefFrames;
    u8 requiredFrameNumUpdateBehaviourFlag;
    u32 picWidthInMbsMinus1;
    u32 picHeightInMapUnitsMinus1;
    u8 frameMbsOnlyFlag;

    u8 mbAdaptiveFrameFieldFlag;
    u8 direct8x8InferenceFlag;
    u8 vuiParametersPresentFlag;
} seqParameterSet;


/**
 * @brief picParameterSetId holds id corrosponding to each
 * picParameterSets entry for searching and sorting
 */
typedef struct picParameterSetTable {
    u32 numberOfEntries;
    picParameterSet **picParameterSetArr;
    u32 *picParameterSetIdArr;
} picParameterSetTable;


/**
 * @brief seqParameterSetId holds id corrosponding to each
 * seqParameterSets entry for searching and sorting
 */
typedef struct seqParameterSetTable { 
    u32 numberOfEntries;
    seqParameterSet **seqParameterSetArr;
    u32 *seqParameterSetIdArr;
} seqParameterSetTable;


typedef struct sliceHeader { 
    u32 firstMbInSlice;
    u32 sliceType;
    u32 picParameterSetId;
    u32 frameNum;
    u8 fieldPicFlag;
    u8 bottomFieldFlag;
    u32 idrPicId;
    u32 picOrderCntLsb;
    i32 deltaPicOrderCntBottom;
    i32 *deltaPicOrderCnt;
    u32 redundantPicCnt;
    u8 directSpatialMvPredFlag;
    u8 numRefIdxActiveOverrideFlag;
    u32 numRefIdx10ActiveMinus1;
    u32 numRefIdx11ActiveMinus1;
    u32 cabacInitIdc;
    i32 sliceQpDelta;
    u8 spForSwitchFlag;
    i32 sliceQsDelta;
    u32 disableDeblockingFilterIdc;
    i32 sliceAlphaC0OffsetDiv2;
    i32 sliceBetaOffsetDiv2;
    u32 sliceGroupChangeCycle;
} sliceHeader;

typedef struct AVC_Data {
    // avcc length of nal unit
    u8 lengthSizeMinus1;
    picParameterSetTable *picParamTable;
    seqParameterSetTable *seqParamTable;
} AVC_Data;

typedef struct NAL_Data {
    u32 placeholder;
} NAL_Data;


typedef struct parsingInfo { 
    u32 *bitsRead;
    u32 *bytesRead;
    u8 *data;
} parsingInfo;


// Main Storage Structs //

typedef struct sampleInfo {
    u32 realTime; // may remove 
    u32 mediaTime;
    u32 sampleNumber;
    u32 chunkNumber;
    u32 chunkOffset;
    u32 sampleSize;
    u32 sampleIndexInChunk;
    u32 sampleOffsetInChunk;
    u32 sampleOffsetInMdat;
    u8 *mdatPointer;
} sampleInfo;


/**
 * @brief 
 * @param *timescale            -   the number of time units that pass per second in 
 * this movies time coordinate system.
 * @param *fullDuration         -   the duration of the longest trak in the movie in
 * time scale units.
 * @param *videoTrackDuration   -   the duration of the video track in the movie in
 * movie time scale units. derived from the track's edts. equal to the sum of the 
 * durations of all the track's edits. if there is not elst then it's the sum of 
 * all sample durations converted into movie timescale.
 *
 */
typedef struct MPEG_Data {
    // 
    linkedList *topBoxesLL;

    // from mvhdParseBox
    u32 mvhdTimeScale; 
    u32 mvhdDuration;
    
    // from tkhdParseBox
    u32 tkhdTrackDuration;
    
    // from edtsParseBox->elstParseBox
    elstTable *elstTable;
    
    // from mdhdParseBox
    u32 mdhdTimeScale;
    u32 mdhdDuration;

    // from dinfParseBox->drefParseBox
    dataReferenceTableEntry **dataReferenceTable;

    // from sttsParseBox
    timeToSampleTableCompressed *timeToSampleTableCompressed;
    timeToSampleTable *timeToSampleTable;

    // from stscParseBox
    sampleToChunkTable *sampleToChunkTable;

    // from stcoParseBox
    chunkOffsetTable *chunkOffsetTable;

    // from stszParseBox
    sampleSizeTable *sampleSizeTable;
    u32 numberOfSamples; 

    // from stssParseBox
    syncSampleTable *syncSampleTable;

    // from cttsParseBox
    compositionOffsetTableCompressed *compositionOffsetTableCompressed;
    compositionOffsetTable *compositionOffsetTable;

    // mdat Data
    box *mdatBox;
    u32 mdatDataOffsetInFile;

    // 
    displayTimeToSampleTable *displayTimeToSampleTable;

    // avcc length of nal unit
    AVC_Data *avcData;
    

} MPEG_Data;