#ifndef FREQUENCY_LIST_H
#define FREQUENCY_LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*Frequency list
 *
 *Used to keep track of string tokens
 *stores strings 
 *alphabetical order of token & then filename
 *also keeps track of number of times tokens apepar
 *remember that token needs freed when it's all said and done
 *run destroy on Json struct
 */
#define TRUE 1
#define FALSE 0
typedef struct Node_ Node;
struct Node_ {
	struct Node_ * next;
	struct Node_ * prev;
	int frequency;
	char * filename;
	char * token;
	int isTrailing; //Node will know if it is trailing. If it is trailing then format record differently
};

typedef struct Json_ Json;
struct Json_ {
	char * filename;
	char * token;
	int frequency;
	int isTrailing;
};

typedef struct FrequencyList_ FrequencyList;
struct FrequencyList_ {
	Node * head;
	Node * tail;
	int numOfNodes;
};

FrequencyList * getFrequencyList();
int addToList(char * filename, char * token, FrequencyList * fList);
int deleteList(FrequencyList * fList);
int isEmpty(FrequencyList * fList);
void printList(FrequencyList * fList);
Json * removeFromHead(FrequencyList * fList);
#endif