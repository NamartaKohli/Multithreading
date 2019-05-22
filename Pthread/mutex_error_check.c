#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

/*PTHREAD_MUTEX_ERRORCHECK
  Error checking is performed on all operations. All three of the above scenarios
  cause the relevant Pthreads function to return an error. This type of mutex
  is typically slower than a normal mutex, but can be useful as a debugging
  tool to discover where an application is violating the rules about how a
  mutex should be used. */

pid_t pid;
pthread_mutex_t mutex;
pthread_mutexattr_t attr;
void* thread_func1(void *value)
{
	printf("hey I am in thread 1\n");
	pid = getpid();
	printf("hey I am in thread 1 with pid value %d\n", pid);
	sleep(1);
	printf("hey I am in thread 1\n");
}


int main()
{
	pthread_t tid1, tid2;
	int value = 5, err;
	void *statusp;
	pid = getpid();
	
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
	pthread_mutex_init(&mutex, &attr);
	err = pthread_mutex_lock(&mutex);
	if(err)
	{
		printf("error in mutex lock \n");
		return EXIT_FAILURE;
	}
	/* once we have locked the mutex we should not lock again */
	err = pthread_mutex_lock(&mutex);
	if(err)
	{
		if(err == EDEADLK)
		{
			printf("Resource deadlock would occur\n");
			return EXIT_FAILURE;

		}
	}
	printf(" main function pid = %d\n", pid);
	pthread_create(&tid1, NULL, thread_func1, NULL);
	pthread_join(tid1, NULL);
	return 0;
}
