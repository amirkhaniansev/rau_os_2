#ifndef __ATOMIC_INT_H__
#define __ATOMIC_INT_H__

#include <stdbool.h>
#include <pthread.h>

typedef struct atomic_int {
    pthread_mutex_t     lock_;
    pthread_mutexattr_t lock_attr_;
    int                 value_;
} atomic_int_t;

atomic_int_t* create_atomic_int(const int value);

bool store(atomic_int_t* atomic_int_v, const int value);

bool load (atomic_int_t* atomic_int_v, int* dest);

bool destroy_atomic_int(atomic_int_t* atomic_int_v);

#endif