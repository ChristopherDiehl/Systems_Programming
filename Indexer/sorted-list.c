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
 SortedListPtr list = malloc(sizeof(SortedListPtr));
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
  * if startNode->numOfIterators == -1 that means that node has been deleted and iterator is responsible for deleting
 */
  void * SLNextItem(SortedListIteratorPtr iter) {
  	if(iter->startNode == 0) return 0;
    if(iter->startNode->numOfIterators ==1) {
      Node tempNode = iter->startNode;
      iter->startNode = iter->startNode->nextNode;
      iter->destroy(tempNode->data);
      free(tempNode);
    } else {
      iter->startNode = iter->startNode->nextNode;
    }
  	if(iter->startNode ==0) return 0;
    iter->startNode->numOfIterators++;
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
 * If numOfIterators in Node > 0 then don't delete iterator
 * Save Iterator node, but free list
 *
*/

void SLDestroy(SortedListPtr list) {
 printf("tempNode = lastNode\n");
 int i =0;
 while(list->lastNode !=0){
  Node tempNode = list->lastNode;
  list->lastNode= list->lastNode->prevNode;
  if(tempNode->numOfIterators == 0) {
    printf("Attempting to destroy %d  stuf \n",i);
    list->destroy(tempNode->data);
    free(tempNode);
  }   
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
 *if data > newObj then compare == -1
 *if data == newObj then compare == 0
 *if data < newObj then compare == 1
 *when node is created the numOfIterators++
 *numOfIterators is for the iterator
 * - numOfIterators keeps track of how many iterators are pointing at the node,
 * -this is how we handle if a node gets deleted while iterator is on it
 * -since no iterators, initialize the numOfIterators ==0
 *
 *
*/

int SLInsert (SortedListPtr list, void *newObj){
  if(list ==0 || newObj ==0) return 0;
  if(list->firstNode == 0){
    printf("First node is created");
    list->firstNode = malloc(sizeof(Node));
    list->firstNode->data = newObj;
    list->lastNode = list->firstNode;
    list->firstNode->numOfIterators =0;
    return 1;
  } else {
    Node newNode = malloc(sizeof(Node));
    if(newNode ==0) return 0;
    newNode->data= newObj;
    newNode->numOfIterators ==0;
    Node iterNode = list->firstNode;

    while(iterNode != 0){ 
      int compareReturn = list->compare(newObj,iterNode->data);
      if(compareReturn == 0) {
        //return 0 if data is already in list. LIST IS UNIQUE
        return 0;
      }else if(compareReturn == 1) {
        if(iterNode->nextNode == 0){
          //NEW LASTNODE
          iterNode->nextNode = newNode;
          newNode->prevNode = iterNode;
          list->lastNode = newNode;
          return 1;
        }
        iterNode=iterNode->nextNode;
      }

      else if(compareReturn == -1) {
        //this is where we add firstNodes and middleNodes
        //if prevNode ==0 then iterNode == firstNode
        //else we must be adding someone in the middle of the list
        if(iterNode->prevNode == 0) {
          //need to create new firstNode
          iterNode->prevNode = newNode;
          newNode->nextNode = iterNode;
          list->firstNode = newNode;
          return 1;

        } else {
          //since not a firstNode and not a lastNode then must be somewhere in the middle
          iterNode->prevNode->nextNode = newNode;
          newNode->prevNode = iterNode->prevNode;
          newNode->nextNode = iterNode;
          iterNode->prevNode = newNode;
          return 1;
        }
      }
    }
    free(newNode);
  }
  return 0;
}

void CYCLE(SortedListPtr list) {
  Node temp = list->firstNode;
  int i = 1;
  printf("FRONT NODE\n"); 
  while(temp != 0) {
     printf("Node: %d DATA: %d\n",i, *(int *)temp->data);
     temp=temp->nextNode;
    i++;
  }
  temp = list->lastNode;
  i--;
  printf("LAST NODE\n");
  while (temp!=0){
   printf("Node: %d DATA: %d\n",i,*(int*)temp->data);
   temp=temp->prevNode;
   i--;
  }   
}
int compareint(void * num0, void *num1) {
  int *compare0 = (int*)num0;
  int *compare1 = (int*)num1;
  if(* compare1 > *compare0)
    return 1;
  if (*compare1 < *compare0)
    return -1;
  return 0;
}
void intDestructFunc(void * thingToDestroy){
  free(thingToDestroy);
}
int main(){
  int (*compare)(void*,void*);
  compare = &compareint;
  void (*destruct)(void *);
  destruct = &intDestructFunc;
  SortedListPtr slp = SLCreate(compare,destruct);
  int i = 0;
  while(i < 10) {
    int * newvalue = (int *) malloc (sizeof(int));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    printf("Test is complete. Return value:  %d\n",returnVal);
    i+= 2;
  }
  i =1;
  while(i < 10) {
    int * newvalue = (int *) malloc (sizeof(int));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    printf("Test is complete. Return value:  %d\n",returnVal);
    i+= 2;
  }
  while(i < 10) {
    int * newvalue = (int *) malloc (sizeof(int));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    printf("Test is complete. Return value:  %d\n",returnVal);
    i+= 2;
  }
  CYCLE(slp);
  SLDestroy(slp);
  return 0;
}