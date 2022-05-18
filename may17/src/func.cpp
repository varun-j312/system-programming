// Program to define functions that create TCP socket and Shared memory
#include "../inc/func.h"

// function to handle error status of functions
int handle_error(int status, const char errmsg[])
{
	if(status < 0) // checking for error status
	{
		perror(errmsg); // print error message
		exit(EXIT_FAILURE);
	}
	//exit(EXIT_SUCCESS);
	return 0;
}

// function to create a TCP socket
int create_tcp_socket(const char *ip_addr, int port_num)
{
	struct sockaddr_in socketAddr; // server address structure
	int sockfd; // TCP socket
	int slen; // length of server address
	int ret; // return status of functions

	slen = sizeof(socketAddr); // calculating size of socket address
	memset(&socketAddr, 0, slen); // clearing garbage value
	
	socketAddr.sin_family = AF_INET; // using IPv4 internet family of address
	socketAddr.sin_port = htons(port_num); // setting port number
	socketAddr.sin_addr.s_addr = inet_addr(ip_addr); // setting IP address

	sockfd = socket(AF_INET, SOCK_STREAM, 0); // creating a socket
	handle_error(sockfd, "Error: Unable to create TCP socket");

	if(inet_addr(ip_addr) == 0)
	{
		ret = bind(sockfd, (struct sockaddr *) &socketAddr, slen); // binding socket to IP address and port number
		handle_error(ret, "Error: Unable to bind TCP socket");

		ret = listen(sockfd, 5); // listening on socket
		handle_error(ret, "Error: Unable to listen on TCP socket");
	}

	return sockfd; // returning TCP socket
}

// function to generate key
key_t generate_key()
{
	key_t key;
	key = ftok("/home/cguser9/system-prog/may17/data/keyfile", 65);
	handle_error(key, "Error generating key");
	return key;
}

// function to create a shared memory
int create_shared_memory()
{
	key_t key = generate_key(); // key to access shared memory
	int shmid; // id of the shared memory
	short *num_of_connections;
	int ret; // stores return status of functions

	shmid = shmget(key, 2, 0666 | IPC_CREAT); // generating the id for shared memory
	handle_error(shmid, "Error creating shared memory");

	num_of_connections = (short *) shmat(shmid, (void *) 0, 0); // storing no. of connections
	*num_of_connections = 0; // initialsing to zero

	ret = shmdt((void *) num_of_connections);
	handle_error(ret, "Error detaching from shared memory");

	return 0;
}

//function to attach shared memory id to address
int attach_shared_memory(int usage)
{
	key_t key = generate_key(); // key to access shared memory
	int shmid; // id of the shared memory
	short *t_connections; // shared memory variable
	short num_of_connections; // stores no. of connections
	int ret; // stores return status of functions

	shmid = shmget(key, 2, 0666); // accessing id of shared memory
	handle_error(shmid, "Error creating shared memory");

	t_connections = (short *) shmat(shmid, (void *) 0, 0); // attaching shared memory id to address
	if(usage == 1)
	{
		*t_connections += 1;
	}
	if(usage == 2)
	{
		*t_connections -= 1;
	}
	num_of_connections = *t_connections;

	ret = shmdt((void *) t_connections);
	handle_error(ret, "Error detaching from shared memory");

	return num_of_connections;
}

//function to destroy shared memory
int destroy_shared_memory()
{
	key_t key = generate_key(); // key to access shared memory
	int shmid; // id of the shared memory
	int ret; // return status of functions
	short *num_of_connections;

	shmid = shmget(key, 2, 0666); // accessing id of shared memory

	ret = shmctl(shmid, IPC_RMID, NULL); // removing shared memory
	handle_error(ret, "Unable to remove shared memory");

	return 0;
}
