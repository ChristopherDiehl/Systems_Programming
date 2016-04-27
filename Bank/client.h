#ifndef CLIENT_H
#define CLIENT_H


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>



/*functions*/
void error(char *msg);			
void * writeToServer(void * args);	
void * readFromServer(void * args);

#endif