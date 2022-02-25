#include <stdio.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "lock.h"

sync_header_t* get_shared_mutex()
{
    // Calculating the size of header.
    const size_t size   = sizeof(sync_header_t);

    // Determing the size. For example it is year|your group number.
    const key_t  key    = 0x2202008;

    // IPC_CREAT means to try to create or get already
    // existing shared memory. 0660 is the user permission.
    const int flag   = IPC_CREAT | 0660;

    // Trying to get shaared memory.
    // Here we are to trying to exclusively create shared
    // memory with IPC_EXCL which can fail if it
    // has already been created previously.
    int shm_id = shmget(key, size, flag | IPC_EXCL);
    if (shm_id < 0) {
        perror("Unable to create a new instance of shared memory");
        printf("Currently accessing the existing one...\n");

        // Trying to get already created shared memory.
        shm_id = shmget(key, size, flag);
        if (shm_id < 0) {
            perror("Unable to access the already created shared memory.\n");
            return NULL;
        }
    }

    // Attaching to the shared memory.
    sync_header_t* header = shmat(shm_id, NULL, 0);
    if (header == (void*)-1) {
        perror("Unable to attach to the shared memory");
        return NULL;
    }   
    
    // If the sync header is not initialize we intialize it.
    if (!header->initialized_) {
        // Initialize fields
        pthread_mutexattr_init(&header->lock_attr_);
        pthread_mutexattr_setpshared(&header->lock_attr_,PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&header->lock_,&header->lock_attr_);
        header->initialized_=true;
    }

    // Returning header
    return header;
}

void enter_critical_section(sync_header_t* header)
{
    if (header == NULL) {
        printf("Invalid synchronization header is provided!\n");
        return;
    }

    if(pthread_mutex_lock(&header->lock_)!=0){
        return;
    }

    sleep(10);
    printf("%s\n","pthread locked");
    
    pthread_mutex_unlock(&header->lock_);
}

void cleanup(sync_header_t* header, bool destroy)
{
    if (header == NULL)
        return;

    if (destroy) {
        printf("Destroy is requested.\n");
        
        // Destroying mutex attribute.
        if (pthread_mutexattr_destroy(&header->lock_attr_) != 0)
            perror("Unable to destroy the mutex attribute");

        // Destroying mutex/
        if (pthread_mutex_destroy(&header->lock_) != 0)
            perror("Unable to destroy the mutex");

        // Getting shared memory.
        const int shm_id = shmget(0x2202005, sizeof(sync_header_t), IPC_CREAT | 0660);
        if (shm_id < 0)
            perror("Unable to get shared memory ID for deletion");

        // Removing shared memory with IPC_RMID command.
        if (shmctl(shm_id, IPC_RMID, NULL) != 0)
            perror("Unable to delete shared memory");
    }

    // Detaching shared memory.
    if (shmdt(header) != 0)
        perror("Unable to detach from the shared memory");
}