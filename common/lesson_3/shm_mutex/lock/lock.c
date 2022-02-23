#include <stdio.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "lock.h"

sync_header_t* get_shared_mutex()
{
    const size_t size   = sizeof(sync_header_t);
    const key_t  key    = 0x2202005;
    const int    flag   = IPC_CREAT | 0660;
    int          shm_id = shmget(key, size, flag | IPC_EXCL);
    if (shm_id < 0) {
        perror("Unable to create a new instance of shared memory");
        printf("Currently accessing the existing one...\n");

        shm_id = shmget(key, size, flag);
        if (shm_id < 0) {
            perror("Unable to access the already created shared memory.\n");
            return NULL;
        }
    }

    sync_header_t* header = shmat(shm_id, NULL, 0);
    if (header == (void*)-1) {
        perror("Unable to attach to the shared memory");
        return NULL;
    }   
    
    if (!header->initialized_) {
        pthread_mutexattr_init(&header->lock_attr_);
        pthread_mutexattr_setpshared(&header->lock_attr_, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&header->lock_, &header->lock_attr_);
        header->initialized_ = true;
    }

    return header;
}

void enter_critical_section(sync_header_t* header)
{
    if (header == NULL) {
        printf("Invalid synchronization header is provided!\n");
        return;
    }

    if (pthread_mutex_lock(&header->lock_) != 0) {
        perror("Unable to enter the critical section");
        return;
    }

    printf("Entered the critical section!\n");
    printf("Sleeping 10 seconds...\n");

    sleep(10);

    if (pthread_mutex_unlock(&header->lock_) != 0) {
        perror("Unable to leave the critical section");
        return;
    }
}

void cleanup(sync_header_t* header, bool destroy)
{
    if (header == NULL)
        return;

    if (destroy) {
        printf("Destroy is requested.\n");
        if (pthread_mutexattr_destroy(&header->lock_attr_) != 0)
            perror("Unable to destroy the mutex attribute");

        if (pthread_mutex_destroy(&header->lock_) != 0)
            perror("Unable to destroy the mutex");

        const int shm_id = shmget(0x2202005, sizeof(sync_header_t), IPC_CREAT | 0660);
        if (shm_id < 0)
            perror("Unable to get shared memory ID for deletion");

        if (shmctl(shm_id, IPC_RMID, NULL) != 0)
            perror("Unable to delete shared memory");
    }

    if (shmdt(header) != 0)
        perror("Unable to detach from the shared memory");
}