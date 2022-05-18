// Program to create a TCP client
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../inc/func.h"
#define MAX 256
#define PORTNO 8009
#define ADDRCLI "127.0.0.1"
using namespace std;

// main funtion that creates a TCP client
int main()
{
	int sockfd; // TCP socket id
	char smsg[MAX], cmsg[MAX]; // buffer to store server and client messages
	struct sockaddr_in servaddr; // variable to store clients address
	socklen_t slen; // to store length of clients address
	int mlen; // to store message length
	int ret; // to store return status of functions

	slen = sizeof(servaddr); // calculating size of client address
	memset(&servaddr, 0, slen);

	sockfd = create_tcp_socket(ADDRCLI, PORTNO); // creating a TCP socket
	handle_error(sockfd, "Unable to create TCP socket");

	servaddr.sin_family = AF_INET; // using internet family of addressing - IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY; // setting IP address
        servaddr.sin_port = htons(PORTNO); // setting port number

	ret = connect(sockfd, (struct sockaddr *) &servaddr, slen); // requesting connection to server
	handle_error(ret, "Unable to make connection");

	cout << "Connection no.: " << attach_shared_memory(0) << endl;
	while(1)
	{
		// clearing message buffers
		memset(smsg, 0, MAX);
        	memset(cmsg, 0, MAX);

		write(1, "Enter message for server: ", 26);
        	mlen = read(0, cmsg, MAX); // reading client message through keyboard
        	mlen = write(sockfd, cmsg, strlen(cmsg)); // sending client message to server

		if(strcmp("Bye\n", cmsg) == 0) // checking for bye message
			break;

        	write(1, "---Message sent from client---\n", 32);

        	mlen = read(sockfd, smsg, MAX); // reading server message
        	write(1, "Server: ", 8);
        	write(1, smsg, strlen(smsg)); // displaying server message

		if(strcmp("Bye\n", smsg) == 0) // checking for bye message
			break;
	}
	close(sockfd); // closing socket

	exit(EXIT_SUCCESS);
}
