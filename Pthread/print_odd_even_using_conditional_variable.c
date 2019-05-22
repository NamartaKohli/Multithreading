#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
int count =1, end_number= 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void * print_even(void *arg)
{
	int final = *((int*)arg);
	while(count != final)
	{
		pthread_mutex_lock(&mutex);
		if((count%2) == 0)
		{
			printf(" printing count value from even function = %d\n", count);
			count++;
			pthread_cond_signal(&cond);
		}
		else 
			pthread_cond_wait(&cond, &mutex);
	
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}
void *print_odd(void *arg)
{
	int final = *((int*)arg);
	while(count != final)
	{
		pthread_mutex_lock(&mutex);
		if((count%2) == 0)
			pthread_cond_wait(&cond, &mutex);
		else 
		{
			printf(" printing count value from odd function = %d\n", count);
			count++;
			pthread_cond_signal(&cond);
		}	
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
	
}
int main(int argc, char* argv[])
{
	pthread_t tid1, tid2, tid3;
	int number = atoi(argv[1]);

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_create(&tid1, NULL, print_even, &number);
	pthread_create(&tid2, NULL, print_odd, &number);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	pthread_exit(NULL);
	return 0;
}
