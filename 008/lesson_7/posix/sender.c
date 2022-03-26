#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char* argv)
{ 
    struct mq_attr attr;  
    attr.mq_flags   = 0;  
    attr.mq_maxmsg  = 10;  
    attr.mq_msgsize = 33;  
    attr.mq_curmsgs = 0;  
    
    mqd_t descriptor = mq_open("/queue_005_6", O_CREAT | O_WRONLY, 0666, &attr);
    if (descriptor == (mqd_t)-1)
    {
        perror("Unable to create a message queue.\n");
        return 1;
    }

	printf("Write Data : ");
	char msg[10];
    fgets(msg, 10, stdin);

    if (mq_send(descriptor, msg, 10, 0) == -1)
    {
        perror("Unable to send a message.\n");
        return 1;
    }

    if (mq_close(descriptor) == -1)
    {
        perror("Unable to close the message queue.\n");
        return 1;
    }

    return 0;
}