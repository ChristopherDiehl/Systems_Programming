//sample compareInt function
compareint(void * num0, void *num1) {
	int *compare0 = (int*)num0;
	int *compare1 = (int*)num1;
	if(compare1 > compare0)
		return 1;
	if (compare1 < compare0)
		return -1;
	return 0;
}
/* list->firstNode = malloc(sizeof(Node));
 list->lastNode = malloc(sizeof(Node));
 
*/

 void SLDestroyIterator(SortedListIteratorPtr iter) {
 	free(iter->startNode);
 	free(iter);
 }

 /*void * SLNextItem(SortedListIteratorPtr iter)
  * SLNextItem just returns the data of the node in iter, then moves iter->startNode to the next Node
  * If iter->startNode == 0 then returns 0
 */
  void * SLNextItem(SortedListIteratorPtr iter) {
  	if(iter->startNode == 0) return 0;

  	void * data = iter->startNode->data;
  	iter->startNode = iter->startNode->nextNode;
  	return data;
  }