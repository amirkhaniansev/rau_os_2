#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "pusher.h"
#include "blocking_queue.h"

void* start_pusher_routine(void* queue)
{
    blocking_queue_t* blocking_queue = queue;
    if (blocking_queue == NULL)
        return NULL;
    
    for (size_t i = 0; i < 1000; i++) {
        sleep(1);
        if (!push(queue, rand() % 100))
            printf("Unable to push!\n");
        if (!dump(queue, "Pushed: "))
            printf("Unable to dump!\n");       
    }

    return NULL;
}

void start_pusher(blocking_queue_t* queue, const bool join)
{
    srand(time(NULL));

    void*     ret;
    pthread_t thread;
    if (pthread_create(&thread, NULL, start_pusher_routine, queue) != 0)
        return;
    if (join)
        pthread_join(thread, &ret);
}