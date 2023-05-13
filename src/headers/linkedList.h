#include "types.h"

linkedList* initLinkedList();
void appendNodeLinkedList(linkedList *list, void *item);
void nullifyLastNodeLinkedList(linkedList *list);
void printAllBoxesLinkedList(linkedList *list);
box *getBoxFromLinkedList(linkedList *list, char boxReturnType[]);
void freeBox(box *boxStruct);
void freeNode(Node *nodeStruct);