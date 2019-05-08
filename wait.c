#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <errno.h>

int main(int argc, char *agrv)
{
	int i, ret, p ; 
	pid_t child_pid;
	for(i =1; i<=argc; i++)
	{
		ret = fork();
		if(ret == -1)
			printf("failed in fork\n");
		else if(ret == 0)
		{
			/* Child sleeps for a while then exits */
			printf("we are in child process = %ld\n", getpid());
			sleep(2);
			printf("we are leaving child process = %ld\n", getpid());
			_exit(EXIT_SUCCESS);

		}
	}
	while(1)
	{

		child_pid = wait(&p); /* Parent waits for each child to exit */
		if(child_pid == -1)
		{
			if(errno == ECHILD)
			{
				printf("there is no more child to wait\n");
				exit(EXIT_SUCCESS);
			}
			else 
			{
				/* Some other (unexpected) error */
				printf("wait _error \n");
			}
		}
		printf("wait returned for pid %ld  with status = %d\n", child_pid, p);
	}
	return 0;
}

