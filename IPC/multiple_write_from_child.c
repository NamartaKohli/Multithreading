#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#define BUF_SIZE 10

int main(int argc, char *argv[])
{
	int pfd[2]; /* Process synchronization pipe */
	int j, count;
	char buf[256];
	ssize_t numRead;
	

	if (argc < 2 || strcmp(argv[1], "--help") == 0)
	printf("%s sleep-time...\n", argv[0]);
	
	count = atoi(argv[2]);

	setbuf(stdout, NULL);

	if (pipe(pfd) == -1)
		printf("pipe creation error");

	for (j = 1; j < count; j++) {

		 switch (fork()) {
			case -1:
				printf("fork creation failed  %d", j);
			case 0: /* Child */
				if (close(pfd[0]) == -1) /* Read end is unused */
					printf("close failed fd[0]");

				if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1]))
					printf("child - partial/failed write");

				//printf("(PID=%ld) closing pipe\n", (long) getpid());

				if (close(pfd[1]) == -1)
					printf("close failed fd[1]");
				_exit(EXIT_SUCCESS);

			default: /* Parent loops to create next child */
				break;
		}
	}

	printf("\nparent comes here \n");

	/* Parent comes here; close write end of pipe so we can see EOF */
	if (close(pfd[1]) == -1) /* Write end is unused */
		printf("error close \n");

	while(1) {
		numRead = read(pfd[0], buf, strlen(argv[1]));
                if (numRead == -1)
                        printf("read");
                 if (numRead == 0) {
                       printf(" End-of-file \n");
			break;
		}
		write(1, buf, numRead);
		write(1, "\n", 1);
	}
	if (close(pfd[0]) == -1)
		printf("Close error \n");

	printf(" Parent ready to go\n");
	/* Parent can now carry on to do other things... */
	exit(EXIT_SUCCESS);
}
