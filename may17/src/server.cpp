// Program to create a TCP server
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../inc/func.h"
#define MAX 256
#define PORTNO 8009
#define ADDRSERV "0.0.0.0"
using namespace std;

// main funtion that creates a TCP server
int main()
{
	int sockfd; // TCP socket id
	int connfd; // TCP connection id
	char smsg[MAX], cmsg[MAX]; // buffer to store server and client messages
	struct sockaddr_in cliaddr; // variable to store clients address
	socklen_t clen; // to store length of clients address
	pid_t client_pid; // to store process id
	int mlen; // to store message length

	clen = sizeof(cliaddr); // calculating size of client address
	memset(&cliaddr, 0, clen);

	sockfd = create_tcp_socket(ADDRSERV, PORTNO); // creating a TCP socket
	handle_error(sockfd, "Unable to create TCP socket");

	create_shared_memory(); // creating a shared memory
	while(1)
	{
		connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clen); // waiting for connection
                handle_error(connfd, "Error: Unable to accept connection");

		cout << "No. of connections: " << attach_shared_memory(1) << endl; // displaying no. of connections

		client_pid = fork();
		if(client_pid == 0)
		{
			close(sockfd);
			while(1)
			{
				// clearing message buffers
				memset(smsg, 0, MAX);
				memset(cmsg, 0, MAX);

				mlen = read(connfd, cmsg, MAX); // reading client message
                        	write(1, "Client: ", 8);
                        	write(1, cmsg, strlen(cmsg)); // displaying client message

				if(strcmp("Bye\n", cmsg) == 0) // check for bye message
					break;

                        	write(1, "Enter message for client: ", 26);
                        	mlen = read(0, smsg, MAX); // reading server message through keyboard
                        	mlen = write(connfd, smsg, strlen(smsg)); // sending server message to client

                        	write(1, "---Message sent from server---\n", 32);

				if(strcmp("Bye\n", smsg) == 0) // check for bye message
					break;
			}
			close(connfd); // close connection
			cout << "No. of connections: " << attach_shared_memory(2) << endl; // displaying no. of connections
			exit(EXIT_SUCCESS);
		}
		close(connfd); // close connection
	}
	destroy_shared_memory(); // destroying shared memory

	exit(EXIT_SUCCESS);
}
