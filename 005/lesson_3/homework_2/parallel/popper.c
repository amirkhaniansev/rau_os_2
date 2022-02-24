#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "popper.h"
#include "blocking_queue.h"

void* start_popper_routine(void* queue)
{
    blocking_queue_t* blocking_queue = queue;
    if (blocking_queue == NULL)
        return NULL;
    
    int result;
    for (size_t i = 0; i < 1000; i++) {
        sleep(1);
        if (!pop(queue, &result))
            printf("Unable to pop!\n");
        if (!dump(queue, "Popped: "))
            printf("Unable to dump!\n");       
    }

    return NULL;
}

void start_popper(blocking_queue_t* queue, const bool join)
{
    srand(time(NULL));
    
    void*     ret;
    pthread_t thread;
    if (pthread_create(&thread, NULL, start_popper_routine, queue) != 0)
        return;
    if (join)
        pthread_join(thread, &ret);
}