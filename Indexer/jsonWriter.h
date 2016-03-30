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
int jsonWrite(FrequencyList * fList, char * filename);
char * getJsonToken (char * file);
char * getJsonRecord (char * token, char * frequency);
char * getFrequency (int frequency);

#define LEN (CHAR_BIT * sizeof(int) - 1) / 3 + 2;

#endif