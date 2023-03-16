#ifndef COMMON_HEAD
    #define COMMON_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    #include "printUtility.h"
    #include "bitUtility.h"
    #define BOX_HEADER_SIZE 8
    #define BOX_HEADER_HALF_SIZE 4
    #define TRUE 1
    #define FALSE 0
    
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
        int size;
        Node *head;
        Node *tail;
        Node *current;
    } linkedList;
#endif



linkedList* initLinkedList() { 
    linkedList *newLinkedList = (linkedList*) malloc(sizeof(linkedList));
    newLinkedList->size = 0;
    newLinkedList->head = (Node*) malloc(sizeof(Node));
    newLinkedList->tail = newLinkedList->head;

    return newLinkedList;
}

void appendNodeLinkedList(linkedList *list, void *item) {
    list->tail->currentItem = item; 
    list->tail->nextNode = (Node*) malloc(sizeof(Node)); 
    list->tail = list->tail->nextNode;
    list->size += 1;
}

//void removeNode(linkedList *linkedList, void *)


void nullifyLastNodeLinkedList(linkedList *list) {
    free(list->tail->nextNode);
    list->tail->nextNode = NULL;
}



 //replaces traverse
void printAllBoxesLinkedList(linkedList *list) { 
    Node *currentNode = list->head;
    for (int i = 0; i < list->size; i++) { 
        box *currentBoxPointer = (box*) currentNode->currentItem;
        printNBytes(currentBoxPointer->boxType, 4, "box type: ", "\t");
        printf("box size: %10u\n", *(currentBoxPointer->boxSize));
        currentNode = currentNode->nextNode;
    }
}

/**
 *  traverses a linked list and returns a box if exsists
 *  @param *headNode:       the first element in the linked list
 *  @param returnBox:       TRUE or FALSE, allows for skiping the box type checking
 *  @param boxReturnType:   a character array that's compared against each box type
 *  @return a box struct pointer
 */

box *getBoxFromLinkedList(linkedList *list, char boxReturnType[]) { 
    box *boxToReturn = NULL;
    Node *currentNode = list->head;

    for (int i = 0; i < list->size; i++) {
        box *currentBoxPointer = (box*) currentNode->currentItem;

        // && (boxToReturn == NULL) for taking the last match, will be based on which one is video
        if ((compareNBytes(currentBoxPointer->boxType, boxReturnType, 4) == 1) && (boxToReturn == NULL)) {
            boxToReturn = currentNode->currentItem;
        }

        currentNode = currentNode->nextNode;
    }

    if (boxToReturn == NULL) { 
        printf("No Match Found For: %s\n", boxReturnType);
    }

    return boxToReturn;
}


void freeBox(box *boxStruct) { 
    free(boxStruct->boxSize);
    free(boxStruct->boxType);
    free(boxStruct->boxData);

    free(boxStruct);
}

void freeNode(Node *nodeStruct) { 
    freeBox(nodeStruct->currentItem);
}