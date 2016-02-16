/*sorted-list.c*/
#include "sorted-list.h"
#include <stdio.h>


/*Flesh out SortedList
 * If malloc fails then return 0, else return list pointer
 * storing compare and delete functions in SortedList
 * returns a pointer to the list with firstNode and lastNode null
 * Builds and initializes sortedListPtr struct
 *
 */
SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df) {
 SortedListPtr  list = malloc(sizeof(SortedListPtr));
 if(list != 0){
	list->destroy = df;
	list->compare = cf;
  list->firstNode = 0;
	return list;
 }
 return 0;
}


/*SortedListIteratorPtr SLCreateIterator(SortedListPtr list) 
 * SortedListIteratorPtr will be a node, 
 *starts at firstNode and 'walks' down list until it node.nextNode == 0
 *then returns null
 *sortedLIstIteratorPtr struct contains only a Node struct
*/
 SortedListIteratorPtr SLCreateIterator(SortedListPtr list) {
 	SortedListIteratorPtr slip = malloc(sizeof(SortedListIteratorPtr));
 	if(slip ==0) return 0;
 	slip->startNode = list->firstNode;
 	return slip;
 }

 /*To destroy a SortedListIteratorPtr, we just destroy the SLIP
  *To do so we use free
 */

  void SLDestroyIterator(SortedListIteratorPtr iter) {
 	 free(iter);
 }

 /*void * SLNextItem(SortedListIteratorPtr iter)
  * SLNextItem just returns the data of the next node in iter
  * If iter->startNode == 0 then returns 0
  * if startNode->numOfReferences == -1 that means that node has been deleted and iterator is responsible for deleting
 */
  void * SLNextItem(SortedListIteratorPtr iter) {
  	if(iter->startNode == 0) return 0;
    if(iter->startNode->numOfReferences ==1) {
      Node tempNode = iter->startNode;
      iter->startNode = iter->startNode->nextNode;
      iter->destroy(tempNode->data);
      free(tempNode);
    } else {
      iter->startNode = iter->startNode->nextNode;
    }
  	if(iter->startNode ==0) return 0;
    iter->startNode->numOfReferences++;
  	return iter->startNode->data;
  }

   /*void * SLGetItem(SortedListIteratorPtr iter)
  * SLNextItem just returns the data of the node in iter
  * If iter->startNode == 0 then returns 0
 */
  void * SLGetItem(SortedListIteratorPtr iter) {
  	if(iter->startNode == 0) return 0;
  	return iter->startNode->data;
  }
/*SLDestroy will free all the nodes in the sortedList
 * Once a node == 0 then free the sortedlist 
 * If numOfReferences in Node > 1 then just go through
*/

void SLDestroy(SortedListPtr list) {
 Node lastNode = list->lastNode;
 while(list->lastNode !=0){
  Node tempNode = lastNode;
  lastNode = lastNode->prevNode;
  list->destroy(tempNode->data);
  free(tempNode);
 }
 free(list);
}

/*SLInsert will return 1 if newObj is not equal to any other item in the last/ was inserted
 * returns a 0 if newObj is equal to an item already in the list/ wasn't inserted
 * Start at firstNode and go through list until we get to the last node,
 * in which case we appendto back of list
 * Since we are starting at front, when the compare function gives -1 
 *  or when the object to be added is less than the current node
 *  newNode = obj trying to append.
 *  Then change currentNode.prevNode.nextNode = newNode.
 *   we change the currentNode.prevNode = newNode
 *If we recieve a 0, or obj is currently in list then return a 0
 *
*/

int SLInsert (SortedListPtr list, void *newObj){
  if(list->firstNode == 0){
    printf("FIRSTNODE ?");
    list->firstNode = (Node)malloc(sizeof(Node));
    list->firstNode->data = &newObj;
    list->lastNode = list->firstNode;
  } else {
    Node iterNode = list->firstNode;
    while(iterNode !=0){ 
      int compareReturn = list->compare(iterNode->data,newObj);
      printf("CompareReturn %d",compareReturn);
      if(compareReturn == -1) {
       iterNode = iterNode->nextNode;
      }
      else if(compareReturn == 1) {
        printf("NEW NODE ABOUT TO BE CREATED");
        Node newNode = (Node)malloc(sizeof(Node));
        newNode->data = &newObj;
        newNode->nextNode = iterNode;
        newNode->prevNode = iterNode->prevNode;
        iterNode->prevNode->nextNode = newNode;
        iterNode->prevNode = newNode;
        printf("NEW NODE CREATED");
        return 1;
      }else if (compareReturn == 0) {
        return 0;
      }
      else {
        printf("MAYDAY CAPTAIN");
      }
    }
  }
  printf("WE DID GOOD WORK SON\n");
  return 0;
}
