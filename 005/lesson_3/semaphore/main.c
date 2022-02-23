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

    const size_t queue_size      = atoi(argv[1]);
    const size_t producers_count = atoi(argv[2]);
    const size_t consumers_count = atoi(argv[3]);

    sync_context_t sync_context;
    sync_context.queue_ = queue_create();

    srand(time(NULL));

    pthread_t consumers[consumers_count];
    // Create TODO

    pthread_t producers[producers_count];
    // Create TODO

    // JOIN TODO

    sem_destroy(&sync_context.full_);
    sem_destroy(&sync_context.empty_);
    sem_destroy(&sync_context.lock_);
    
    queue_destroy(sync_context.queue_);

    return 0;
}