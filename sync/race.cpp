// Program to demonstrate a race condition
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
using namespace std;

int balance = 0;

void* computebalance(void* arg)
{
	int b, c;
	b = balance;
	for(int i = 0; i < 50000; i++)
	{
		c = 5000 * 1234;
	}
	b += 10;
	balance = b;

	return NULL;
}

int main()
{
	int  i;
	pthread_t thread_id[200];

	cout << "Balance before thread: " << balance << endl;

	for(i = 0; i < 200; i++)
	{
		pthread_create(&thread_id[i], NULL, computebalance, NULL);
	}

	for(i = 0; i < 200; i++)
	{
		pthread_join(thread_id[i], NULL);
	}

	cout << "Balance after thread: " << balance << endl;

	exit(EXIT_SUCCESS);
}
