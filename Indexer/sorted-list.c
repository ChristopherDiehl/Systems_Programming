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
	list->DestructFuncT = df;
	list->CompareFuncT = cf;
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
 	if(slip ==0) return null;
 	slip->startNode = malloc(sizeof(Node));
 	if (slip->startNode == 0) return null;
 	slip->startNode = list->firstNode;
 	return slip;
 }

 /*To destroy a SortedListIteratorPtr, we just destroy the node then the SLIP
  *To do so we use free
 */

  void SLDestroyIterator(SortedListIteratorPtr iter) {
 	free(iter->startNode);
 	free(iter);
 }

 /*void * SLNextItem(SortedListIteratorPtr iter)
  * SLNextItem just returns the data of the next node in iter
  * If iter->startNode == 0 then returns 0
 */
  void * SLNextItem(SortedListIteratorPtr iter) {
  	if(iter->startNode == 0) return 0;
  	iter->startNode = iter->startNode->nextNode;
  	if(iter->startNode ==0) return 0;
  	void * data = iter->startNode->data;
  	return data;
  }

   /*void * SLGetItem(SortedListIteratorPtr iter)
  * SLNextItem just returns the data of the node in iter
  * If iter->startNode == 0 then returns 0
 */
  void * SLGetItem(SortedListIteratorPtr iter) {
  	if(iter->startNode == 0) return 0;
  	void * data = iter->startNode->data;
  	return data;
  }