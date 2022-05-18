// Program to create a test TCP client
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
	char cmsg[MAX] = "Hello from Client to Server"; // client message
	char smsg[MAX]; // buffer to receive server message
	int sockfd; // socket variable
	int mlen, slen; // message length, socket address length
	int ret; // return status of functions

	slen = sizeof(sockaddr_in); // size of socket address
	// clearing garbage value
	memset(&servaddr, 0, slen);
	memset(smsg, 0, MAX);

	servaddr.sin_family = AF_INET; // using internet family of addressing - IPv4
	servaddr.sin_addr.s_addr = inet_addr(ADDRSERV); // setting port address
	servaddr.sin_port = htons(PORTNO); // setting port number

	sockfd = socket(AF_INET, SOCK_STREAM, 0); // creating socket
	handleError(sockfd, "Error: Unable to create socket");
	
	ret = connect(sockfd, (struct sockaddr *)&servaddr, slen); // requesting connection to server
	handleError(ret, "Error: Unable to make connection");

	mlen = send(sockfd, cmsg, strlen(cmsg), 0); // send message to server
	cout << "Message sent from Client" << endl;
	mlen = recv(sockfd, smsg, MAX, 0); // receive message from server
	cout << smsg << endl;

	close(sockfd); // closing socket after use

	return 0;
}
