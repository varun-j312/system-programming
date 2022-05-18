// Program to define functions that create TCP socket and Shared memory
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
using namespace std;

// function to handle error status of functions
int handle_error(int status, const char errmsg[]);

// function to create a TCP socket
int create_tcp_socket(const char *ip_addr, int port_num);

// function to generate a key
key_t generate_key();

// function to create a shared memory
int create_shared_memory();

//function to attach shared memory id to address
int attach_shared_memory(int usage);

//function to destroy shared memory
int destroy_shared_memory();
