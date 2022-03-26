// C Program for Message Queue (Sender Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// structure for message queue
typedef struct msg_buffer {
	long msg_type;
	char msg_text[100];
} message_t;

int main()
{
	// ftok to generate unique key
	key_t key = ftok("progfile", 65);

	// msgget creates a message queue
	// and returns identifier
	int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        perror("Unable to get message queue\n");
        return 1;
    }
	
    message_t msg;
    msg.msg_type = 1;

	printf("Write Data : ");
	fgets(msg.msg_text, 10, stdin);

	// msgsnd to send message
	if (msgsnd(msgid, &msg, sizeof(msg), 0) == -1)
    {
        perror("Unable to send a message\n");
        return 1;
    }
    
	// display the message
	printf("Data send is : %s \n", msg.msg_text);

	return 0;
}