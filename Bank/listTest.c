#include "simpleList.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	SimpleList * sl = getList();

	appendToList(500,sl);
	appendToList(1234,sl);
	appendToList(04,sl);
	appendToList(23,sl);
	appendToList(64,sl);

	int i = 0;
	//printList(sl);
	do {
		printf("STRAT\n");

		i = getFront(sl);
		if(i == -1){
			break;
		}
		printf("%d\n",i);
		printf("GOING BACK\n");
	}while(i != -1);
	printf("DESTROYING LIST");
	destroyList(sl);
	return 0;
}