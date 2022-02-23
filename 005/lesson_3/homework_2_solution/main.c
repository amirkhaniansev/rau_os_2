#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "blocking_queue.h"
#include "pusher.h"
#include "popper.h"

int main(int argc, char** argv)
{
    if (argc < 3 || argc > 4) {
        printf("Please, provide the pusher and popper counts!\n");
        return 1;
    }

    size_t pusher_count = atoi(argv[1]);
    size_t popper_count = atoi(argv[2]);

    size_t queue_max_size = -1;
    if (argc == 4) 
        queue_max_size = atoi(argv[3]);
    
    printf("You requested %zu pusher and %zu popper threads ", pusher_count, popper_count);
    printf("with queue max size %zu.\n", queue_max_size);

    blocking_queue_t* queue = create_queue(queue_max_size);
    if (queue == NULL) {
        printf("Unable to create a blocking queue!\n");
        return 1;
    }

    printf("Blocking queue is successfully created!\n");

    printf("Creating pushers...\n");
    for (size_t i = 0; i < pusher_count; i++)
        start_pusher(queue, false);
    
    printf("Creating poppers...\n");
    for (size_t i = 0; i < popper_count; i++)
        start_popper(queue, false);
    
    sleep(1000);
    
    return 0;
}