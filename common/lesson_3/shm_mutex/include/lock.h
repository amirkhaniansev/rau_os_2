#ifndef __LOCK_H__
#define __LOCK_H__

#include <stdbool.h>
#include <pthread.h>

typedef struct header {
    pthread_mutex_t     lock_;
    pthread_mutexattr_t lock_attr_;
    bool                initialized_;
} sync_header_t;

sync_header_t* get_shared_mutex();

void enter_critical_section(sync_header_t* header);

void cleanup(sync_header_t* header, bool destroy);

#endif