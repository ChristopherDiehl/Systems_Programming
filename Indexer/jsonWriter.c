#include "jsonWriter.h"
#include "frequencyList.h"
#include "fileTokenizer.h"
/* Writes a frequency list tov a file given into it by filename
 *BY CALLING jsonWRITE YOU DELETE THE FREQUENCY LIST
 *DO NOT CALL THEN EXPECT TO use Frequency List later
 */
int jsonWrite(FrequencyList * fList, char * filename) {

  if(filename == 0 || fList == 0){
  	printf("Please enter valid input\n");
  	return 1;
  }
  printf("[-] writing to %s\n",filename);

  //open up the file
  FILE * file = fopen(filename,"w");
  if(file == 0){
  	printf("[-] error opening file\n");
  	return 1;
  }
  //write starting part of file
  char * start = "{\"list\" : [\n";
  fwrite(start, 1 , strlen(start) , file);
  int trailing = 0;

  while(isEmpty(fList) == 1){
    //start removing from list
  	Json * temp = removeFromHead(fList);
   // printList(fList);
    if(trailing == 0){
    //  printf("trailing = =0\n");
      char * formattedtoken = getJsonToken(temp->token);
      fwrite(formattedtoken,1,strlen(formattedtoken),file);
      free(formattedtoken);
     // printf("past free\n");
    }
    //see if node has any trailing nodes
    trailing = temp->isTrailing;
  //  printf("going into record\n");
  	char * record = getJsonRecord(temp->filename, getFrequency(temp->frequency),trailing);
  	fwrite(record,1,strlen(record),file);
  	free(record);
    //printf("freeing  record\n");
  	destroyJson(temp);
   // printf("freed JSon\n");
  	if(temp == 0) 
  		break;
    if(trailing == 0){
      if(isEmpty(fList) == 1){
        char endFileRecord [] = "\t]},\n";
        fwrite(endFileRecord,1, strlen(endFileRecord),file);
      } else {
        char endFileRecord []= "\t]}\n]}";
        fwrite(endFileRecord,1, strlen(endFileRecord),file);
      }

    }

  }
  printf("[-] changes to the file have been written\n");
  if(file != NULL)
	fclose(file); 
  printf("CANT CLOSE?");
}

//formats the Json token :  {"token"[
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


//parses int frequency int o char *
char * getFrequency (int frequency){
	char str[16];
	sprintf(str, "%d", frequency);
	char * returnval = malloc(strlen(str));
	strcpy(returnval,str);
	return returnval;
}
//sample record {"baa" : 1}
//trailing appends , to baa
//returns a record
char * getJsonRecord (char * token, char * frequency, int trailing){
  char * entry = 0;
	if(trailing == 0){
      //this handles the 
   entry = calloc((sizeof(token) + sizeof(frequency) +11),1);
  }else {
      //make room for extra comma
   entry = calloc((sizeof(token) + sizeof(frequency) +12),1);
  }

  	int tokensize= strlen (token) +1;
  	int freqsize = strlen(frequency) +1;
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

    if(trailing == 0){
     //dont add that extra comma
     entry[8+tokensize+freqsize] = '\n';
     entry[9+tokensize+freqsize] = '\0';

    } else {
      //put that extra comma in
      entry[8+tokensize+freqsize] = ',';
      entry[9+tokensize+freqsize] = '\n';
      entry[10+tokensize+freqsize] = '\0';
    }

		free(frequency);
    return entry;

}

int main (int argc, char **argv){

  if(argc !=3)
  {
    printf("[-]Please enter valid args\n" );
    return -1 ; 
  } 

  TokenizerT * tk = Tokenize(argv[2]);
  FrequencyList * fList = getFrequencyList();
  while(1)
  {
    char * res = GetToken(tk);
    if(res != 0 && strcmp(res, "") != 0){
      //printf("Trying to add \"%s\" from %s to list\n", res, argv[1]);
      addToList(argv[2], res, fList);

      
    }
    if(res == 0)
      break ; 
    //printf("%s\n",res);

  } 
  jsonWrite(fList,argv[1]);

  deleteList(fList);
  tkDestroy(tk);

  return 0;
}
