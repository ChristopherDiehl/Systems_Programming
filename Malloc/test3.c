#include  "mymalloc.h"
#include  <unistd.h>
#include  <stdio.h>
#include  <string.h>
//test to make sure data is stored in pointers correctly without overwrite
int main() {
	int a = 5;
	int b = 6;
	int c = 7;
	int * ptra = malloc(sizeof(int));
	int * ptrb = malloc(sizeof(int));
	int * ptrc = malloc(sizeof(int));

	* ptra = 2134;
	* ptrb = 5123;
	* ptrc= 8952;
	if(*ptra != 2134 || *ptrb != 5123|| *ptrc != 8952){
		printf("FAILED\n");
	}

	printf("DONE\n");
	return 0;
}