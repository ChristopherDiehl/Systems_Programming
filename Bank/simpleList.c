#include "simpleList.h"

SimpleList * getList()
{
	SimpleList * sl = malloc(sizeof(SimpleList));
	sl->head = 0;
	sl->tail = 0;
	sl->size = 0;
	return sl;
}

void appendToList(pthread_t thread_id, SimpleList * sl)
{
	if(sl == 0)
	{
		printf("[-] invalid parameter");
		return;
	}

	Node * tempNode = malloc(sizeof(Node));
	tempNode->tid = thread_id;

	if(sl->head == 0)
	{

		sl->head = tempNode;
		sl->tail = sl->head;
		//printf("%d\n",sl->head->tid);

	} else
	{
		//printf("ELSE %d\n",sl->head->tid);

		sl->tail->next = tempNode;
		sl->tail = tempNode;
	}
	sl->size ++;
}

pthread_t getFront(SimpleList * sl)
{
	

	if(sl->head == 0)
	{
		printf("returning -1");
		return NULL;
	}

	Node * tempNode = sl->head;
	pthread_t temp = tempNode->tid;
	sl->head = sl->head->next;
	free(tempNode);
	return temp;

}
void printList(SimpleList * sl)
{
	Node * temp = sl->head;
	while(temp != 0)
	{
		printf("%d\n", temp->tid);
		temp = temp->next;
	}
}
void destroyList(SimpleList * sl)
{
	while(sl->head != 0)
	{
		Node * tempNode = sl->head;
		sl->head = sl->head->next;
		free(tempNode);
	}

	free(sl);
}
int getSize(SimpleList * sl)
{
	return sl->size;
}