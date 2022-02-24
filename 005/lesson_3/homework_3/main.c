// C STD headers
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// Linux sys headers for waiting children
#include <sys/types.h>
#include <sys/wait.h>

// POSIX pthread library header
#include <pthread.h>

// Your homework header
#include "ipc.h"

// Entry point of your program
int main(int argc, char** argv)
{
    // Your program expects the count of children as a cmd line argument.
    if (argc != 2) {
        printf("Please, provide the child count.\n");
        return 1;
    }

    // Creating shared memory segment
    const int shm_id = create_shm();
    if (shm_id < 0)
        return 1;

    // Count of your children.
    size_t child_count  = atoi(argv[1]);

    // Segment size for every child.
    size_t segment_size = 1024 / child_count;

    // Array for children process IDs.
    pid_t  pids[child_count];
    for (size_t i = 0; i < child_count; i++) {
        // Here we fork().
        pids[i] = fork();

        // If the pid is -1 => fork() faild.
        if (pids[i] == -1) {
            perror("Unable to fork:");
            break;
        }       

        // If the pid > 0 => we are in the parent process.
        // In this case we contine to create the remaining chidlren.
        if (pids[i] > 0) {
            continue;
        }

        // At this point we know that pid == 0 => we are in the child.
        // Here we run partial sum calculation.
        run_child(shm_id, i, segment_size);

        // Here we return to exit the child process.
        // Try to answer the question what will happen in case
        // we do not return here.
        return 0;
    }

    // Waiting for the children.
    wait_children(pids, child_count);

    // Running the parent.
    run_parent(shm_id);

    // Destroying shared memory segment.
    destroy_shm(shm_id);

    return 0;
}