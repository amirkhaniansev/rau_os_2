// C Program for Message Queue (Receiver Process)
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

	// msgrcv to receive message
    message_t msg;
	int bytes_count = msgrcv(msgid, &msg, sizeof(message_t), 1, 0);
    if (bytes_count == -1)
    {
        perror("Unable to receive a message.\n");
        return 1;
    }

	// display the message
	printf("Data Received is : %s \n", msg.msg_text);

	// to destroy the message queue
	msgctl(msgid, IPC_RMID, NULL);

	return 0;
}
