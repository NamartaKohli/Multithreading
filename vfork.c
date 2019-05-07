#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	int istack = 222;
	switch (vfork()) {
	case -1:
		printf("failed in vfork");

	case 0: /* Child executes first, in parent's memory space */
		sleep(3); /* Even if we sleep for a while, parent still is not scheduled */
		printf("Child executing\n");
		istack *= 3; /* This change will be seen by parent */
		_exit(EXIT_SUCCESS);

	default: /* Parent is blocked until child exits */
		printf("Parent executing\n");

	printf("istack=%d\n", istack);
	exit(EXIT_SUCCESS);
	}
}
