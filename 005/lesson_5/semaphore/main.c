#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

#include "queue.h"
#include "error_messages.h"

typedef struct sync_context {
    sem_t    full_;
    sem_t    empty_;
    sem_t    lock_;
    queue_t* queue_;   
} sync_context_t;

void* execute_producer(void* input)
{
    sync_context_t* context = input;

    for (size_t i = 0; i < 25UL; i++) {
        sleep(1);

        const int new_item = rand() % 999;
        
        // Enter critical section

        printf("New item %3d is produced and inserted into the storage: ", new_item);

        queue_push(context->queue_, new_item);
        queue_dump(context->queue_);

        // Leave critical section
    }

    return NULL;
}

void* execute_consumer(void* input)
{
    sync_context_t* context = input;

    for (size_t i = 0; i < 25UL; i++) {
        sleep(1);

        // Enter critical section
        
        const int item = queue_front(context->queue_);
        printf("New item %3d is consumed and removed from the storage:  ", item);

        queue_pop(context->queue_);
        queue_dump(context->queue_);

        // Leave critical section
    }

    return NULL;
}

int main(int argc, char** argv)
{
    if (argc != 4) {
        printf(ERROR_FORMAT, ARGS_ERROR);
        return 1;
    }   

    // Parse command line arguments
    // TODO
    size_t q_size = atoi(argv[1]);
    size_t prod_size = atoi(argv[2]);
    size_t cons_size = atoi(argv[3]);

    // Initialize synchronization context
    sync_context_t sync_context;
    sem_init(&sync_context.lock_, 0, 1);
    sem_init(&sync_context.full_, 0, q_size);
    sem_init(&sync_context.empty_, 0, 0);
    sync_context.queue_ =  queue_create();

    // Initialize random machine
    // TODO
    pthread_t consumers[cons_size];
    for (int i = 0; i < cons_size; ++i)
    {
        pthread_create(&consumers[i], NULL, execute_consumer, NULL);
    }
    // Create consumer threads
    pthread_t prod[prod_size];
    for (int i = 0; i < prod_size; ++i)
    {
        pthread_create(&prod[i], NULL, execute_producer, NULL);
    }
    // Create producer threads
     
    // Join producer threads
    for (int i = 0; i < cons_size; ++i)
    {
        pthread_join(&consumers[i], NULL);
    }
    // Join consumer threads
    for (int i = 0; i < prod_size; ++i)
    {
        pthread_join(&prod[i], NULL);
    }
    // Destroy synchronization context
    

    return 0;
}