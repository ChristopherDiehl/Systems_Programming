#ifndef JSON_WRITER_H
#define JSON_WRITER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "frequencyList.h"

/*Given a frequency list, and a filename.
 *will write everything in the frequency list in JSON formatting
 */

/*Defines */



int jsonWrite(FrequencyList * fList, char * filename);
char * getJsonToken (char * file);
char * getJsonRecord (char * token, char * frequency, int trailing);
char * getFrequency (int frequency);
int doesFileExist(const char *filename);

#define LEN (CHAR_BIT * sizeof(int) - 1) / 3 + 2;
#define trailing_record() ("},\n\0")
#define not_trailing_record() ("}\n\0")
#define record_first() ("\t\t{\"")
#define record_second() ("\" :")
#define file_token_one() ("\t{\"")
#define file_token_two() ("\":[\n\0")
#define not_empty_record() ("\t]},\n")
#define empty_record() ("\t]}\n")
#define start_token() ("{\"list\" : [\n")
#define trailing() ("}\n\0")

#endif