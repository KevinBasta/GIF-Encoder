
linkedList* initLinkedList();
void appendNodeLinkedList(linkedList *list, void *item);
void nullifyLastNodeLinkedList(linkedList *list);
void printAllBoxesLinkedList(linkedList *list);
u32 getOffsetToBoxLinkedList(linkedList *list, u8 boxCompareType[]);
box *getBoxFromLinkedList(linkedList *list, u8 boxReturnType[]);
void freeBox(box *boxStruct);
Node *freeBoxNode(Node *nodeStruct);
void freeLinkedList(linkedList *list, u8 type[]);