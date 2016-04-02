#include "frequencyList.h"
#include "fileTokenizer.h"
#include "jsonWriter.h"
#include <errno.h>
int main(int argc, char **argv) {

	/* check the input */ 
	if(argc !=3 || argv[1] == 0 || argv[2] == 0)
	{
		printf("[-]Please enter valid args\n" );
		return -1 ; 
	}	
	FrequencyList * fList = getFrequencyList();
    DIR *fDir = opendir(argv[2]);
    if(errno == ENOTDIR  ){
    	//must be file
 		file_handler(argv[2],fList);
    }else if (errno == 0){
    	//must be a directory
    	directory_handle(argv[2],fList);
    } else {
    	//pDir is null...
    	printf("Not a valid directory or file name\n");
    	return -1;
    }
	jsonWrite(fList,argv[1]);
	deleteList(fList);


}	

void directory_handle(char * name, FrequencyList * fList)
{

	DIR *fDir = opendir(name)
	struct dirent fDirent = 0;
    while ((pDirent = readdir(pDir)) != NULL) {
		char * fullPath = strcat(name,pDirent->d_name);
    	if(pDirent->d_type == DT_REG){
    		file_handler(fullPath,fList);
    	} else if(pdirent->d_type == DT_DIR){
    		directory_handle(fullPath,fList);
    	}
    }
    
    closedir(fDir);
}

void file_handler(char * name, FrequencyList * fList)
{
	if(name == 0 || fList == 0){
		printf("Error in file_handler\n");
		exit(0);2
	}
	TokenizerT * tk = Tokenize(name);
	while(1)
	{
		char * res = GetToken(tk);
		if(res != 0 && strcmp(res, "") != 0){
			addToList(name, res, fList);
			
		}
		if(res == 0)
			break ; 
	}	

	tkDestroy(tk);
}
