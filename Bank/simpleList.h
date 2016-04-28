#ifndef SIMPLE_LIST
#define SIMPLE_LIST

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node_
{
	struct Node_ * next;
	int tid;

}Node;

typedef struct SimpleList_
{
	Node * head;
	Node * tail;

}SimpleList;


void appendToList(int thread_id, SimpleList * sl);
int getFront(SimpleList * sl);
void destroyList(SimpleList * sl);
SimpleList * getList();
void printList(SimpleList * sl);
#endif