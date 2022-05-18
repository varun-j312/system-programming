// Program to create a test UDP client
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
	int mlen; // message length
	socklen_t slen; // server address length
	int ret; // return status of functions

	slen = sizeof(servaddr); // size of socket address
	// clearing garbage value
	memset(&servaddr, 0, slen);
	memset(smsg, 0, MAX);

	servaddr.sin_family = AF_INET; // using internet family of addressing - IPv4
	servaddr.sin_addr.s_addr = inet_addr(ADDRSERV); // setting IP address
	servaddr.sin_port = htons(PORTNO); // setting port number

	sockfd = socket(AF_INET, SOCK_DGRAM, 0); // creating a socket
	handleError(sockfd, "Error: Unable to create socket");

	// sending message to server
	mlen = sendto(sockfd, cmsg, strlen(cmsg), MSG_CONFIRM, (struct sockaddr *)&servaddr, slen);
	cout << "Message sent from Client" << endl;
	// receiving message from server
	mlen = recvfrom(sockfd, smsg, MAX, MSG_WAITALL, (struct sockaddr *)&servaddr, &slen);
	cout << smsg << endl;

	close(sockfd); // closing socket after use

	return 0;
}
