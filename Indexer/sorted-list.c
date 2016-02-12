/*sorted-list.c*/
#include "sorted-list.h"

/*Flesh out SortedList
 * If malloc fails then return 0, else return list pointer
 * storing compare and delete functions in SortedList
 * returns a pointer to the list with firstNode and lastNode null
 */
SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df) {
 SortedListPtr  list = malloc(sizeof(SortedListPtr));
 if(list != 0){
	list->destroy = df;
	list->compare = cf;
	return list;
 }
 return 0;
}

/*SLDestroy will free all the nodes in the sortedList
 * Once a node == 0 then free the sortedlist
*/

void SLDestroy(SortedListPtr list) {
 while(list->firstNode !=0){
  Node tempNode = firstNode;
  firstNode = firstNode->nextNode;
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
*/

int SLInsert (SortedListPtr list, void *newObj){

}
