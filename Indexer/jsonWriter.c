#include "jsonWriter.h"

int jsonWrite(FrequencyList * fList, char * filename) {

  if(filename == 0 || fList == 0){
  	printf("Please enter valid input\n");
  	return 1;
  }
  printf("[-] writing to file\n");

  FILE * file = fopen(filename,"w");
  if(file == 0){
  	printf("[-] error opening file\n");
  	return 1;
  }
  char * start = "{\"list\" : [\n";
  fwrite(start, 1 , strlen(start) , file);
  
  while(isEmpty(fList) == 1){
  	Json * temp = removeFromHead(fList);

    char * formattedtoken = getJsonToken(temp->token);

  	fwrite(formattedtoken,1,strlen(formattedtoken),file);
  	free(formattedtoken);
  	int trailing = temp->trailingNodes;

  	char * record = getJsonRecord(temp->filename, getFrequency(temp->frequency),trailing);
  	fwrite(record,1,strlen(record),file);
  	free(record);
  	destroyJson(temp);
  	if(temp == 0) 
  		break;

  	int i = 0;
  	//take care of nodes with same token but different filenames & frequencies
  	for(i = 0; i < trailing; i ++) {

  		Json * trailingNode = removeFromHead(fList);
  		char * trailingRecord = getJsonRecord(trailingNode->filename, getFrequency(trailingNode->frequency),trailing-i);
		  fwrite(trailingRecord,1,strlen(trailingRecord),file);
  		free(trailingRecord);
  		destroyJson(trailingRecord);
  	}

  	if(isEmpty(fList) == 1){
  		char endFileRecord [] = "\t]},\n";
  		fwrite(endFileRecord,1, strlen(endFileRecord),file);
  	} else {
  		char endFileRecord []= "\t]}\n]}";
  		fwrite(endFileRecord,1, strlen(endFileRecord),file);
  	}

  }
  printf("[-] changes to the file have been written\n");
  //close file
  fclose(file);
}

char * getJsonToken (char * file){
  	char * filename = calloc((sizeof(file) +10),1);
  	int filesize= strlen (file);
  	 filename[0] = '\t';
  	 filename[1] = '{';
  	 filename[2] = '\"';
     strcat(filename,file);
     filename[filesize+3] = '\"';
     filename[filesize+4] = ':';
     filename[filesize+4] = '[';
     filename[filesize+5] = '\n';
     filename[filesize+6] = '\0';


    return filename;

}

char * getFrequency (int frequency){

	char str[15];
	sprintf(str, "%d", frequency);
	char * returnval = malloc(strlen(str));
	strcpy(returnval,str);
	return returnval;
}
//sample record {"baa" : 1}
//trailing appends , to baa
char * getJsonRecord (char * token, char * frequency, int trailing){
	if(trailing == 0){
	  	char * entry = calloc((sizeof(token) + sizeof(frequency) +11),1);
	  	int tokensize= strlen (token);
	  	int freqsize = strlen(frequency);
		entry[0] = '\t';
		entry[1] = '\t';
		entry[2] = '{';
		entry[3] = '\"';
		strcat(entry,token);
		entry[4+tokensize] = '\"';
		entry[5+tokensize] = ' ';
		entry[6+tokensize] = ':'; 
		strcat(entry,frequency);
		entry[7+tokensize+freqsize] = '}'; 
		entry[8+tokensize+freqsize] = '\n';
		entry[9+tokensize+freqsize] = '\0';
		free(frequency);
	    return entry;

	} else{
	  	char * entry = calloc((sizeof(token) + sizeof(frequency) +12),1);
	  	int tokensize= strlen (token);
	  	int freqsize = strlen(frequency);
		entry[0] = '\t';
		entry[1] = '\t';
		entry[2] = '{';
		entry[3] = '\"';
		strcat(entry,token);
		entry[4+tokensize] = '\"';
		entry[5+tokensize] = ' ';
		entry[6+tokensize] = ':'; 
		strcat(entry,frequency);
		entry[7+tokensize+freqsize] = '}'; 
		entry[7+tokensize+freqsize] = ',';
		entry[9+tokensize+freqsize] = '\n';
		entry[10+tokensize+freqsize] = '\0';
		free(frequency);
	    return entry;


	}

}