#ifndef MPEG_HEAD
    #define MPEG_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "headers/types.h"
    #include "headers/bitUtility.h"
    #include "headers/printUtility.h"
#endif


/**
 * @brief create a new linkedList
 * @return a new linked list struct with all values initialized
 */
linkedList* initLinkedList() { 
    linkedList *newLinkedList = (linkedList*) malloc(sizeof(linkedList));
    
    newLinkedList->size = (unsigned int*) malloc(sizeof(unsigned int));
    *(newLinkedList->size) = 0;

    newLinkedList->head = (Node*) calloc(1, sizeof(Node));
    newLinkedList->tail = newLinkedList->head;
    newLinkedList->current = newLinkedList->head;

    return newLinkedList;
}


/**
 * @brief place an item in the current node and create a new node
 * @param *list     -   the linkedList to append to
 * @param *item     -   the item to add to the tail node's currentItem
 */
void appendNodeLinkedList(linkedList *list, void *item) {
    list->tail->currentItem = item; 
    list->tail->nextNode = (Node*) calloc(1, sizeof(Node)); 

    list->tail = list->tail->nextNode;
    *(list->size) += 1;
}


// may not be needed
// void removeNode(linkedList *linkedList, void *)


/**
 * @brief set last node's nextNode to null
 * @param *list     -   the linkedList affected  
 */
void nullifyLastNodeLinkedList(linkedList *list) {
    //free(list->tail->nextNode); NextNode not initalized
    list->tail->nextNode = NULL;
}


/**
 * @brief print boxType & boxSize of each box in a linkedList
 * @param *list  -   the linkedList read from
 */
void printAllBoxesLinkedList(linkedList *list) { 
    Node *currentNode = list->head;
    for (int i = 0; i < *(list->size); i++) { 
        box *currentBoxPointer = (box*) currentNode->currentItem;
        printNBytes(currentBoxPointer->boxType, 4, "box type: ", "\t");
        printf("box size: %10u\n", *(currentBoxPointer->boxSize));
        currentNode = currentNode->nextNode;
    }
}


/**
 * @brief find and return box by its boxType
 * @param *list             -   the linkedList searched
 * @param *boxReturnType    -   the type to find    
 * @return NULL or box pointer to a box of type *boxReturnType
 */
box *getBoxFromLinkedList(linkedList *list, char boxReturnType[]) { 
    box *boxToReturn = NULL;
    list->current = list->head;

    for (int i = 0; i < *(list->size); i++) {
        box *currentBoxPointer = (box*) list->current->currentItem;

        if ((compareNBytes(currentBoxPointer->boxType, boxReturnType, 4) == 1)) {
            // DEBUG printf("True, returning\n");
            boxToReturn = list->current->currentItem;

            list->current = list->current->nextNode;

            return boxToReturn;
        }

        list->current = list->current->nextNode;
    }

    if (boxToReturn == NULL) { 
        printf("No (Other) Match Found For: %s\n", boxReturnType);
    }

    return boxToReturn;
}


/**
 * @brief reset list->current to allow for another getBoxFromLinkedList
 * from the start of the linkedList
 * @param list      -   the linkedlist to reset list->current for
 */
void resetCurrentNodeLinkedList(linkedList *list) { 
    list->current = list->head;
}


/**
 * @brief free ALL fields of a box. all fields must be allocations 
 * and not references
 * @param boxStruct     -   the struct to free the fields of
 */
void freeBox(box *boxStruct) { 
    // this is only required for boxes who's 
    // fields are allocations and not references
    // to their parent container
    free(boxStruct->boxSize);
    free(boxStruct->boxType);
    free(boxStruct->boxData);

    // freeing the box itself since it holds pointer values
    free(boxStruct);
}


/**
 * @brief call a function to free the currentItem
 * @param nodeStruct    -   node to free
 * @return  the next node
 *
 * @note can generalize as needed 
 */
Node *freeBoxNode(Node *nodeStruct) { 
    Node *nextNode = nodeStruct->nextNode;

    freeBox(nodeStruct->currentItem);
    // Note: free(nodeStruct->currentItem) handeled by freeBox
    // Note: not freeing nodeStruct->nextNode handeled by freeLinkedLists if last
    free(nodeStruct);

    return nextNode;
}


/**
 * @brief free all nodes in a linkedList
 * @param list  -   list to free
 * @param type  -   type of items contained in linkedList
 */
void freeLinkedList(linkedList *list, char type[]) { 

    Node *currentNode = list->head;

    // stopping right before last element to avoid 
    // attempting to free a NULL created by nullifyLastNodeLinkedList
    for (int i = 0; i < *(list->size) - 1; i++) {
        if (strncmp(type, "box", 3)) { 
            currentNode = freeBoxNode(currentNode);
            // DEBUG printf("%d %d\n", currentNode, currentNode->nextNode);
            list->head = currentNode;
        }
    }

    // freeing the last node
    if (strncmp(type, "box", 3)) { 
        freeBox(currentNode->currentItem);
        if (currentNode->nextNode != NULL) { 
            // this case should not happen
            printf("error occured in freeLinkedList final node");
            free(currentNode->nextNode);
        }
    }

    free(currentNode);
}

/* int main() {
    linkedList *test = initLinkedList();
    box *testBox1 = (box*) malloc(sizeof(box));
    box *testBox2 = (box*) malloc(sizeof(box));
    box *testBox3 = (box*) malloc(sizeof(box));
    appendNodeLinkedList(test, testBox1);
    appendNodeLinkedList(test, testBox2);
    appendNodeLinkedList(test, testBox3);

    freeLinkedList(test, "box");
} */