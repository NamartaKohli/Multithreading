#include"header.h"

int main()
{
	int fd, i=15;
	int sync_fd, data_fd ;

	sync_fd = shm_open(SHARED_SYNC_FILENAME, O_RDWR, 0666);
	if(sync_fd == -1)
	{
		printf("failed to open shared memory for sync object\n");
		return EXIT_FAILURE;
	}

	data_fd = shm_open(SHARED_DATA_FILENAME, O_RDWR, 0666);
	if(data_fd == -1)
	{
		printf("failed to open shared memory for shared data \n");
		return EXIT_FAILURE;
	}
	ftruncate(sync_fd, sizeof(shared_sync_t));

	ftruncate(data_fd, sizeof(shared_sync_t));

	shared_sync_t* sync_object = (shared_sync_t*)mmap(0, sizeof(shared_sync_t), PROT_READ|PROT_WRITE, MAP_SHARED, sync_fd, 0);
	if(sync_object == MAP_FAILED)
	{
		printf("failed to mapped memory for sync object\n");
		goto sync_err;
	
	}
	shared_data_t * shared_data = (shared_data_t*)mmap(0, sizeof(shared_data_t), PROT_READ|PROT_WRITE, MAP_SHARED,data_fd, 0);
	if(shared_data == MAP_FAILED)
	{
		printf("failed to mapped memory for shared data\n");
		goto data_err;
	}

	while(1)
	{
		pthread_mutex_lock(&sync_object->ipc_mutex);
		if(shared_data->status == 1)
		{
			write(STDOUT_FILENO, shared_data->msg, sizeof(shared_data->msg));
			sleep(5);
			shared_data->status = 0;
			pthread_cond_signal(&sync_object->ipc_condvar);
		}
		else 
		{
			printf("waiting on conditional variable in second process status = %d\n", shared_data->status);
			pthread_cond_wait(&sync_object->ipc_condvar, &sync_object->ipc_mutex);
			
		}
		pthread_mutex_unlock(&sync_object->ipc_mutex);
	}

	close(sync_fd);
	shm_unlink(SHARED_SYNC_FILENAME);
	close(data_fd);
	shm_unlink(SHARED_DATA_FILENAME);

	return EXIT_SUCCESS;

data_err: close(data_fd);
	  shm_unlink(SHARED_DATA_FILENAME);
sync_err:
	  close(sync_fd);
	  shm_unlink(SHARED_SYNC_FILENAME);
	  return EXIT_FAILURE;
	
	
}
