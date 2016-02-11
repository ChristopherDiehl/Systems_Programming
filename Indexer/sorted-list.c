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
