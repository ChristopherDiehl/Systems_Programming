#include "client.h"

/*Static Variables*/
pthread_t threads[2];
int end = 0;
char command_buffer[256];								 // char array to store data  going to the server
char response_buffer[256];								// char array to store data  coming from the server
int thread0Active;
int thread1Active;
int err = -1;


int main(int argc, char *argv[])
{
	
   int sockfd = -1;													// file descriptor for our socket
   int portno = -1;												  // server port to connect to
   struct sockaddr_in serverAddressInfo;					 // Super-special secret C struct that holds address info for building our socket
   struct hostent *serverIPAddress;							// Super-special secret C struct that holds info about a machine's address

   thread1Active = 0;
   thread0Active = 0;

   if (argc < 3)
   {
   	fprintf(stderr,"[-] Usage %s (hostname) (port)\n", argv[0]);
   	exit(0);
   }

   portno = atoi(argv[2]);

   serverIPAddress = gethostbyname(argv[1]);
   if (serverIPAddress == NULL)
   {
   	fprintf(stderr,"[-] ERROR, no such host\n");
   	exit(0);
   }

   sockfd = buildSocket();


	/** We now have the IP address and port to connect to on the server, we have to get    **/
	/**   that information into C's special address struct for connecting sockets                     **/

	// zero out the socket address info struct .. always initialize!
   bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));

	// set a flag to indicate the type of network address we'll be using 
   serverAddressInfo.sin_family = AF_INET;

	// set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
   serverAddressInfo.sin_port = htons(portno);

	// do a raw copy of the bytes that represent the server's IP address in 
	//   the 'serverIPAddress' struct into our serverIPAddressInfo struct
   bcopy((char *)serverIPAddress->h_addr, (char *)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);

   while(TRUE) 
   {
   	if(connect(sockfd,(struct sockaddr *)&serverAddressInfo,sizeof(serverAddressInfo)) == 0)
   	{
   		printf("[-] We are connected to the server!\n");
   		break;
   	}
   	close(sockfd);
   	printf("[-] ERROR connecting\n");
   	sockfd = buildSocket();
   	sleep(3);		//wait 3 seconds before trying again
   }

	/** If we're here, we're connected to the server .. w00t!  Time to multithread**/
   err = pthread_create(&(threads[0]), NULL, &writeToServer, (void *) &sockfd);

   if (err != 0)
   {
   	close(sockfd);
   	error("\n[-]can't create thread :[]"); 
   }

   thread0Active = 1;
   err = pthread_create(&(threads[1]), NULL, &readFromServer, (void *) &sockfd);

   if (err != 0)
   {
   	close(sockfd);
   	error("\n[-]can't create thread :[]"); 
   }

   thread1Active = 1;


   pthread_join(threads[0], NULL);
   pthread_join(threads[1], NULL);	

   return 0;
}

int buildSocket() 
{

	int iSetOption = 1;
	// try to build a socket .. if it doesn't work, complain and exit
   int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sockfd < 0) 
   {
   	error("[-] ERROR creating socket");
   }

   err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));
   if (sockfd < 0) 
   {
   	error("[-] ERROR creating socket");
   }
 	return sockfd;
}



void * writeToServer(void * args)
{
	printf("[-] Thread created to write to server\n");

	int sockfd = *((int *) args);
	int n = -1;
	while(end != 1)
	{
		printf("[-] Please enter message: ");
		// zero out the message buffer
		bzero(command_buffer,256);

		// get a message from the client
		fgets(command_buffer,255,stdin);
		command_buffer[strcspn(command_buffer, "\n")] = '\0';

		if(strcmp(command_buffer,"exit") == 0)
		{
			end = 1;
		}

		// try to write it out to the server
		n = write(sockfd,command_buffer,strlen(command_buffer));

		if (n < 0)
		{
			close(sockfd);
			error("ERROR writing to socket\n");
		}

		if(end == 1)
		{
			break;
		}

		sleep(2);
	}
	return 0;
}

void * readFromServer(void * args)
{

	printf("[-] Thread created to read from server\n");
	int sockfd = *((int *) args);
	int n = -1;

	while(end != 1)
	{
		bzero(response_buffer,256);

		// read a message from the server into the buffer
		n = read(sockfd,response_buffer,255);

		// if we couldn't read from the server for some reason, complain and exit
		if (n < 0)
		{
			close(sockfd);
			error("[-] ERROR reading from socket\n");
		}

		// print out server's message
		printf("%s\n",response_buffer);
	}

	return 0;
}


void error(char *msg)
{

	if(thread0Active == 1)
	{
		pthread_join(threads[0], NULL);
	}
	
	if(thread1Active == 1)
	{
		pthread_join(threads[1], NULL);
	}

	perror(msg);
	exit(0);
}
