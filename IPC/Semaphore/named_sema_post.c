#include<stdio.h>
#include<semaphore.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	sem_t *sem;
	int sem_val =10;
	sem = sem_open("/test_sem", O_RDWR);
	if(sem == SEM_FAILED)
	{
		printf("failed to create semaphore\n");
		return EXIT_FAILURE;
	}
	
	if (sem_getvalue(sem, &sem_val) == -1)
	{
		printf("failed to get sem value\n");
		goto err;
	}
	
	printf("value of the semaphore is %d\n", sem_val);

	printf("going to post the semaphore\n");

	sem_post(sem);

	printf("After sem post \n");

	sem_close(sem);
	sem_unlink("/test_sem");
	return EXIT_SUCCESS;

err:
	sem_close(sem);
	sem_unlink("/test_sem");
	return EXIT_FAILURE;
}


