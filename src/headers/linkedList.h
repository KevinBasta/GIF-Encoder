typedef struct box { 
    unsigned int *boxSize;
    char *boxType;
    char *boxData;
} box;

typedef struct Node { 
    void *currentItem;
    void *nextNode;
} Node;

typedef struct linkedList {
    unsigned int *size;
    Node *head;
    Node *tail;
    Node *current;
} linkedList;

linkedList* initLinkedList();
void appendNodeLinkedList(linkedList *list, void *item);
void nullifyLastNodeLinkedList(linkedList *list);
void printAllBoxesLinkedList(linkedList *list);
box *getBoxFromLinkedList(linkedList *list, char boxReturnType[]);
void freeBox(box *boxStruct);
void freeNode(Node *nodeStruct);