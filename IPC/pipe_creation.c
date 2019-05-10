#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 10
#define STDOUT	1

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


			/* The process reading from the pipe closes its write descriptor for the pipe, so
			that, when the other process completes its output and closes its write descriptor,
			the reader sees end-of-file (once it has read any outstanding data in the pipe).
			If the reading process doesn’t close the write end of the pipe, then, after the
			other process closes its write descriptor, the reader won’t see end-of-file, even after
			it has read all data from the pipe. Instead, a read() would block waiting for data,
			because the kernel knows that there is still at least one write descriptor open for the
			pipe. That this descriptor is held open by the reading process itself is irrelevant */

			if (close(pfd[1]) == -1) /* Write end is unused */ //if comment this line Child process will always block on read.
				printf("close - child process failed");

			for (;;) { /* Read data from pipe, echo on stdout */
				numRead = read(pfd[0], buf, BUF_SIZE);
				if (numRead == -1)
					printf("read");
				if (numRead == 0)
					break; /* End-of-file */
				if (write(STDOUT, buf, numRead) != numRead)
					printf("child - partial/failed write");
			}

			write(STDOUT, "\n", 1);
			if (close(pfd[0]) == -1)
				printf("close child process failed");

			_exit(EXIT_SUCCESS);

		default: /* Parent - writes to pipe */

			/* The writing process closes its read descriptor for the pipe for a different reason.
			When a process tries to write to a pipe for which no process has an open read
			descriptor, the kernel sends the SIGPIPE signal to the writing process. By default,
			this signal kills a process. A process can instead arrange to catch or ignore this signal 
			If the writing process doesn’t close the read end of the pipe, then, even after the
			other process closes the read end of the pipe, the writing process will still be able to
			write to the pipe.*/

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
