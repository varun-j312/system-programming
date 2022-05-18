// Program to create a test UDP server
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
	int sockfd; // socket variable
	int mlen, slen; // message length, server address length
	socklen_t clen; // client address length
	int ret; // return status of functions
	
	slen = sizeof(servaddr); // size of server address
	clen = sizeof(cliaddr); // size of client address
	// clearing garbage value
	memset(&servaddr, 0, slen);
	memset(&cliaddr, 0, clen);
	memset(cmsg, 0, MAX);

	servaddr.sin_family = AF_INET; // using internet family of addressing - IPv4
	//servaddr.sin_addr.s_addr = inet_addr(ADDRSERV); // setting IP address
	servaddr.sin_addr.s_addr = INADDR_ANY; // setting IP address
	servaddr.sin_port = htons(PORTNO); // setting port number

	sockfd = socket(AF_INET, SOCK_DGRAM, 0); // creating a socket
	handleError(sockfd, "Error: Unable to create socket");
	ret = bind(sockfd, (struct sockaddr *)&servaddr, slen); // binding socket & network info
	handleError(ret, "Error: Unable to bind socket");
	//ret = listen(sockfd, 5); // listining for clients
	//handleError(ret, "Error: Unable to listen on socket");

	// receiving message from client
	mlen = recvfrom(sockfd, cmsg, MAX, MSG_WAITALL, (struct sockaddr *)&cliaddr, &clen);
	cout << cmsg << endl;
	// sending message to client
	mlen = sendto(sockfd, smsg, strlen(smsg), MSG_CONFIRM, (struct sockaddr *)&cliaddr, clen);
	cout << "Message sent from Server" << endl;

	return 0;
}
