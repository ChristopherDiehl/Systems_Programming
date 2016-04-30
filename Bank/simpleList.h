#ifndef SIMPLE_LIST
#define SIMPLE_LIST

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct Node_
{
	struct Node_ * next;
	pthread_t tid;

}Node;

typedef struct SimpleList_
{
	Node * head;
	Node * tail;
	int size;

}SimpleList;


void appendToList(pthread_t thread_id, SimpleList * sl);
pthread_t getFront(SimpleList * sl);
void destroyList(SimpleList * sl);
SimpleList * getList();
void printList(SimpleList * sl);
int getSize(SimpleList * sl);
#endif