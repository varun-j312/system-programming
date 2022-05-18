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
        char smsg[MAX], cmsg[MAX]; // buffer to send & receive messages
        int sockfd; // socket variable
        int mlen, slen; // message length, socket address length
        //socklen_t serverAddrLen; // length of server address
        int ret; // return status of functions

        slen = sizeof(sockaddr_in); // size of socket address
        // clearing garbage value
        memset(&servaddr, 0, slen);
        memset(smsg, 0, MAX);
        memset(cmsg, 0, MAX);

        servaddr.sin_family = AF_INET; // using internet family of addressing - IPv4
        servaddr.sin_addr.s_addr = inet_addr(ADDRSERV); // setting IP address
        servaddr.sin_port = htons(PORTNO); // setting port number

        sockfd = socket(AF_INET, SOCK_STREAM, 0); // creating socket
        handleError(sockfd, "Error: Unable to create socket");

        ret = connect(sockfd, (struct sockaddr *)&servaddr, slen); // requesting connection to server
        handleError(ret, "Error: Unable to make connection");

	while(1)
	{
        	memset(smsg, 0, MAX);
        	memset(cmsg, 0, MAX);

        	write(1, "Enter message for server: ", 26);
        	mlen = read(0, cmsg, MAX); // reading client message through keyboard
        	mlen = write(sockfd, cmsg, strlen(cmsg)); // sending client message to server

		if(strcmp("BYE\n", cmsg) == 0)
			break;

        	write(1, "---Message sent from client---\n", 32);

        	mlen = read(sockfd, smsg, MAX); // reading server message
        	write(1, "Server: ", 8);
        	write(1, smsg, strlen(smsg)); // displaying server message

		if(strcmp("BYE\n", smsg) == 0)
			break;
	}

        close(sockfd); // closing socket after use

        return 0;
}