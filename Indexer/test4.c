#include "frequencyList.h"
#include "fileTokenizer.h"
#include "jsonWriter.h"

int main(int argc, char **argv) {

	/* check the input */ 
	if(argc !=2)
	{
		printf("[-]Please enter valid args\n" );
		return -1 ; 
	}	

	TokenizerT * tk = Tokenize(argv[1]);
	FrequencyList * fList = getFrequencyList();
	while(1)
	{
		char * res = GetToken(tk);
		if(res != 0 && strcmp(res, "") != 0){
			printf("Trying to add \"%s\" from %s to list\n", res, argv[1]);
			printf("Return VAL %d\n",addToList(argv[1], res, fList));
			//printf("==========\n");
			//printList(fList);
			//printf("==========\n");
			
		}
		if(res == 0)
			break ; 
		//printf("%s\n",res);

	}	
	printf("Second time around\n");
	tk = Tokenize("cable.txt");
	while(1)
	{
		char * res = GetToken(tk);
		if(res != 0 && strcmp(res, "") != 0){
			printf("Trying to add \"%s\" from %s to list\n", res, argv[1]);
			printf("Return VAL %d\n",addToList("cabla.txt", res, fList));
			//printf("==========\n");
			//printList(fList);
			//printf("==========\n");
			
		}
		if(res == 0)
			break ; 
		//printf("%s\n",res);

	}	
		tk = Tokenize("bacon0.txt");
	while(1)
	{
		char * res = GetToken(tk);
		if(res != 0 && strcmp(res, "") != 0){
			printf("Trying to add \"%s\" from %s to list\n", res, argv[1]);
			printf("Return VAL %d\n",addToList("bacon0.txt", res, fList));
			//printf("==========\n");
			//printList(fList);
			//printf("==========\n");
			
		}
		if(res == 0)
			break ; 
		//printf("%s\n",res);

	}	
	tk = Tokenize("bacon1.txt");
	while(1)
	{
		char * res = GetToken(tk);
		if(res != 0 && strcmp(res, "") != 0){
			printf("Trying to add \"%s\" from %s to list\n", res, argv[1]);
			printf("Return VAL %d\n",addToList("bacon1.txt", res, fList));
			//printf("==========\n");
			//printList(fList);
			//printf("==========\n");
			
		}
		if(res == 0)
			break ; 
		//printf("%s\n",res);

	}	
		tk = Tokenize("fileTokenizer.c");
	while(1)
	{
		char * res = GetToken(tk);
		if(res != 0 && strcmp(res, "") != 0){
			printf("Trying to add \"%s\" from %s to list\n", res, argv[1]);
			printf("Return VAL %d\n",addToList("fileTokenizer.c", res, fList));
			//printf("==========\n");
			//printList(fList);
			//printf("==========\n");
			
		}
		if(res == 0)
			break ; 
		//printf("%s\n",res);

	}	
		tk = Tokenize("test1.c");
	while(1)
	{
		char * res = GetToken(tk);
		if(res != 0 && strcmp(res, "") != 0){
			printf("Trying to add \"%s\" from %s to list\n", res, argv[1]);
			printf("Return VAL %d\n",addToList("test1.c", res, fList));
			//printf("==========\n");
			//printList(fList);
			//printf("==========\n");
			
		}
		if(res == 0)
			break ; 
		//printf("%s\n",res);

	}	
		tk = Tokenize("asst3hints.txt");
	while(1)
	{
		char * res = GetToken(tk);
		if(res != 0 && strcmp(res, "") != 0){
			printf("Trying to add \"%s\" from %s to list\n", res, argv[1]);
			printf("Return VAL %d\n",addToList("asst3hints.txt", res, fList));
			//printf("==========\n");
			//printList(fList);
			//printf("==========\n");
			
		}
		if(res == 0)
			break ; 
		//printf("%s\n",res);

	}	
	printList(fList);
	jsonWrite(fList,"output1.txt");

	printf("=======================\n");
	printList(fList);

	tkDestroy(tk);
  return 0;
}


