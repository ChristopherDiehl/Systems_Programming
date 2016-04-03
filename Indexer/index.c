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
    closedir(fDir);
    printf("Writing\n");
	jsonWrite(fList,argv[1]);
	deleteList(fList);
	delete_file_paths();
	return 1;
}	

void delete_file_paths() {
	int i = 0;
	for (i = 0; i < filepath_index; i ++){
		printf("%s\n", filepaths[i]);
		free(filepaths[i]);
	}
	free(filepaths);
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
    	printf("while\n");
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
		printf("fullpath %s, fDirent->type %d fDirent->name %s name: %s\n",fullpath, fDirent->d_type, fDirent->d_name, name);
    	if(fDirent->d_type == DT_REG){
    		file_handler(fullpath,fList);
    	} else if(fDirent->d_type == DT_DIR){
    		directory_handle(fullpath,fList);
    	}
    	//free(fullpath);
    }
    
    closedir(fDir);
}

void file_handler(char * name, FrequencyList * fList)
{

	if(filepath_index == (filepath_count)){
		//expand filepaths by a multiplicity of 2
		printf("segfault?\n");
		char ** temp = malloc(filepath_count *2 * sizeof(char *));
		int i = 0;
		printf("filepath_count %d\n", filepath_count );
		for (i = 0; i < filepath_count; i ++){
			printf("%d filepath: %s\n",i, filepaths[i]);
			temp[i] = malloc(strlen(filepaths[i])+1);
			strcpy(temp[i],filepaths[i]);
			//free(filepaths[i]);
		}
		printf("out of for loop\n");
		free(filepaths);
		filepaths = temp;
		filepath_count = filepath_count * 2;
		printf("no segfault\n");
	}
	filepaths[filepath_index] = malloc(strlen(name)+1);
	strcpy(filepaths[filepath_index],name);
	printf("File Handler name: %s\n", filepaths[filepath_index]);
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
		free(res);
	}	
	//printList(fList);
	tkDestroy(tk);
	filepath_index++;
}
