
// Preprocessor constants
#define BOX_HEADER_SIZE 8
#define BOX_HEADER_HALF_SIZE 4
#define TRUE 1
#define FALSE 0
#define VERSION_SIZE 1
#define FLAG_SIZE 3


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
    unsigned int boxSize;
    char *boxType;
    char *boxData;
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
    unsigned int size;
    Node *head;
    Node *tail;
    Node *current;
} linkedList;



// Specialized MPEG-4 Structs //



/**
 * @brief Sample boxes sub container
 */
typedef struct sampleDescription { 
    unsigned int *size; 
    char *dataFormat;
    char *reserved;
    unsigned short dataReferenceIndex;
} sampleDescription;


/**
 * @brief unused. opted to have null terminated 
 * table entry struct arrays instead.
 */
/* typedef struct table { 
    unsigned int *numberOfEntries;
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
typedef struct elstTableEntry { 
    unsigned int trackDuration;
    int mediaTime;
    unsigned int mediaRate;
} elstTableEntry;


/**
 * @brief Dinf box sub container
 */
typedef struct dataReferenceTableEntry { 
    unsigned int size; 
    char *type;
    char *version;
    char *flags;
    char *data;
} dataReferenceTableEntry;


typedef struct timeToSampleTableEntry { 
    unsigned int sampleCount; 
    unsigned int sampleDuration;
} timeToSampleTableEntry;


typedef struct sampleToChunkTableEntry { 
    unsigned int firstChunk;
    unsigned int samplesPerChunk;
    unsigned int sampleDescriptionId;
} sampleToChunkTableEntry;




// Main Storage Structs //

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
    // from mvhdParseBox
    unsigned int mvhdTimeScale; 
    unsigned int mvhdDuration;
    
    // from tkhdParseBox
    unsigned int tkhdTrackDuration;
    
    // from edtsParseBox->elstParseBox
    elstTableEntry **elstTable;
    
    // from mdhdParseBox
    unsigned int mdhdTimeScale;
    unsigned int mdhdDuration;

    // from dinfParseBox->drefParseBox
    dataReferenceTableEntry **dataReferenceTable;

    // from sttsParseBox
    timeToSampleTableEntry **timeToSampleTable;
    unsigned int numberOfSamples; // unsure if want to keep here or make pointer

    // from stscParseBox
    sampleToChunkTableEntry **sampleToChunkTable;


} MPEG_Data; 