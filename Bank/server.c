#include "server.h"
#include "simpleList.h"
#include "bank.h"


/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

/*STATIC VARIABLES*/
SimpleList  * sl;
int thread_exit;
int server_sock;
bank * bk;  // bk does not have to passed as param as it is global ! 


// Variable to Handle Bank

// create a global variable for the bank so taht it is visible to all the threads !



void sigHandler(int dummy) 
{
	thread_exit = TRUE;
	shutdown(server_sock,SHUT_RDWR);
	close(server_sock);
	printf("THREAD EXIT == TRUE\n");
}

void * sessionAcceptor( void * args)
{

	server_sock = 0;													 	// file descriptor for our server socket
	int newsockfd = 0;												  // file descriptor for a client socket
	socklen_t clilen = 0;													 // utility variable - size of clientAddressInfo below
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

		n = pthread_create(&(client_thread), NULL, &connectionHandler, (void *) &newsockfd); //add pthread id to linked list


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
void * connectionHandler( void * client_socket)
{


	int socket = *(int *)client_socket; 
	char buffer[BUFFER_SIZE];									  // char array to store data going to and coming from the socket
	int closeWithoutMessage = FALSE;							 // int used to determine error message to send to client
	int n = 0;														// utitlity var used for read and write value
	// Used for Banking Information	
	char * accountName ; 
	int sessionOpen = 0 ; 
	char * returnMessage = (char *) calloc(BUFFER_SIZE , 1);
	int account_bound_to_thread = -1 ; 

	pthread_t id = pthread_self();
	printf("[-]connection handler running with thread id %lu \n",(unsigned long)id);

	char * strTok_ptr; // used to store the location by strtok_r

	while(thread_exit != TRUE) 
	{

		bzero(buffer,BUFFER_SIZE);

		// try to read from the client socket
		n = read(socket,buffer,BUFFER_SIZE -1);

		// if the read from the client blew up, complain and exit
		if (n < 0)
		{
			printf("[-] ERROR reading from socket");
			closeWithoutMessage = TRUE;
			break;
		}

		printf("buffer = %s\n",buffer);

		char * command = strtok_r(buffer , " \n\0\t" , &strTok_ptr); 
		printf("[-] command = %s\n",command);

		if( strcmp(command , "open") == 0 )
		{

			if(sessionOpen)
			{
				strcpy(returnMessage,"Session Active : End Session to Open New Account");
			}
			else
			{
				accountName = strtok_r(NULL, " \n\0\t" , &strTok_ptr); 
				printf("acc : %s " , accountName);
				int res = openAccount(bk , accountName, 0); 
				printf("Res : %d \n" , res);
				if(res == 1)
				{
					sessionOpen = 1;
					strcpy(returnMessage,"Account successfuly opened");
					printf("account opened %s\n",accountName);

				}
				else if (res == -2)
				{
					strcpy(returnMessage,"Account with same name already exits. Open new account with different name or start the account ");
				}
				else 
				{
					strcpy(returnMessage, "Error : Cannot support more than 20 accounts ");
				}
		}

		}
		else if( strcmp(command , "start") == 0 )
		{
			if(sessionOpen)
			{
				/*
				 * how to handle client starting a session again 
				 *  1) with a differetn account name ? 
				 *  	> Do we end this current session and switch to a new session 
				 *  	> or give out an error > 
				 *  2) with same account name ? 
				 *  	> Do we not do anything or say that the session is already active ? 
				 */

				 if( strcmp(accountName, strtok_r(NULL , " " , & strTok_ptr) ) == 0 )
				 {
				 	strcpy(returnMessage,"Session Already Active with given Account");
				 }
				 else
				 {
				 	strcpy(returnMessage , "Close Session to create session with new Account");
				 }
			}
				else
				{
					accountName  = strtok_r(NULL , " " , &strTok_ptr);
					printf("[-] start account : %s " , accountName);
					account_bound_to_thread = startAccount(bk , accountName ); 
					
					if(account_bound_to_thread == -2)
					{
						sprintf(returnMessage , "[-] Account %s Already in Session. Wait till out of Session " ,accountName);
					}
					else if (account_bound_to_thread == -1 )
					{
						sprintf(returnMessage , "[-] Account %s , does not exit . Open Account Before Use " ,accountName);
					}
					else
					{
						if(TRUE) printf("account thread # %d" , account_bound_to_thread);
						sprintf(returnMessage , "[-] Account %s Started" ,accountName);
				// OPEN THE SESSION
						sessionOpen = 1; 

					}

				}
		}
		else if( strcmp(command , "credit") == 0 )
		{
			if(sessionOpen)
			{
				char * ammountStr = strtok_r(NULL , " \n\0\t" , &strTok_ptr);
				float ammount = atof(ammountStr); 
				if(TRUE) printf("balance %f\n " , ammount);	
				creditAccount(bk , account_bound_to_thread , ammount); 
				sprintf(returnMessage , "[-] Account Credited With %f" ,ammount);
				//strcpy(returnMessage,strcat("Account Credited with " , ammountStr));
			} else
			{
				strcpy(returnMessage,"Open Session First");
			}
		}

		else if( strcmp(command , "debit") == 0 )
		{
			if(sessionOpen)
			{
				char * ammountStr = strtok_r(NULL , " \n\0\t" , &strTok_ptr);
				float ammount = atof(ammountStr); 

				if(debitAccount(bk , account_bound_to_thread , ammount) == 1)
				{
					sprintf(returnMessage , "[-] Account Debited With %f" ,ammount);
				}
				else
				{
					sprintf(returnMessage , "[-] Insufficeint Funds to Withdraw %f" ,ammount);
				}

			}

			else
			{
				strcpy(returnMessage,"Open Session First");
			}

		}
		else if( strcmp(command , "balance") == 0 )
		{
			if(sessionOpen)
			{
				float bal = balanceAccount(bk , account_bound_to_thread);		
				sprintf(returnMessage , "[-] Balance %f" ,bal);

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
				finishAccount(bk , account_bound_to_thread);			
			// session ends here !
				sessionOpen = 0 ; 
				account_bound_to_thread = -1; 
				accountName = NULL; 
				sprintf(returnMessage , "[-] Current Session Closed ");
			}
			else
			{
				sprintf(returnMessage , "[-] Open Session First");
			}
		}

		else if( strcmp(command , "exit") == 0 )
		{
			closeWithoutMessage = TRUE;
			strcpy(returnMessage, "Thank you for banking with us");
			//free(command); // DONT CALL FREE NO MEM ALLOC
			n = write(socket,returnMessage,255);

			if (n < 0)
			{
				printf("[-] ERROR writing to the socket");
				closeWithoutMessage = TRUE;
				break;
			}

			return 0 ; 
		}
		else
		{
			printf("Incorrect Input");
		}

		//free(command); // DONOT CALL FREE STRTOK DOES NOT ALLOCATE MEM
		n = write(socket,returnMessage,BUFFER_SIZE);

		if (n < 0)
		{
			printf("[-] ERROR writing to the socket");
			closeWithoutMessage = TRUE;
			break;
		}
	
		memset(returnMessage , 0 , BUFFER_SIZE);

	} // end of while loop

		///made it to hear then control+c has been called.. kill the process
	if(closeWithoutMessage == FALSE)
	{
		char * exitMessage = "The server has been shut down.\n Sorry for any inconvenience\n";
		n = write(socket, exitMessage, strlen(exitMessage) +1);
	}

	close(socket);

	return 0;

}

void * printBankStatus (void * socket)
{
	int i = 0;

	pthread_t id = pthread_self();
	printf("[-] Print bank status running with thread id %lu \n",(unsigned long)id);


	if(bk == NULL)
	{
		printf("[-]Something went horribly wrong. Bank not initialized");
		return 0;
	}
	while(thread_exit != TRUE)
	{	

		for(i = 0; i < MAX_CLIENTS; i++)
		{

			if(bk->accountArray[i] == 0)
			{
				continue;

			} else if(bk->accountArray[i]->_inUse == TRUE)
			{
				printf("%s, IN SESSION\n",bk->accountArray[i]->_name);

			} else if(bk->accountArray[i]->_inUse == FALSE)
			{
				printf("%s, %f\n",bk->accountArray[i]->_name , bk->accountArray[i]->_balance);

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
	signal(SIGINT, sigHandler);													 //set control+c call sigHandler
	int err = 0;	
	bk = createBank();											

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


	return 0; 
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
