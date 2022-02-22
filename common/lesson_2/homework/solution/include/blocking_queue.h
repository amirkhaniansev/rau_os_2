#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

// You can and should include other 
// necessary header files as well.
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "queue.h"

typedef struct blocking_queue {
    queue_t*        storage_;
    size_t          size_;
    pthread_mutex_t lock_;
    pthread_cond_t  can_be_pushed;
    pthread_cond_t  can_be_popped;
} blocking_queue_t;

// Creates queue. If there are some errors
// this function should return NULL.
blocking_queue_t* create_queue(const size_t size);

// Tries to push the item to the queue.
// If the queue is full this call should block.
// However if the queue does not have size limit  
// this call may never theoretically block.
// Returns true if there is no error, false otherwise.
bool push(blocking_queue_t* queue, const int  item);

// Tries to pop an item and save it into dest.
// This function should always block if the queue is empty.
// Returns true if there is no error, false otherwise.
bool pop(blocking_queue_t* queue, int* dest);

// Dumps the queue content in the STD output stream.
bool dump(blocking_queue_t* queue, const char* msg);

// Tries to destroy the queue.
// Returns true if there is no error, false otherwise.
bool destroy_queue(blocking_queue_t* queue);

#endif