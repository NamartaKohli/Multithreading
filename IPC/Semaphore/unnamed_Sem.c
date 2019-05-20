#include<semaphore.h>
#include<fcntl.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

static int global_data =8;
static sem_t sem;

static void *thread1_function(void *arg)
{
	int i, count = *((int*)arg); 
	for(i =0; i<count; i++)
	{
		if(sem_wait(&sem) == -1)
			return (void*)EXIT_FAILURE;
		global_data++;
		printf(" valueof global data = %d in function %s\n", global_data, __func__);
		if(sem_post(&sem) == -1)
			return (void*)EXIT_FAILURE;
		sleep(3);
	}
	return EXIT_SUCCESS;
}

static void *thread2_function(void *arg)
{
	int i, count = *((int*)arg); 
	for(i =0; i<count; i++)
	{
		if(sem_wait(&sem) == -1)
			return (void*)EXIT_FAILURE;
		global_data++;
		printf(" valueof global data = %d in function %s\n", global_data, __func__);
		if(sem_post(&sem) == -1)
			return (void*)EXIT_FAILURE;
		sleep(3);
	}
	return EXIT_SUCCESS;
}

int main()
{
	pthread_t tid1, tid2;
	void *ret;
	int loop;
	printf("Enetr the how many loop you want \n");
	scanf("%d", &loop);
	if(sem_init(&sem, 0, 1) == -1)
	{
		printf("failed to init semaphore\n");
		return EXIT_FAILURE;
	}  
	
	if(pthread_create(&tid1, NULL, thread1_function, &loop) == -1)
	{
		printf("failed to init semaphore\n");
		sem_destroy(&sem);
		return EXIT_FAILURE;
	}
	if(pthread_create(&tid1, NULL, thread2_function, &loop) == -1)
	{
		printf("failed to init semaphore\n");
		sem_destroy(&sem);
		return EXIT_FAILURE;
	}
	if(pthread_join(tid1, &ret) == -1)
	{
		sem_destroy(&sem);
		return EXIT_FAILURE;
	}
	
	if(pthread_join(tid2, &ret) == -1)
	{
		sem_destroy(&sem);
		return EXIT_FAILURE;
	}

}
