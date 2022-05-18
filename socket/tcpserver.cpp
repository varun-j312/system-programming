// Program to create a test TCP server
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define MAX 256
#define ADDRSERV "127.0.0.1"
#define PORTNO 8009
using namespace std;

int handleError(int status, const char errmsg[])
{
        if(status < 0)
        {
                perror(errmsg);
                exit(EXIT_FAILURE);
        }
        return 0;
}

int main(int argc, char* argv[])
{
	struct sockaddr_in servaddr; // server address
	struct sockaddr_in cliaddr; // client address
	char smsg[MAX] = "Hello from Server to Client"; // server message
	char cmsg[MAX]; // buffer to receive client message
	int sockfd, connfd; // socket and connection variable
	int mlen, slen; // message length, socket address length
	socklen_t clen; // length of client socket address
	int ret; // return status of functions

	slen = sizeof(sockaddr_in); // size of socket address
	// clearing garbage value
	memset(&servaddr, 0, slen);
	memset(&cliaddr, 0, slen);
	memset(cmsg, 0, MAX);

	servaddr.sin_family = AF_INET; // using internet family of addressing - IPv4
	servaddr.sin_addr.s_addr = inet_addr(ADDRSERV); // setting IP address
	servaddr.sin_port = htons(PORTNO); // setting port number

	sockfd = socket(AF_INET, SOCK_STREAM, 0); // creating socket
	handleError(sockfd, "Error: Unable to create socket");
	ret = bind(sockfd, (struct sockaddr *)&servaddr, slen); // binding socket & network info
	handleError(ret, "Error: Unable to bind socket");
	ret = listen(sockfd, 5); // listening for clients
	handleError(ret, "Error: Unable to listen on socket");

	connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clen); // waiting for connection
	handleError(connfd, "Error: Unable to accept connection");

	mlen = recv(connfd, cmsg, MAX, 0); // receive message from client
	cout << cmsg << endl;
	mlen = send(connfd, smsg, strlen(smsg), 0); // send message to client
	cout << "Message sent from Server" << endl;

	close(connfd); // close connection

	return 0;
}
