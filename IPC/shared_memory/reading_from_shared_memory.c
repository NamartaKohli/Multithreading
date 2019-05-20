#include<stdio.h>
#include<string.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/stat.h>

int main()
{
	int fd;
	struct stat sb;
	char *addr;
	fd = shm_open("/test_shared_memory", O_RDONLY, 0); /*open existing object*/
	if(fd == -1)
	{
		printf("failed to open shared memory\n");
		perror("");
		return EXIT_FAILURE;
	}
	/* Obtain the size of the file and use it to specify the size of
           the mapping and the size of the buffer to be written */
	if(fstat(fd, &sb) == -1)
	{
		printf("failed in fstat\n");
		close(fd);
		sem_unlink("/test_shared_memory");
		return EXIT_FAILURE;
	}
	addr = (char*)mmap(NULL,sb.st_size,PROT_READ, MAP_SHARED, fd, 0);
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
	write(STDOUT_FILENO, addr, sb.st_size);
	/* The shm_unlink() function removes the shared memory object specified by name.
	   Removing a shared memory object doesn’t affect existing mappings of the object
           (which will remain in effect until the corresponding processes call munmap() or terminate),
           but prevents further shm_open() calls from opening the object.*/

	munmap(addr, sb.st_size);  
	shm_unlink("/test_shared_memory");
	return EXIT_SUCCESS;
}
