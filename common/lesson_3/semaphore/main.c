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
        
        sem_wait(&context->empty_);
        sem_wait(&context->lock_);

        printf("New item %3d is produced and inserted into the storage: ", new_item);

        queue_push(context->queue_, new_item);
        queue_dump(context->queue_);

        sem_post(&context->lock_);
        sem_post(&context->full_);
    }

    return NULL;
}

void* execute_consumer(void* input)
{
    sync_context_t* context = input;

    for (size_t i = 0; i < 25UL; i++) {
        sleep(1);

        sem_wait(&context->full_);
        sem_wait(&context->lock_);
        
        const int item = queue_front(context->queue_);
        printf("New item %3d is consumed and removed from the storage:  ", item);

        queue_pop(context->queue_);
        queue_dump(context->queue_);

        sem_post(&context->lock_);
        sem_post(&context->empty_);
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
    sem_init(&sync_context.full_,  0, 0);
    sem_init(&sync_context.empty_, 0, queue_size);
    sem_init(&sync_context.lock_,  0, 1);

    sync_context.queue_ = queue_create();

    srand(time(NULL));

    pthread_t consumers[consumers_count];
    for (size_t i = 0; i < consumers_count; i++) {
        pthread_create(&consumers[i], NULL, execute_consumer, &sync_context);
    }

    pthread_t producers[producers_count];
    for (size_t i = 0; i < producers_count; i++) {
        pthread_create(&producers[i], NULL, execute_producer, &sync_context);
    }

    for (size_t i = 0; i < producers_count; i++) {
        pthread_join(producers[i], NULL);
    }   

    for (size_t i = 0; i < consumers_count; i++) {
        pthread_join(consumers[i], NULL);
    }
    
    sem_destroy(&sync_context.full_);
    sem_destroy(&sync_context.empty_);
    sem_destroy(&sync_context.lock_);
    
    queue_destroy(sync_context.queue_);

    return 0;
}