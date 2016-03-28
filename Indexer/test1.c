#include "frequencyList.h"
#include "fileTokenizer.h"

int main(int argc, char **argv) {

	/* check the input */ 
	if(argc !=2)
	{
		return -1 ; 
	}	

	TokenizerT * tk = Tokenize(argv[1]);
	FrequencyList * fList = getFrequencyList();
	while(1)
	{
		char * res = GetToken(tk);
		char * temp = malloc(strlen(res) +1);
		strcpy(temp,res);
		if(res != 0){
			printf("Trying to add \"%s\" from %s to filename\n", temp, argv[1]);
			//printf("Return VAL %d\n",addToList(argv[1], res, fList));
		}
		if(res == 0)
			break ; 
		//printf("%s\n",res);
		if(res != 0)
			free(res);
	}	
	printf("=======================\n");
	printList(fList);

	tkDestroy(tk);
  return 0;
}
