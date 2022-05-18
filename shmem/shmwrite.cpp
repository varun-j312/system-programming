// Program to write data into shared memory
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define MAX 256
using namespace std;

int main()
{
	key_t key;
	int shmid;
	char *msg;

	key = ftok("keyfile", 65);
	shmid = shmget(key, 1024, 0666 | IPC_CREAT | IPC_EXCL);
	if(shmid == -1)
	{
		perror("Error creating shared memory");
		exit(EXIT_FAILURE);
	}

	msg = (char *)shmat(shmid, (void *)0, 0);
	write(1, "Enter your data to store: ", 26);
	read(0, msg, MAX);

	cout << "Data written to shared memory: " << msg << endl;
	
	shmdt(msg);

	return 0;
}
