#ifndef COMMON_TYPES
    #define COMMON_TYPES
    #include "main.h"
    #include "typesMPEG-4.h"
    #include "typesAVC.h"
    #include "typesStorage.h"
    #include "typesUtility.h"
#endif //COMMON_TYPES

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

linkedList* initLinkedList();
void appendNodeLinkedList(linkedList *list, void *item);
void nullifyLastNodeLinkedList(linkedList *list);

void printAllBoxesLinkedList(linkedList *list);

u32 getOffsetToBoxLinkedList(linkedList *list, u8 boxCompareType[]);
box *getBoxFromLinkedList(linkedList *list, u8 boxReturnType[]);

void freeBox(box *boxStruct);
Node *freeBoxNode(Node *nodeStruct);
void freeLinkedList(linkedList *list, char type[]);
