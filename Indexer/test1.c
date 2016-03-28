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
		if(res != 0){
			printf("Trying to add \"%s\" from %s to list\n", res, argv[1]);
			printf("Return VAL %d\n",addToList(argv[1], res, fList));
		}
		if(res == 0)
			break ; 
		//printf("%s\n",res);

	}	
	addToList("Bacon3 Me","Bacon", fList);

	printf("=======================\n");
	printList(fList);

	tkDestroy(tk);
  return 0;
}
