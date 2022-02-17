#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

// You can and should include other 
// necessary header files as well.
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct blocking_queue {
    // All the fields you need to implement.
    // This is all left on your imagination.
    // Hint: you will obviously need storage and synchronization primitives.
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
bool pop(blocking_queue_t* queue, const int* dest);

// Tries to destroy the queue.
// Returns true if there is no error, false otherwise.
bool destroy_queue(blocking_queue_t* queue);

#endif