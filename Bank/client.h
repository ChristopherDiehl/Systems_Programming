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

#define TRUE 1
#define FALSE 0

/*functions*/
void error(char *msg);			
void * writeToServer(void * args);	
void * readFromServer(void * args);
int buildSocket();

#endif