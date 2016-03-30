#include "frequencyList.h"
#include "fileTokenizer.h"

int main(int argc, char **argv) {

	/* check the input */ 
	//char * f0 = "bacon0.txt";
	//char * f1 = "bacon1.txt";

	TokenizerT * tk = Tokenize("bacon0.txt");
	FrequencyList * fList = getFrequencyList();
	while(1)
	{
		char * res = GetToken(tk);
		if(res != 0 && strcmp(res, "") != 0){
			printf("Trying to add \"%s\" from %s to list\n", res, "bacon0.txt");
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
			printf("Trying to add \"%s\" from %s to list\n", res, "bacon1.txt");
			printf("Return VAL %d\n",addToList("bacon1.txt", res, fList));
			//printf("==========\n");
			//printList(fList);
			//printf("==========\n");
			
		}
		if(res == 0)
			break ; 
		//printf("%s\n",res);

	}	
	printf("=======================\n");
	printList(fList);

	tkDestroy(tk);
  return 0;
}


