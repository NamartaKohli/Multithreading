#include "header.h"

int main()
{
	int fd, i=15;
	shared_data_t* sdata = NULL;

	fd = shm_open(SHAREDMEM_FILENAME, O_RDWR, 0666);
	if(fd == -1)
	{
		printf("failed to open shared memory \n");
		return EXIT_FAILURE;
	}

	sdata = (shared_data_t*)mmap(0, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(sdata == MAP_FAILED)
	{
		printf("failed to map shared memory \n");
		return EXIT_FAILURE;
	}
	
	while(1)
	{
		sleep(3);
		pthread_mutex_lock(&sdata->ipc_mutex);
		printf("I am in locked section of second process\n");

		printf("data is in second %d\n", sdata->number);
		sleep(20);

		printf("I am leaving locked section of second process\n");

		pthread_mutex_unlock(&sdata->ipc_mutex);
		sleep(10);
	}
	close(fd);
	munmap(sdata, sizeof(shared_data_t));
	shm_unlink(SHAREDMEM_FILENAME);
	return 0;
	
	
}
