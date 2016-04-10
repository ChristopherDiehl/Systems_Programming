#include "frequencyList.h"
#include "fileTokenizer.h"
#include "jsonWriter.h"
#include <errno.h>
#include <sys/dir.h>

/*preprocessor*/
void directory_handle(char * name, FrequencyList * fList);
void file_handler(char * name, FrequencyList * fList);
void delete_file_paths();

char ** filepaths;
int filepath_count;
int filepath_index;
//it works... just need to get refernce to name
int main(int argc, char **argv) {

	filepaths = malloc(5 *sizeof(char *));
	filepath_count = 5;
	filepath_index = 0;
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
   // printList(fList);
	jsonWrite(fList,argv[1]);
	deleteList(fList);
	//free(fList);
	delete_file_paths();
	if(fDir!=0)
     closedir(fDir);

	return 1;
}	

void delete_file_paths() {
	int i = 0;
	for (i = 0; i < filepath_index; i ++){
		free(filepaths[i]);
	}
	free(filepaths);
}

void directory_handle(char * name, FrequencyList * fList)
{
	DIR *fDir = opendir(name);
	if(fDir == 0){
		return;
	}
	struct dirent * fDirent;
    while ((fDirent = readdir(fDir)) != NULL) {
    	//path can't be longer than 1024 bytes
		char * fullpath = calloc(1,1024);
		if((strcmp(fDirent->d_name, ".") == 0) || (strcmp (fDirent->d_name,"..") ==0)){
			free(fullpath);
			continue;
		}
		strcpy(fullpath,name);
		strcat(fullpath,"/");
		//printf("fp %s\n", fullpath);
		strcat(fullpath,fDirent->d_name);
    	if(fDirent->d_type == DT_REG){
    		file_handler(fullpath,fList);
    	} else if(fDirent->d_type == DT_DIR){
    		directory_handle(fullpath,fList);
    	}
    	///need to handle fullpaths
    	free(fullpath);
    }
    
    closedir(fDir);
}

void file_handler(char * name, FrequencyList * fList)
{

	if(filepath_index == (filepath_count)){
		//expand filepaths by a multiplicity of 2
		char ** temp = malloc(filepath_count *2 * sizeof(char *));
		int i = 0;
		for (i = 0; i < filepath_count; i ++){
			temp[i] = filepaths[i];
		}
		free(filepaths);
		filepaths = temp;
		filepath_count = filepath_count * 2;
	}

	filepaths[filepath_index] = malloc(strlen(name)+1);
	strcpy(filepaths[filepath_index],name);
	//free(name);
	if(filepaths[filepath_index] == 0 || fList == 0){
		printf("Error in file_handler\n");
		exit(0);
	}

	TokenizerT * tk = Tokenize(filepaths[filepath_index]);
	if (tk == 0) {
		printf("[-] Tokenizer is null...\n");
		return;

	}
	while(1)
	{
		char * res = GetToken(tk);
		if(res != 0 && strcmp(res, "") != 0){
			addToList(filepaths[filepath_index], res, fList);
			
		}
		if(res == 0)
			break ; 
	}	
	//printList(fList);
	tkDestroy(tk);
	filepath_index++;
}
