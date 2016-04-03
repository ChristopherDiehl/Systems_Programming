#include "frequencyList.h"
#include "fileTokenizer.h"
#include "jsonWriter.h"
#include <errno.h>
#include <sys/dir.h>

/*preprocessor*/
 void directory_handle(char * name, FrequencyList * fList);
 void file_handler(char * name, FrequencyList * fList);


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
    printf("Writing\n");
	jsonWrite(fList,argv[1]);
	deleteList(fList);


}	

void directory_handle(char * name, FrequencyList * fList)
{
	printf("%s\n", name);
	DIR *fDir = opendir(name);
	if(fDir == 0){
		return;
	}
	struct dirent * fDirent;
    while ((fDirent = readdir(fDir)) != NULL) {
    	//path can't be longer than 1024 bytes
		char * fullpath = malloc(1024);
		strcat(fullpath,name);
		//strcat(fullpath,fDirent->dname);
		printf("fullpath %s, fDirent->type %d fDirent->name %s name: %s\n",fullpath, fDirent->d_type, fDirent->d_name, name);
    	if(fDirent->d_type == DT_REG){
    		file_handler(fullpath,fList);
    	} //else if(fDirent->d_type == DT_DIR){
    		//printf("Calling directory_handle\n");
    	//	directory_handle(fullpath,fList);
    	//}
    }
    
    closedir(fDir);
}

void file_handler(char * name, FrequencyList * fList)
{
	printf("File Handler name: %s\n",name );
	if(name == 0 || fList == 0){
		printf("Error in file_handler\n");
		exit(0);
	}
	TokenizerT * tk = Tokenize(name);
	if (tk == 0) {
		printf("[-] Tokenizer is null...\n");
		return;
	}
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
