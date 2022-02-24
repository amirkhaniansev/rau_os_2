#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ipc.h"

int create_shm()
{
    // TODO
}

void run_child(const int shm_id, const size_t child_number, const size_t size)
{
    // TODO
}

void run_parent(const int shm_id)
{
    // TODO
}

void wait_children(const pid_t* pids, const size_t count)
{
    for (size_t i = 0; i < count; i++)
        waitpid(pids[i], NULL, 0);
}

void destroy_shm(const int shm_id)
{
    // TODO
}