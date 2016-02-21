#include "sorted-list.h"

 /*
  * FORWARD DECLERATION 
  */


/*
 * SLCreate creates a new, empty, 'SortedList'.
 *
 * SLCreate's parameters will be a comparator (cf) and destructor (df) function.
 *   Both the comparator and destructor functions will be defined by the user as per
 *     the prototypes above.
 *   Both functions must be stored in the SortedList struct.
 * 
 * SLCreate must return NULL if it does not succeed, and a non-NULL SortedListPtr
 *   on success.
 */

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df)
{
	SortedListPtr res = (SortedListPtr)malloc(sizeof(struct SortedList)); 	 
	res->CompareFuncT = cf;  
	res->DestructFuncT = df ; 

	
	res->_llist = NULL ;	
	return res;
}


/*
 * SLDestroy destroys a SortedList, freeing all dynamically-allocated memory.
 * THIS WILL KEEP NODES WITH ITERATORS ALIVE BUT DELETE THEIR NEXT NODE
 */
void SLDestroy(SortedListPtr list) 
{
	while(list->_llist != NULL)
	{
		printf("Freeing stuff\n");
	
		if(list->_llist->_value == 0 || list->_llist == 0)break;
		list->DestructFuncT(list->_llist->_value);
		Node * toBeDeleted = list->_llist; 
		if( toBeDeleted->_ref == 0){
			list->_llist = list->_llist->_next;
			free(toBeDeleted);
		} else {
			toBeDeleted->_ref--;
		}
	}

	free(list);
}


//===0.2: List Insert/Remove

/*
 * SLInsert inserts a given data item 'newObj' into a SortedList while maintaining the
 *   order and uniqueness of list items.
 * 
 * SLInsert should return 1 if 'newObj' is not equal to any other items in the list and
 *   was successfully inserted.
 * SLInsert should return 0 if 'newObj' is equal to an item already in the list or it was
 *   not successfully inserted
 *
 * Data item equality should be tested with the user's comparator function *
 */

int SLInsert(SortedListPtr list, void *newObj)
{
	/*
	 * if the same element if to be inserted twice ,do no add the new element 

	 * The smaller element is added after the largert element , this ensures that 
	 * the larger elements are in the front 
	 *
	 * larger large small smaller
	 *
	 */
	if(list == 0 || newObj == 0 ) return 0 ; 
	Node * ptr = list->_llist; 
	Node * prev = NULL ; 
	Node * newNode = (Node *)malloc(sizeof(Node));		
	newNode->_value = newObj ; 
	newNode->_ref = 1;  
	/*
	 * The only possibilities are : 
	 * 	Node added at front
	 * 	Node added in between 
	 * 	Node added at end
	 * 	Node not added as it is a repetition < free the memory here > 
	 *
	 */

	if(ptr == NULL) 
	{
		newNode->_next = NULL ;  
		list->_llist = newNode ; 
		return 1;  
	}
	else 
	{
		
		while(ptr != NULL )
		{
			int cmpVal = list->CompareFuncT(ptr->_value, newObj);
			if( cmpVal == -1)
			{
				newNode->_next = ptr; 	
				if(prev == NULL)	
				{
					list->_llist = newNode; 
					newNode->_next = ptr;
				}
				else
				{
					prev->_next = newNode; 	
				}
				return 1; 
			}
			else if (cmpVal == 0) 
			{
				free(newNode); 
				return 0 ; 
			}
			prev = ptr; 
			ptr = ptr->_next; 	
		}	

		newNode->_next = NULL ;
		list->_llist->_next = newNode; 
		return 1;
	}
}


/*
 * SLRemove should remove 'newObj' from the SortedList in a manner that
 *   maintains list order.
 *
 * SLRemove must not modify the data item pointed to by 'newObj'.
 *
 * SLRemove should return 1 on success, and 0 on failure.
  * possibilities : 
 * 	Remove element  [ element found ]
 * 		> from front
 * 		> between 
 * 		> end
 *	
 *	Do not remove element [ element not found ]
 * 		> either list empty
 * 		> element not found
 */


int SLRemove(SortedListPtr list, void *newObj)
{

	Node * ptr = list->_llist;  
	Node * prev = NULL ; 
	if(ptr == NULL) return 0 ;
	else 
	{
		while(ptr != NULL )
		{
			int cmpVal = list->CompareFuncT(ptr->_value, newObj);
			if( cmpVal == 0 ) { 		
				if(prev == NULL) {
					list->_llist = ptr->_next;		
				}
				else {
					prev->_next = ptr->_next; 	
				}
				ptr->_ref--; 
				if(ptr->_ref == 0 ) 
				{
					list->DestructFuncT(ptr->_value);	 
					free(ptr);
					return 1; 
				}
				else {
					if(ptr->_next != 0)			
						ptr->_next->_ref++; 	 
					return 1; 
				}
			}
			prev = ptr; 
			ptr = ptr->_next; 
		} 
	return 0; 
	}
}





/*
 * SLCreateIterator creates a SortedListIterator for the SortedList pointed to by 'list'.
 *
 * SLCreateIterator should return a non-NULL SortedListIteratorPtr on success.
 * SLCreateIterator should return a NULL SortedListIteratorPtr if it could not
 *  construct a SortedListIterator, or if the SortedListPtr parameter 'list' is NULL.
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list)
{
	SortedListIteratorPtr res = (SortedListIteratorPtr)malloc(sizeof( struct SortedListIterator));
	if(list->_llist != NULL)	
	{
		list->_llist->_ref++; 
	}
	res->_elemPtr  = list->_llist; 

	res->destroy = list->DestructFuncT; 
	return res ; 
}


/*
 * SLDestroyIterator destroys a SortedListIterator pointed to by parameter 'iter'.
 *
 * SLDestroyIterator should destroy the SortedListIterator, but should NOT
 *  affect the list used to create the SortedListIterator in any way.
 */

void SLDestroyIterator(SortedListIteratorPtr iter)
{
	if(iter == 0) return;
	if(iter->_elemPtr != 0 ) {
		if(iter->_elemPtr->_ref == 0) {
			iter->destroy(iter->_elemPtr->_value);
			free(iter->_elemPtr);
		}
		else {
			iter->_elemPtr->_ref--; 
		}
	}
	free(iter); 
}


//===1.2: SortedList Iterator Get/Next Operations

/*
 * SLNextItem returns a pointer to the data associated with the
 *  next item in the SortedListIterator's list
 *
 * SLNextItem should return a NULL if all of iter's elements have
 *  been iterated through.
 *
 * NB: Be sure to check the length of the list that SortedListIterator holds
 *         before attempting to access and return an item from it.
 *         If an item is removed from the list, making it shorter, be careful not
 *         to try to read and return an item off the end of the list.
 */

void * SLNextItem(SortedListIteratorPtr iter)
{
	if(iter->_elemPtr == NULL || iter->_elemPtr->_next == NULL) 
	{
		return NULL ; 	
	}
	else 
	{
		void * res = iter->_elemPtr->_next->_value; 
		iter->_elemPtr->_ref--; 	
		Node * tempElemPtr = iter->_elemPtr; 	 
		if(iter->_elemPtr->_next != NULL)
		{
			iter->_elemPtr = iter->_elemPtr->_next;  
			iter->_elemPtr->_ref++;  
		}
		else
		{
			iter->_elemPtr = NULL;
		}
		if(tempElemPtr->_ref == 0 )
		{
			tempElemPtr->_next = NULL ; 	
			iter->destroy( tempElemPtr->_value ) ; 	
			free(tempElemPtr);
		}	
		
		return res;  
	}
}


/*
 * SLGetItem should return a pointer to the current data item
 *   The current data item is the item that was most recently returned by SLNextItem
 *   SLGetItem should not alter the data item that SortedListIterator currently refers to
 *
 * SLGetItem should return a NULL pointer if the SortedListIterator has been advanced
 *  all the way through its list.
 *
 * NB: Be sure to check the length of the list that SortedListIterator holds
 *         before attempting to access and return an item from it.
 *         If an items are removed from the list, making it shorter, be careful not
 *         to try to read and return an item off the end of the list.
 */

void * SLGetItem( SortedListIteratorPtr iter )
{
	if(iter->_elemPtr == NULL) 
	{
		return NULL ; 	
	}
	else 
	{
		return iter->_elemPtr->_value;
	}		
		
}

