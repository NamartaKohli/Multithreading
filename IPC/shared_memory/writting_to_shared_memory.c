#include<stdio.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<stdlib.h>
#include<string.h>

char msg [50];

int main()
{
	int fd;
	void *addr;
	fd = shm_open("/test_shared_memory", O_CREAT | O_EXCL | O_RDWR, 0666);
	if(fd == -1)
	{
		printf("failed to open shared memory\n");
		return EXIT_FAILURE;
	} 
	printf("Enter the msg which you want to write on shared memory\n");
	fgets( msg, sizeof(msg), stdin);
	
	if (ftruncate(fd, strlen(msg)) == -1) /* Resize object to hold string */
	{
		printf("failed to turncate the fd\n");
		close(fd);
		sem_unlink("/test_shared_memory");
		return EXIT_FAILURE;
	}

	addr = mmap(NULL, strlen(msg), PROT_READ| PROT_WRITE, MAP_SHARED, fd, 0);
	if(addr == MAP_FAILED)
	{
		printf("failed in memory mapping \n");
		close(fd);
		sem_unlink("/test_shared_memory");
		return EXIT_FAILURE;
	}
	if(close(fd) == -1) /* 'fd' is no longer needed */
	{
	 	printf("failed to close the fd\n");
		return EXIT_FAILURE;
	}
	memcpy(addr, msg, sizeof(msg)); /* copy string to shared memory */
	
	/* The shm_unlink() function removes the shared memory object specified by name.
	   Removing a shared memory object doesn’t affect existing mappings of the object
           (which will remain in effect until the corresponding processes call munmap() or terminate),
           but prevents further shm_open() calls from opening the object.*/

	munmap(addr, strlen(msg));  

	sem_unlink("/test_shared_memory");

	return EXIT_SUCCESS;	
	
}


