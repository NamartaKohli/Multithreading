#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/* thread shares process ID and parent process ID
   thats in each thread pid is same */

/* thread ID is unique to each thread */

/* The execution of a thread terminates in one of the following ways: 
   1. The thread’s function performs a return specifying a return value for the
      thread. As we did in thread_func3()
   2. The thread calls pthread_exit(). As we did in thread_func1();
   3. The thread is canceled using pthread_cancel()
   4. Any of the threads calls exit(), or the main thread performs a return (in the
      main() function), which causes all threads in the process to terminate immediately.
*/
int retval = 0;

void* thread_func1(void *value)
{
	printf("hey I am in thread 1\n");
	pid_t pid = getpid();
	printf("hey I am in thread 1 with pid value %d\n", pid);
	sleep(1);
	printf("hey I am in thread 1\n");

/* The pthread_exit() function terminates the calling thread, and specifies a return
   value that can be obtained in another thread by calling pthread_join().
   The retval argument specifies the return value for the thread. The value pointed
   to by retval should not be located on the thread’s stack, since the contents of that
   stack become undefined on thread termination.
*/
	retval =50;
	pthread_exit(&retval);
}

void* thread_func2(void *value)
{
	int arg = *((int*)value);
	pid_t pid = getpid();
	printf("hey I am in thread 2 with pid value %d\n", pid);
	printf("hey I am in thread 2 with arg value %d\n", arg);
	sleep(2);
	pthread_exit(NULL);
}

void* thread_func3(void *value)
{
	printf("hey I am in thread 3\n");
	pid_t pid = getpid();
	char *arg = (char*)value;
	printf("hey I am in thread 3 with pid value %d arg = %s\n", pid, arg);
	sleep(1);
	printf("hey I am in thread 1\n");
	return (void*)strlen(arg);
}
int main()
{
	pthread_t tid1, tid2, tid3;
	int value = 5;
	void *thread3_ret;
	int *thread1_ret;
	pid_t pid = getpid();
	
	printf("main function pid = %d\n", pid);

	pthread_create(&tid1, NULL, thread_func1, NULL);
	
	/*passing argument to the thread */
	pthread_create(&tid2, NULL, thread_func2,  &value);

	pthread_create(&tid3, NULL, thread_func3, "hello World");
	
/* The pthread_join() function waits for the thread identified by thread to terminate.
   If that thread has already terminated, pthread_join() returns immediately.
*/
	pthread_join(tid1, (void**)&thread1_ret);
	printf("hey return value from thread 1 is %ld\n", (long)*thread1_ret);

	pthread_join(tid2, NULL);
	
	pthread_join(tid3, (void**)&thread3_ret);
	printf("hey return value from thread 3 is %ld\n", (long)thread3_ret);

/* Each thread within a process is uniquely identified by a thread ID. 
   thread can obtain its own ID using pthread_self().
   The pthread_join() call fails, returning the error EDEADLK (Resource deadlock would occur )
   because in below statement main thread while trying to join with itself or waiting for itself.
   
*/
	value = pthread_join(pthread_self(), NULL);
	if(value == EDEADLK)
		printf("************should not do this **************\n");
	return 0;
}
