#ifndef __IPC_H__
#define __IPC_H__

#include <pthread.h>

typedef struct shm {
    pthread_mutexattr_t attr_;  
    pthread_mutex_t     mutex_;
    size_t              sum_;  
    size_t              arr_[1024];                 
} shm_t;

int create_shm();

void run_child(const int shm_id, const size_t child_number, const size_t size);

void run_parent(const int shm_id);

void wait_children(const pid_t* pids, const size_t count);

void destroy_shm(const int shm_id);

#endif