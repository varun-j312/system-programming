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
        struct sockaddr_in servaddr, cliaddr; // server & client address
        char smsg[MAX], cmsg[MAX]; // buffer to send & receive messages
        int sockfd, connfd; // socket and connection variable
        int mlen, slen; // message length, socket address length
        socklen_t clen; // length of client socket address
        pid_t client_pid; // to store pid of child processes
        int ret; // return status of functions

        slen = sizeof(sockaddr_in); // size of socket address
        // clearing garbage value
        memset(&servaddr, 0, slen);
        memset(&cliaddr, 0, slen);
        memset(smsg, 0, MAX);
        memset(cmsg, 0, MAX);

        servaddr.sin_family = AF_INET; // using internet family of addressing - IPv4
        //servaddr.sin_addr.s_addr = inet_addr(ADDRSERV); // setting IP address
        servaddr.sin_addr.s_addr = INADDR_ANY; // setting IP address
        servaddr.sin_port = htons(PORTNO); // setting port number

        sockfd = socket(AF_INET, SOCK_STREAM, 0); // creating socket
        handleError(sockfd, "Error: Unable to create socket");
        ret = bind(sockfd, (struct sockaddr *)&servaddr, slen); // binding socket & network info
        handleError(ret, "Error: Unable to bind socket");
        ret = listen(sockfd, 5); // listening for clients
        handleError(ret, "Error: Unable to listen on socket");

        while(1)
        {
                connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clen); // waiting for connection
                handleError(connfd, "Error: Unable to accept connection");

                client_pid = fork();
                if(client_pid == 0)
                {
                        close(sockfd);
			while(1)
			{
        			memset(smsg, 0, MAX);
        			memset(cmsg, 0, MAX);

                        	mlen = read(connfd, cmsg, MAX); // reading client message
                        	write(1, "Client: ", 8);
                        	write(1, cmsg, strlen(cmsg)); // displaying client message

				if(strcmp("BYE\n", cmsg) == 0)
					break;

                        	write(1, "Enter message for client: ", 26);
                        	mlen = read(0, smsg, MAX); // reading server message through keyboard
                        	mlen = write(connfd, smsg, strlen(smsg)); // sending server message to client

                        	write(1, "---Message sent from server---\n", 32);

				if(strcmp("BYE\n", smsg) == 0)
					break;
			}

                        close(connfd);
                        exit(EXIT_SUCCESS);
                }

                close(connfd); // close connection
        }

        return 0;
}
