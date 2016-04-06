#include "jsonWriter.h"

/* Writes a frequency list tov a file given into it by filename
 *BY CALLING jsonWRITE YOU DELETE THE FREQUENCY LIST
 *DO NOT CALL THEN EXPECT TO use Frequency List later
 */

int jsonWrite(FrequencyList * fList, char * filename) 
{

  if(filename == 0 || fList == 0)
  {
  	printf("Please enter valid input\n");
  	return 1;
  }

  printf("[-] writing to %s\n",filename);

  //open up the file
  FILE * file = 0; 

  if(doesFileExist(filename))
  {
   
    printf("[-] %s already exists. Enter [1] to append or [0] to overwrite\n",filename );
    int n;
    int append = scanf ("%d",&n);
   
    if (append == EOF ) 
    {
      printf("[-] invalid arguments, overwriting file");
      file = fopen(filename,"w");

    } 

    else if(append)
    {
       file = fopen(filename,"a");
    } 

  }
  else 
  {
     file = fopen(filename,"w");
  }

   
  if(file == 0)
  {
  	printf("[-] error opening file\n");
  	return 1;

  }
  //write starting part of file
  fwrite(start_token(), 1 , strlen( start_token()) , file);
  int trailing = 0;

  while(isEmpty(fList) == 1){

  	Json * temp = removeFromHead(fList);
    if(trailing == 0)
    {
      char * formattedtoken = getJsonToken(temp->token);
      fwrite(formattedtoken,1,strlen(formattedtoken),file);
      free(formattedtoken);
    }

    trailing = temp->isTrailing;

  	char * record = getJsonRecord(temp->filename, getFrequency(temp->frequency),trailing);
  	fwrite(record,1,strlen(record),file);
  	free(record);
  	destroyJson(temp);

  	if(temp == 0) {
  		break;
    }

    if(trailing == 0)
    {
      if(isEmpty(fList) == 1)
      {
        fwrite(not_empty_record(),1, strlen(not_empty_record()),file);
      } 
      else 
      {
        fwrite(empty_record(),1, strlen(empty_record()),file);
      }

    }

  }

  printf("[-] changes to the file have been written\n");
  if(file != NULL) {
	 fclose(file); 
  }
}

//formats the Json token :  {"token"[
char * getJsonToken (char * file)
{
	char * filename = calloc((strlen(file) +12),1);
	int filesize= strlen (file);

  strcat(filename,file_token_one());
  strcat(filename,file);
  strcat(filename,file_token_two());



  return filename;

}


//parses int frequency int o char *
char * getFrequency (int frequency)
{
	char str[16];
	sprintf(str, "%d", frequency);
	char * returnval = calloc(strlen(str)+1,1);
	strcpy(returnval,str);

	return returnval;
}

//sample record {"baa" : 1}
//trailing appends , to baa
//returns a record
char * getJsonRecord (char * token, char * frequency, int trailing) 
{

  if(token == 0 || frequency == 0)
  {
    return "Token or frequency == 0";
  }

  char * entry = 0;
  entry = calloc((strlen(token) + strlen(frequency) +30),1);

	int tokensize= strlen (token);
	int freqsize = strlen(frequency);

  strcat(entry,record_first());
	strcat(entry,token);
  strcat(entry,record_second());
	strcat(entry,frequency);

  if(trailing == 0)
  {
   strcat(entry,not_trailing_record());
  } 

  else
  {
    strcat(entry,trailing_record());
  }

	free(frequency);
  return entry;

}

/*Used to determine if the file to write to already exists*/
int doesFileExist(const char *filename) 
{
  FILE * fp = fopen(filename,"r");

  if (fp != NULL)
  {
    fclose(fp);
    printf("File exists\n");
    return 1;
  }
  return 0;
}
