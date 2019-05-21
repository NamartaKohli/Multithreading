#include "header.h"

int main()
{
	int fd, i =25;
	fd = shm_open(SHAREDMEM_FILENAME, O_CREAT|O_EXCL|O_RDWR, 0666);
	if(fd == -1)
	{
		printf("failed to open shared memory\n");
		return EXIT_FAILURE;
	}

	ftruncate(fd, sizeof(shared_data_t));

	shared_data_t* sdata = (shared_data_t*)mmap(0, sizeof(shared_data_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(sdata == MAP_FAILED)
	{
		printf("failed to mapped shared memory\n");
		return EXIT_FAILURE;
	}

	pthread_mutexattr_init(&sdata->mutex_attr);
	pthread_mutexattr_setpshared(&sdata->mutex_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&sdata->ipc_mutex, &sdata->mutex_attr);

	while(1)
	{
		pthread_mutex_lock(&sdata->ipc_mutex);
		printf("I am in locked section of first process \n");
		sdata->number = i++;
		sleep(10);
		printf("I am leaving locked section of first process = %d\n", sdata->number);
		pthread_mutex_unlock(&sdata->ipc_mutex);
		sleep(5);

	}
	pthread_mutex_destroy(&sdata->ipc_mutex);
	close(fd);
	munmap(sdata, sizeof(shared_data_t));
	shm_unlink(SHAREDMEM_FILENAME);
	return 0;
	
	
}
