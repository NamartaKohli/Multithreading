#include<stdio.h>
#include<semaphore.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

/* This program will create a semaphore with name "test_sem"
   and acquire the semaphor and get blocked and the other program name
   named_sema_post.c post the semaphore and unblock this process */ 
	

int main()
{
	sem_t *sem;
	sem = sem_open("/test_sem", O_CREAT| O_EXCL | O_RDWR, 0666, 0);
	if(sem == SEM_FAILED)
	{
		printf("failed to create semaphore\n");
		return EXIT_FAILURE;
	}
	printf("going to acquire the semaphore\n");
	if (sem_wait(sem) == -1)
	{
		printf("failed to acquire semaphore\n");
		goto err;
	}
	printf("After sem wait \n");

	sem_close(sem);
	sem_unlink("/test_sem");
	return EXIT_SUCCESS;
err:
	sem_close(sem);
	sem_unlink("/test_sem");
	return EXIT_FAILURE;
	
}


