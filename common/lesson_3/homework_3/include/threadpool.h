#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <stdlib.h>

typedef struct threadpool {
    // The fields you need. 
    // This is left upon your imagination.
} threadpool_t;

typedef void* input_t;
typedef void* output_t;
typedef output_t (*work_t)(input_t);

// Creates a new threadpool.
threadpool_t* create_threadpool(size_t size);

// Executes a new function pointer named work on the threadpool.
void execute(threadpool_t* threadpool, work_t work);

// Destroys the threadpool.
void destroy_threadpool(threadpool_t* threadpool);

#endif