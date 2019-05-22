#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int global_data =10;

void * thread1_func(void *arg)
{
	int i;
	pthread_mutex_lock(&mtx);
	for(i =0; i< 10; i++)
	{
		global_data = i+1;
		printf("global data in thread 1 is = %d\n", global_data);
	}
	pthread_mutex_unlock(&mtx);
	pthread_exit(EXIT_SUCCESS);
}

void *thread2_func(void *arg)
{
	int i;
	pthread_mutex_lock(&mtx);
	for(i =0; i< 10; i++)
	{
		global_data = global_data +i;
		printf("global data in thread 2 is = %d\n", global_data);
	}
	pthread_mutex_unlock(&mtx);
	pthread_exit(EXIT_SUCCESS);
}

int main()
{
	pthread_t pid1, pid2;
	int err;
	
	err = pthread_create(&pid1, NULL, thread1_func, NULL);
	if(err != 0)
	{
		printf("Pthread creation error\n");
		return EXIT_FAILURE;
	}
	err = pthread_create(&pid2, NULL, thread2_func, NULL);
	if(err != 0)
	{
		printf("Pthread creation error\n");
		return EXIT_FAILURE;
	}
	err = pthread_join(pid1, NULL);
	if(err != 0)
	{
		printf("Pthread creation error\n");
		return EXIT_FAILURE;
	}
	err = pthread_join(pid2, NULL);
	if(err != 0)
	{
		printf("Pthread creation error\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

