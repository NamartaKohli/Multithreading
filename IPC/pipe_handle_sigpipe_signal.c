#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* When a process tries to write to a pipe for which no process has an open read
   descriptor, the kernel sends the SIGPIPE signal to the writing process.*/

#define BUF_SIZE 10

struct sigaction act;

void sighandler(int signum, siginfo_t *info, void *ptr)
{
    printf("Received signal %d\n", signum);
    printf("Signal originates from process %lu\n",
        (unsigned long)info->si_pid);
}


int main(int argc, char *argv[])
{
	int pfd[2]; /* Pipe file descriptors */
	char buf[BUF_SIZE];
	ssize_t numRead;
	if (argc != 2 || strcmp(argv[1], "--help") == 0)
		printf("%s string\n", argv[0]);
	if (pipe(pfd) == -1) /* Create the pipe */
		printf("pipe creation error");

	memset(&act, 0, sizeof(act));
	act.sa_sigaction = sighandler;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGPIPE, &act, NULL);
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

			if (close(pfd[0]) == -1)
				printf("close child process failed");

			_exit(EXIT_SUCCESS);

		default: /* Parent - writes to pipe */
			sleep(3);

			printf("pid of parent process id is %ld\n", getpid());

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
