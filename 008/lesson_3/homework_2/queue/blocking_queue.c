#include <stdlib.h>
#include <stdio.h>

#include "blocking_queue.h"

void* clean_up(blocking_queue_t* queue);

blocking_queue_t* create_queue(const size_t size)
{
    blocking_queue_t* inst = malloc(sizeof(blocking_queue_t));
    if (inst == NULL)
        return NULL;

    if (pthread_mutex_init(&inst->lock_, NULL) != 0)
        return clean_up(inst);
    if (pthread_cond_init(&inst->can_be_pushed, NULL) != 0)
        return clean_up(inst);
    if (pthread_cond_init(&inst->can_be_popped, NULL) != 0)
        return clean_up(inst);

    inst->size_ = size;
    inst->storage_ = queue_create();

    return inst;
}

bool push(blocking_queue_t* queue, const int item)
{
    if (queue == NULL)
        return false;

    if (pthread_mutex_lock(&queue->lock_) != 0)
        return false;

    if (queue->size_ != -1) {    
        while (queue->size_ == queue_size(queue->storage_)) {
            pthread_cond_wait(&queue->can_be_pushed, &queue->lock_);
        }
    }

    queue_push(queue->storage_, item);

    pthread_cond_signal(&queue->can_be_popped);
    if (pthread_mutex_unlock(&queue->lock_) != 0)
        return false;
    
    return true;
}

bool pop(blocking_queue_t* queue, int* dest)
{
    if (queue == NULL || dest == NULL)
        return false;

    if (pthread_mutex_lock(&queue->lock_) != 0)
        return false;
    while (queue_empty(queue->storage_))
        pthread_cond_wait(&queue->can_be_popped, &queue->lock_);

    *dest = queue_front(queue->storage_);

    queue_pop(queue->storage_);
    pthread_cond_signal(&queue->can_be_pushed);
    if (pthread_mutex_unlock(&queue->lock_) != 0)
        return false;

    return true;    
}

bool dump(blocking_queue_t* queue, const char* msg)
{
    if (queue == NULL)
        return false;

    if (pthread_mutex_lock(&queue->lock_) != 0)
        return false;
    printf("%s", msg);
    queue_dump(queue->storage_);
    if (pthread_mutex_unlock(&queue->lock_) != 0)
        return false;
    
    return true;
}

bool destroy_queue(blocking_queue_t* queue)
{
    if (queue == NULL)
        return false;

    if (pthread_cond_destroy(&queue->can_be_pushed) != 0)
        return false;

    if (pthread_cond_destroy(&queue->can_be_popped) != 0)
        return false;

    if (pthread_mutex_destroy(&queue->lock_) != 0)
        return false;

    queue_destroy(queue->storage_);
    free(queue);

    queue = NULL;
    
    return true;
}

void* clean_up(blocking_queue_t* queue)
{
    pthread_cond_destroy(&queue->can_be_pushed);
    pthread_cond_destroy(&queue->can_be_popped);
    pthread_mutex_destroy(&queue->lock_);
    free(queue);
    return NULL;
}