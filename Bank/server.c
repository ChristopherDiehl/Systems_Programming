#include "server.h"
#include "simpleList.h"


/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
//need to transport socket over heap
/*STATIC VARIABLES*/
Account ** bank;
Account * account;
SimpleList  * sl;
int thread_exit;
int server_sock;
int numofclients;

void sigHandler(int dummy) 
{
    thread_exit = TRUE;
    shutdown(server_sock,SHUT_RDWR);
    close(server_sock);
}

void * sessionAcceptor( void * args)
{

	server_sock = 0;													 	// file descriptor for our server socket
	int newsockfd = 0;												  // file descriptor for a client socket
	socklen_t clilen = 0;											 // utility variable - size of clientAddressInfo below
	int n = -1;												    		// utility variable - for checking pthread_create return vals
	struct sockaddr_in serverAddressInfo;				     // Super-special secret C struct that holds address info for building our server socket
	struct sockaddr_in clientAddressInfo;					 // Super-special secret C struct that holds address info about our client socket
	int i = 0; 													   // looping variable
	int sl_size = 0;											  // variable used to store size of simple list	
   int iSetOption = 1;										 // variable used to set socket options

   pthread_t id = pthread_self();
	printf("[-] Session acceptor running with thread id %lu \n",(unsigned long)id);

	// try to build a socket .. if it doesn't work, complain and exit
   

	thread_exit = 0;
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
 
	if (server_sock < 0)
	{
		error("ERROR opening socket");
	}

	n = setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));

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
	if (bind(server_sock, (struct sockaddr *) &serverAddressInfo, sizeof(serverAddressInfo)) < 0)
	{
		printf("[-] ERROR on binding");

	}

	printf("[-] Server running on port %d\n",PORT);

	// set up the server socket to listen for client connections
	listen(server_sock,5);

	clilen = sizeof(clientAddressInfo);


	while (thread_exit != TRUE)
	{
		newsockfd = accept(server_sock, (struct sockaddr *) &clientAddressInfo, &clilen);
		if(thread_exit == TRUE)
		{
			break;
		}
		pthread_t client_thread;

		if (newsockfd < 0) 
		{
			error("[-] ERROR on accept");
		}

	//	int * temp_socket = malloc(sizeof(int));
		//temp_socket = &newsockfd;
		n = pthread_create(&(client_thread), NULL, &connectionHandler, (void *) &newsockfd); //add pthread id to linked list
		
		appendToList(client_thread, sl);

	   if (n != 0)
	   {
	   	close(server_sock);
	   	error("\n[-]can't create thread :[]\n"); 
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
	int array_number = -1;
   pthread_t id = pthread_self();
	int sessionOpen = 0 ; 
	char * returnMessage = calloc(BUFFER_SIZE , 1);
	char * strTok_ptr; // used to store the location by strtok_r

	printf("[-]Client accepted. Running with thread id %lu \n",(unsigned long)id);

   while(thread_exit != TRUE)
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
			n = write(newsockfd, exitMessage, strlen(exitMessage)+1);
			closeWithoutMessage = TRUE;
			break;
		}

		char * command = strtok_r(buffer , " \n\0\t" , &strTok_ptr); 

		if( strcmp(command , "open") == 0 )
		{

			if(sessionOpen)
			{
				strcpy(returnMessage,"Session Active : End Session to Open New Account \n");
			}

			else if(numofclients == 19)
			{
				strcpy(returnMessage, "Error : Cannot support more than 20 accounts \n");
			}

			else
			{

				char * accountName = strtok_r(NULL, " \n\0\t" , &strTok_ptr); 
				printf("acc : %s \n" , accountName);
				printf("numofclients: %d\n",numofclients);
				if(checkForDuplicateAccount(accountName) == TRUE)
				{ //new account
					bank[numofclients] = malloc(sizeof(Account));
					bank[numofclients]->username = malloc(BUFFER_SIZE);
					strcpy(bank[numofclients]->username,accountName);
					bank[numofclients]->balance = 0;
					bank[numofclients]->active = TRUE;
					array_number = numofclients;
					numofclients++;
					sessionOpen = 1;
				}
				else 
				{
					strcpy(returnMessage,"Account with same name already exits. Open new account with different name or start the account \n");
				}
			}
		}

		else if( strcmp(command , "start") == 0 )
		{
			if(sessionOpen)
			{
				strcpy(returnMessage , "Please finish account before trying to start a different account\n");
			}
			else
			{
				char * accountName  = strtok_r(NULL , " " , &strTok_ptr);
				printf("[-] start account : %s \n" , accountName);
				int check = checkForOpenAccount(accountName);
				if(check != -1 && check !=  -2)
				{ //safe to start
					sessionOpen = TRUE;
					array_number = check;
					bank[array_number]->active = TRUE;
				}
				else if (check == -1)
				{
					strcpy(returnMessage , "Account is currently in session\n");

				}
				else if(check == -2)
				{
					strcpy(returnMessage , "There is no account in that name currently opened\n");

				}
			}
		}

		else if( strcmp(command , "credit") == 0 )
		{
			if(sessionOpen)
			{
				char * amountStr = strtok_r(NULL , " \n\0\t" , &strTok_ptr);
				float amount = atof(amountStr); 
				printf("balance %f\n ", amount);	
				bank[array_number]->balance += amount;
				sprintf(returnMessage , "[-] Account Credited With %f\n" ,amount);

			} 
			else
			{
				strcpy(returnMessage,"Open Session First\n");
			}
		}

		else if( strcmp(command , "debit") == 0 )
		{
			if(sessionOpen)
			{
				char * amountStr = strtok_r(NULL , " \n\0\t" , &strTok_ptr);
				float amount = atof(amountStr); 
				printf("balance %f\n ", amount);	
				float temp = bank[array_number]->balance;
				if (temp += amount < 0)
				{
					sprintf(returnMessage , "[-] Insufficient funds at this time %f\n" ,amount);
				} 
				else
				{
					bank[array_number]->balance += amount;
					sprintf(returnMessage , "[-] Account Credited With %f\n" ,amount);
				}

			} else
			{
				strcpy(returnMessage,"Open Session First\n");
			}

		}

		else if( strcmp(command , "balance") == 0 )
		{
			if(sessionOpen)
			{
					
				sprintf(returnMessage , "[-] Balance %f" ,bank[array_number]->balance);

			}
			else
			{
				sprintf(returnMessage , "[-] Open Session First");
			}
		}

		else if( strcmp(command , "finish") == 0 )
		{
			if(sessionOpen)
			{

				sessionOpen = 0 ; 
				bank[array_number]->active = FALSE;
				array_number = -1;
				sprintf(returnMessage , "[-] Current Session Closed ");
			}
			else
			{
				sprintf(returnMessage , "[-] Open Session First");
			}
		}

		else
		{
			printf("Invalid Input");
			strcpy(returnMessage, "[-] Invalid input\n");
		}

		n = write(*(int *)socket,returnMessage,BUFFER_SIZE);

		if (n < 0)
		{
			printf("[-] ERROR writing to the socket");
			closeWithoutMessage = TRUE;
			break;
		}
		
		memset(returnMessage , 0 , BUFFER_SIZE);


   }

   ///made it to hear then control+c has been called.. kill the process
   if(closeWithoutMessage == FALSE)
   {
   	char * exitMessage = "The server has been shut down.\n Sorry for any inconvenience\n";
		n = write(newsockfd, exitMessage, strlen(exitMessage) +1);
   }

	close(newsockfd);
	//free(socket);

	return 0;
	
}
//returns -1 if in use
// returns -2 if not open 
int checkForOpenAccount(char * accountName)
{
	int i = 0;
	for(i = 0; i < MAX_CLIENTS; i++)
	{

		if(bank[i] == 0)
			{continue;}

		else if(strcmp(bank[i]->username,accountName) == 0 )
		{
			if(bank[i]->active == TRUE)
			{
				return -1;
			}
			
			return i;
		}

	}
	return -2;
}
//returns true if there is no duplicate
int checkForDuplicateAccount(char * accountName)
{
	int i = 0;
	for(i = 0; i < MAX_CLIENTS; i++)
	{
		if(bank[i] == 0 || bank[i]->username == 0)
			{continue;}

		else if(strcmp(bank[i]->username,accountName) == 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}

void * printBankStatus (void * socket)
{
	int i = 0;

	pthread_t id = pthread_self();
	printf("[-] Print bank status running with thread id %lu \n",(unsigned long)id);


	if(bank == NULL)
	{
		printf("[-]Something went horribly wrong. Bank not initialized");
		return 0;
	}
   while(thread_exit != TRUE)
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

//// TOKEN EXTRACTION ////////////
int __strncpy(char * dest, char * src , int len)
{
	int i ;
	for(i = 0 ; i < len ; ++i)
	{
		*(dest + i) = *(src + i);
	}
	dest[len] = '\0';	
	return 0 ; 
}

char * tokenSpace(char * str , int * len ) // len gives the position of the string we have process
{
	if( *len != 0)
		len++; // ignore the space that we saw earlier ! 
	char * p = str + *len;  
	char * startOfStrTk = p ; 
	int tokLen = 0 ; 
	int strlength = strlen(str);
	printf("str: %s\n",str);
	while(*p != '\0' && *len < strlength)
	{
		if(isspace(*p))
		{
			break ; 	
		}
		printf("%c" ,*p);
		tokLen++ ; 
	}
	char * res = (char *) malloc( tokLen + 1) ; 	 // free this memory !! 
	__strncpy(res , startOfStrTk , tokLen);  
	printf("Res: %s\n",res);
	return res ; 
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
	signal(SIGINT, sigHandler);													 //set control+c call sigHandler
	int err = 0;																		//utitility var used ot check pthread_create output
	numofclients = 0;
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

   pthread_join(bankthread, NULL);	
   pthread_join(session_acceptor, NULL);


   destroyList(sl);
   free(bank);



	return 0; 
}