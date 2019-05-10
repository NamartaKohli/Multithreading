#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 10

int main(int argc, char *argv[])
{
	int pfd[2]; /* Pipe file descriptors */
	char buf[BUF_SIZE];
	ssize_t numRead;
	if (argc != 2 || strcmp(argv[1], "--help") == 0)
		printf("%s string\n", argv[0]);
	if (pipe(pfd) == -1) /* Create the pipe */
		printf("pipe creation error");

	switch (fork()) {
		case -1:
			printf("fork creation error");
		case 0: /* Child - reads from pipe */
			if (close(pfd[1]) == -1) /* Write end is unused */
				printf("close - child process failed");
			for (;;) { /* Read data from pipe, echo on stdout */
				printf("close - child process failed\n");
				numRead = read(pfd[0], buf, BUF_SIZE);
				if (numRead == -1)
					printf("read");
				if (numRead == 0)
					break; /* End-of-file */
				if (write(1, buf, numRead) != numRead)
					printf("child - partial/failed write");
			}
			write(STDOUT_FILENO, "\n", 1);
			if (close(pfd[0]) == -1)
				printf("close child process failed");
			_exit(EXIT_SUCCESS);
		default: /* Parent - writes to pipe */
			if (close(pfd[0]) == -1) /* Read end is unused */
				printf("close - parent");
			if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1]))
				printf("parent - partial/failed write");
			if (close(pfd[1]) == -1) /* Child will see EOF */ //if comment this line Child process will always block on read.
				printf("close");
			wait(NULL); /* Wait for child to finish */
			exit(EXIT_SUCCESS);
	}
}
