#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char* argv)
{
    mqd_t descriptor = mq_open("/queue_005_6", O_RDONLY);
    if (descriptor == (mqd_t)-1)
    {
        perror("Unable to open the message queue.\n");
        return 1;
    }

    char msg[50];
    int  byte_count = mq_receive(descriptor, msg, 50, NULL);
    if (byte_count == -1)
    {
        perror("Unable to receive the message.\n");
        return 1;
    }

    printf("Message received: %s\n", msg);
    
    if (mq_close(descriptor) == -1)
    {
        perror("Unable to close the message queue.\n");
        return 1;
    }

    if (mq_unlink("/queue_005_6") == -1)
    {
        perror("Unable to unlink the message queue.\n");
        return 1;
    }

    return 0;
}