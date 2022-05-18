// Program to read data from shared memory
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
	shmid = shmget(key, 1024, 0666);
	if(shmid == -1)
	{
		perror("Error creating shared memory");
		exit(EXIT_FAILURE);
	}

	msg = (char *)shmat(shmid, (void *)0, 0);

	cout << "Data read from shared memory: " << msg << endl;
	
	shmdt(msg);
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}
