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
	SortedListPtr res = (SortedListPtr)malloc(sizeof(struct SortedList)); 	 // Allocate memory for the struct
	res->CompareFuncT = cf;  // funcptr
	res->DestructFuncT = df ; 

	// Create the first null node and make the sortedList have a pointer to it. 
	
	res->_llist = NULL ;	 // Nothing to point to right now .. 
	return res;
}


/*
 * SLDestroy destroys a SortedList, freeing all dynamically-allocated memory.
 */
void SLDestroy(SortedListPtr list) // DANGER !!!!!!!!!!!!!
{
	while(list->_llist != NULL)
	{

	// call the destructor on each element in the linked list 
		list->DestructFuncT(list->_llist->_value);
			
	// call destructor on each node ? 
		Node * toBeDeleted = list->_llist; 
		list->_llist = list->_llist->_next;
		free(toBeDeleted);

	// start from end and delete the node
	// have a pointer to the end of the node ? 
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
	 */


	/*
	 * The smaller element is added after the largert element , this ensures that 
	 * the larger elements are in the front 
	 *
	 * larger large small smaller
	 *
	 */
	if(list == 0 || newObj == 0 ) return 0 ; 
	Node * ptr = list->_llist;  // Point to the start or the 
	Node * prev = NULL ; 
	// The new Node to be inserted 
	Node * newNode = (Node *)malloc(sizeof(Node));		
	newNode->_value = newObj ; 
	newNode->_ref = 1;  // all the references has a value 1 as it is pointed by something when 
				// added to the lsit 
	/*
	 * The only possibilities are : 
	 * 	Node added at front
	 * 	Node added in between 
	 * 	Node added at end
	 * 	Node not added as it is a repetition < free the memory here > 
	 *
	 */

	if(ptr == NULL) // adding the first element to the list
	{
		newNode->_next = NULL ;  // show the new end of the linked list 
		list->_llist = newNode ; 
		return 1;  // successfully added to the begining of the list 
	}
	else // determine the right position to add the element to 
	{
		// if the element to be added is smaller , keep going froward		
		while(ptr != NULL )
		{
			int cmpVal = list->CompareFuncT(ptr->_value, newObj);
//			printf(" CMP VAL : %d " , cmpVal);
			if( cmpVal == -1)
			{
				// element to be inserted is larger 
				newNode->_next = ptr; 	// insert new element in betwen ptr and prev
				// handle the case when adding the second elemnt when prev == NULL
				if(prev == NULL)	
				{
					list->_llist = newNode; 
					newNode->_next = ptr;
				}
				else
				{
					prev->_next = newNode; 	
				}
				printf(" INSERTED ELEM %d " , *(int *)newObj) ; 
				return 1; 
			}
			else if (cmpVal == 0) // the newObj is not added to list 
			{
				printf(" REPEAT ELEM %d " , *(int *)newObj) ; 
				free(newNode); // free the allocated memory as we are not add it to list 
				return 0 ; // element not added to list  
			}
			prev = ptr; 
			ptr = ptr->_next; 	// move to next location 
		}	
		// if we reached this location then we have not added the node in between 
		// we add at the end
		newNode->_next = NULL ;
		list->_llist->_next = newNode; // element added at the end 	
		return 1;
	}
	// this location is never reached 
}

/*
 * AMBIGUITY 
 * 	Who does the final delete  ? 
 * 	who delete the node that has been removed from the list ? 
 *
 *
 *
 *
 */


/*
 * SLRemove should remove 'newObj' from the SortedList in a manner that
 *   maintains list order.
 *
 * SLRemove must not modify the data item pointed to by 'newObj'.
 *
 * SLRemove should return 1 on success, and 0 on failure.
 */

int SLRemove(SortedListPtr list, void *newObj)
{
	/*
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

	Node * ptr = list->_llist;  // point to start of the list
	Node * prev = NULL ; // Book keeping


	if(ptr == NULL) // adding the first element to the list
	{
		return 0 ; // nothing to delete  FAILED TO DELETE 
	}
	else // determine the right position to add the element to 
	{
		while(ptr != NULL )
		{
			int cmpVal = list->CompareFuncT(ptr->_value, newObj);

			if( cmpVal == 0 ) // found the location to delete the obj from 
			{ 		// the obj at location ptr 
			
					// Simple delete			
				if(prev == NULL) // object found in the first position itself
				{
					// The head should point to the next element now
					list->_llist = ptr->_next;		
					// ptr will be deleted based on the reference count
				}
				else
				{
					// delink the  ptr from the linked list
					prev->_next = ptr->_next; 	// skip the obj at ptr
				}

					// since we have delinked the ptr node from the linked list , the number of references to this decreases by 1
					ptr->_ref--; // decrease the reference

					if(ptr->_ref == 0 ) // not pointed to by any other pointer
					{
						// delete value using the caller defined destructor 
						list->DestructFuncT(ptr->_value);	 
						
						// delete the ptr free it as it has been malloced
						free(ptr);
						
						printf(" DELETED ELEM %d " , *(int *)newObj) ; 
						// delete succeded
						return 1; 
					}
					else // other " things " point to the current node ptr
					{

						// if there are other pointer to this node 
						// WHAT TO DO ? 
						// We delink the node from the list 
						// but do not delete it so that who ever points to 
						// this node can still get to it. 
						//
						// Also make no changes to the next from the ptr node
						// as this ensures that the other elements in the list 
						// can be reached by following this node 
						
						// delink the ptr from the linked list
	//					prev->_next = ptr->_next; 	// skip the obj at ptr
						// The delinking was done earlier so skip it here


						// Now ptr->_next had two " things pointing at it " 
						//  	1) the node which was decoupled from the lis t
						//  	2) the previous node to the decoupled node which now points at ptr->next 
						// Upadat the reference counter for ptr->_next ; 
						
						ptr->_next->_ref++; 	 // to take care of multiple pointers pointing at the node 

						// Nothing more to do 
						// We have delinked this node and touches nothing else so that 
						// the " thing " with a reference to this node 
						// can still access and iterate using this 

						// delete failed  ??  WARNING VERIFY THIS OR SPECIFY IN README 
						return 0 ; 
					}

			//		return 1 ; // DELETE SUCCEDED  // OR SHOULD WE NOT RETURN A 1 
			}
			
			// reaching this location means that we have not found the object we were looking for  	
			// we keep searching 

			prev = ptr; 
			ptr = ptr->_next; // move along the linked list
		} // end of while loop 
			
	// reaching this location means we have not found the object
	// we have failed in the delete 	
	return 0 ; 
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
	// Since an new " thing " is pointing to the first node in the linked list we increment the reference 
	if(list->_llist != NULL)		// AMBIGUITY WHAT HAPPENS IF LL IS NULL AND WE ADD LATER ON ? HOW WILL WE CHANGE THE REFERENCE COUNT ? 
	{
		list->_llist->_ref++; // increace the reference count 
	}
	res->_elemPtr  = list->_llist;  // point to the first element in the linked list

	// tentative 
	res->_sortList = list ; 
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
	// Since a " thing " is no longer pointing to the node , we can decrease the reference . 
	iter->_elemPtr->_ref--; 
	// Do not delete the node that the element is pointing to 
	// but delete the iterator struct 
	free(iter); // WARNING :: IS THIS RIGHT ? 
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
	if(iter->_elemPtr == NULL || iter->_elemPtr->_next == NULL) // REACHED END OF LIST 
	{
		return NULL ; 	
	}
	else // WARNING IS THIS ENOUGH ?  // SHOULD THE ITERATOR DELETING ANYTHING , IF IT ALONE POINTS TO THAT THING ? 
	{
		// get the data in the iterator 
		void * res = iter->_elemPtr->_next->_value; 
		
		// since the iterator moves to be next element 
		// the current element will have a reduced number of references
		iter->_elemPtr->_ref--; 	
		
		// now if the reference of the current element has become zero we can delete it , as no other object points to it . 
		// since the iterator cannot move back we this element is no longer needed .. 
			
		Node * tempElemPtr = iter->_elemPtr; 	 // holds a temporary reference to the ele

		if(iter->_elemPtr->_next != NULL)
		{
			iter->_elemPtr = iter->_elemPtr->_next;  
			// since the iter is now pointing to the next element we increase its reference 
			iter->_elemPtr->_ref++;  // note : elemptr now points to the next element 
		}
		else // if the next node is NULL , we make it point to null and do not change the reference  counter
		{
			iter->_elemPtr = NULL;
		}
	
		// If the node the iter pointed to before moving to the next node is orphaned meaning the ref == 0 , then we delete it to save memory // WARNING 
		if(tempElemPtr->_ref == 0 )
		{
			tempElemPtr->_next = NULL ; 	
			// will require changes in strucutr of itreator to get access to the destructor of void *
			iter->_sortList->DestructFuncT( tempElemPtr->_value ) ; 	

			free(tempElemPtr);
		}	
		
		return res;  // return the element
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
	if(iter->_elemPtr == NULL) // REACHED END OF LIST 
	{
		return NULL ; 	
	}
	else // WARNING IS THIS ENOUGH ?  // SHOULD THE ITERATOR DELETING ANYTHING , IF IT ALONE POINTS TO THAT THING ? 
	{
		// get the data in the iterator 
		void * res = iter->_elemPtr->_value; 
		
		// since the iterator moves to be next element 
		// the current element will have a reduced number of references
		iter->_elemPtr->_ref--; 	
		
		// now if the reference of the current element has become zero we can delete it , as no other object points to it . 
		// since the iterator cannot move back we this element is no longer needed .. 
			
		Node * tempElemPtr = iter->_elemPtr; 	 // holds a temporary reference to the ele


		// move the iterator to the next element if it is not null
		if(iter->_elemPtr->_next != NULL)
		{
			iter->_elemPtr = iter->_elemPtr->_next;  

			// since the iter is now pointing to the next element we increase its reference 
			iter->_elemPtr->_ref++;  // note : elemptr now points to the next element 
		
		}
		else // if the next node is NULL , we make it point to null and do not change the reference  counter
		{
			iter->_elemPtr = NULL;
		}
		

		// If the node the iter pointed to before moving to the next node is orphaned meaning the ref == 0 , then we delete it to save memory // WARNING 
		if(tempElemPtr->_ref == 0 )
		{
			tempElemPtr->_next = NULL ; 	
			// will require changes in strucutr of itreator to get access to the destructor of void *
			iter->_sortList->DestructFuncT( tempElemPtr->_value ) ; 	
			free(tempElemPtr);
		}	

	return res;  // return the element
	}		
		
	// never reach here 	
}



