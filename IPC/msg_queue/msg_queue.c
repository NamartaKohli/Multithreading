#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<mqueue.h>
#include<unistd.h>

/* This program has covered following :
--> Queue creation with use defined attribute
--> Get the Message queue attribute 
--> send msg in Message queue
--> same process receive msg from queue
--> close and unlink the queue.

*/

int main()
{

	mqd_t mqd;
	char msg[20]; 
	char msg1[500]; 
	int prio, i, num_read;
	struct mq_attr attr;

	attr.mq_maxmsg = 50;
	attr.mq_msgsize = 500;

	mqd = mq_open("/test_mq", O_CREAT |O_EXCL | O_RDWR, S_IRUSR | S_IWUSR, &attr);
	if(mqd == (mqd_t)-1)
	{
		printf("test message queue creation failed\n");
		return EXIT_FAILURE;
	}

	
	if(mq_getattr(mqd, &attr) == -1)
	{
		printf("sending msg to queue failed\n");
		goto err;
	}

	printf("Maximum # of messages on queue: %ld\n", attr.mq_maxmsg);
	printf("Maximum message size: %ld\n", attr.mq_msgsize);
	printf("# of messages currently on queue: %ld\n", attr.mq_curmsgs);


	printf("Enetr priority of msg\n");
	scanf("%d", &prio);
	getchar();
	printf("Enetr your msg \n");
	fgets(msg, sizeof(msg), stdin);
	if (mq_send(mqd, msg, strlen(msg), prio) == -1)
	{
		printf("sending msg to queue failed\n");
		goto err;
	}
	if((num_read = mq_receive(mqd, msg1, attr.mq_msgsize,&prio)) == -1)
	{
		perror("Error: ");
		printf("Receiving  msg to queue failed num_read  = %d\n", num_read);
		goto err;
	}
		printf("priority is %d num_read = %d\n", prio, num_read);
		write(STDOUT_FILENO, msg, num_read); 
		write(STDOUT_FILENO, "\n", 1); 
			
	mq_close(mqd);
	mq_unlink("/test_mq");
	return EXIT_SUCCESS;
	
err:
	mq_close(mqd);
	mq_unlink("/test_mq");
	return EXIT_FAILURE;
	
}
