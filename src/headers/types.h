
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
    unsigned int *boxSize;
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
    unsigned int *size;
    Node *head;
    Node *tail;
    Node *current;
} linkedList;



// Specialized MPEG-4 Structs //

/**
 * @brief Dinf box sub container
 */
typedef struct dataReference { 
    unsigned int *size; 
    char *type;
    char *version;
    char *flags;
    char *data;
} dataReference;


/**
 * @brief Sample boxes sub container
 */
typedef struct sampleDescription { 
    unsigned int *size; 
    char *dataFormat;
    char *reserved;
    unsigned short dataReferenceIndex;
} sampleDescription;