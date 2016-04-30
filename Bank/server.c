#include "server.h"
#include "simpleList.h"


/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

/*STATIC VARIABLES*/
Account ** bank;
Account * account;
SimpleList  * sl;
int thread_exit;

void sigHandler(int dummy) 
{
    thread_exit = 1;
}

void * sessionAcceptor( void * args)
{

	int sockfd = 0;													 	// file descriptor for our server socket
	int newsockfd = 0;												  // file descriptor for a client socket
	socklen_t clilen = 0;													 // utility variable - size of clientAddressInfo below
	int n = -1;												    		// utility variable - for checking pthread_create return vals
	struct sockaddr_in serverAddressInfo;				     // Super-special secret C struct that holds address info for building our server socket
	struct sockaddr_in clientAddressInfo;					 // Super-special secret C struct that holds address info about our client socket
	int i = 0; 													   // looping variable
	int sl_size = 0;											  // variable used to store size of simple list	
   int iSetOption = 1;										 // variable used to set socket options

   pthread_t id = pthread_self();
	printf("[-]session acceptor running with thread id %lu \n",(unsigned long)id);

	// try to build a socket .. if it doesn't work, complain and exit
   

	thread_exit = 0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
 
	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}

	n = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));

   if (n != 0) 
   {
   	error("[-] ERROR creating socket");
   }

	// zero out the socket address info struct .. always initialize!
	bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));

	// set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
	serverAddressInfo.sin_port = htons(PORT);

	// set a flag to indicate the type of network address we'll be using  
	serverAddressInfo.sin_family = AF_INET;

	// set a flag to indicate the type of network address we'll be willing to accept connections from
	serverAddressInfo.sin_addr.s_addr = INADDR_ANY;

	/** We have an address struct and a socket .. time to build up the server socket **/

	// bind the server socket to a specific local port, so the client has a target to connect to      
	if (bind(sockfd, (struct sockaddr *) &serverAddressInfo, sizeof(serverAddressInfo)) < 0)
	{
		printf("[-] ERROR on binding");

	}

	// set up the server socket to listen for client connections
	listen(sockfd,5);

	clilen = sizeof(clientAddressInfo);


	while (!thread_exit)
	{

		newsockfd = accept(sockfd, (struct sockaddr *) &clientAddressInfo, &clilen);
		pthread_t client_thread;

		if (newsockfd < 0) 
		{
			error("[-] ERROR on accept");
		}

		int * temp_socket = malloc(sizeof(int));
		temp_socket = &newsockfd;
		n = pthread_create(&(client_thread), NULL, &connectionHandler, (void *) &newsockfd); //add pthread id to linked list
		appendToList(client_thread, sl);

	   if (n != 0)
	   {
	   	close(sockfd);
	   	error("\n[-]can't create thread :[%s]"); 
	   }

	}

	//sig int has been called. time to join threads created

	sl_size = getSize(sl);

	for(i = 0; i < sl_size; i++)
	{
		pthread_t temp = getFront(sl);
		pthread_join(temp, NULL);
	}

	return 0;

}

/*This handles client connection && sets up the actual 'server'*/
void * connectionHandler( void * socket)
{

	char buffer[BUFFER_SIZE];										// char array to store data going to and coming from the socket
	int newsockfd = *((int *) socket);  					  // zero out the char buffer to receive a client message
	int closeWithoutMessage = FALSE;							 // int used to determine error message to send to client
	int n = 0;														// utitlity var used for read and write value
   pthread_t id = pthread_self();
	printf("[-]connection handler running with thread id %lu \n",(unsigned long)id);

   while(!thread_exit)
   {
   	bzero(buffer,BUFFER_SIZE);

		// try to read from the client socket
		n = read(newsockfd,buffer,BUFFER_SIZE -1);

		// if the read from the client blew up, complain and exit
		if (n < 0)
		{
			printf("[-] ERROR reading from socket");
			closeWithoutMessage = TRUE;
			break;
		}

		if(strcmp(buffer,"exit") == 0)
		{
			char * exitMessage = "Thank you for banking with us";
			n = write(newsockfd, exitMessage, sizeof(exitMessage));
			closeWithoutMessage = TRUE;
			break;
		}

		printf("Here is the message: %s\n",buffer);

		// try to write to the client socket
		n = write(newsockfd,"I got your message",BUFFER_SIZE);
		// if the write to the client below up, complain and exit

		if (n < 0)
		{
			printf("[-] ERROR writing to the socket");
			closeWithoutMessage = TRUE;
			break;
		}

   }

   ///made it to hear then control+c has been called.. kill the process
   if(closeWithoutMessage == FALSE)
   {
   	char * exitMessage = "The server has been shut down.\n Sorry for any inconvenience\n";
		n = write(newsockfd, exitMessage, sizeof(exitMessage));
   }

	close(newsockfd);
	free(socket);

	return 0;
	
}

void * printBankStatus (void * socket)
{
	int i = 0;

	pthread_t id = pthread_self();
	printf("[-]print bank status running with thread id %lu \n",(unsigned long)id);


	if(bank == NULL)
	{
		printf("[-]Something went horribly wrong. Bank not initialized");
		return 0;
	}
   while(!thread_exit)
   {

   	for(i = 0; i < MAX_CLIENTS; i++)
		{

			if(bank[i] == 0)
			{
				continue;

			} else if(bank[i]->active == TRUE)
			{
				printf("%s, IN SESSION\n",bank[i]->username);
			
			} else if(bank[i]->active == FALSE)
			{
				printf("%s, %f\n",bank[i]->username,bank[i]->balance);

			}
		}

		sleep(20);
   }

	return 0;
}

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	//int sockfd, newsockfd, portno, clilen;
	//struct sockaddr_in serv_addr;
	//struct sockaddr_in cli_addr;

	pthread_t bankthread;																	//prints out bank status
	pthread_t session_acceptor;														  //sets up 'server'
	sl = getList();																		 //stores pthread_t ids of clients for joining
	bank = malloc(sizeof(Account) * MAX_CLIENTS);								//set up bank struct
	//bank->accounts = malloc(sizeof(Account) * MAX_CLIENTS);				  //make room for people
	signal(SIGINT, sigHandler);													 //set control+c call sigHandler
	int err = 0;																		//utitility var used ot check pthread_create output

	err = pthread_create(&(bankthread), NULL, &printBankStatus, NULL);

   if (err != 0)
   {
   	error("\n[-]can't create bank thread "); 
   }

   err = pthread_create(&(session_acceptor), NULL, &sessionAcceptor, NULL);

   if (err != 0)
   {
   	error("\n[-]can't create session acceptor thread "); 
   }

   destroyList(sl);
   free(bank);
   pthread_join(session_acceptor, NULL);
   pthread_join(bankthread, NULL);	


	return 0; 
}
