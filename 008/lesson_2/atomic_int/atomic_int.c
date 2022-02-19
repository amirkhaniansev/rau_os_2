#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <sys/types.h>

#include "atomic_int.h"

atomic_int_t* create_atomic_int(const int value)
{
    atomic_int_t* new_inst = malloc(sizeof(atomic_int_t));
    if (new_inst == NULL)
        return NULL;

    if (pthread_mutexattr_init(&new_inst->lock_attr_) != 0) {
        free(new_inst);
        return NULL;
    }

    if (pthread_mutex_init(&new_inst->lock_, &new_inst->lock_attr_) != 0) {
        pthread_mutexattr_destroy(&new_inst->lock_attr_);
        free(new_inst);
        return NULL;
    }

    new_inst->value_ = value;
    return new_inst;
}

bool store(atomic_int_t* atomic_int_v, const int value)
{
    if (atomic_int_v == NULL)
        return false;
    
    if (pthread_mutex_lock(&atomic_int_v->lock_) != 0)
        return false;

    printf("Thread %lu: Storing value %d in   the atomic variable.\n", pthread_self(), value);

    atomic_int_v->value_ = value;

    if (pthread_mutex_unlock(&atomic_int_v->lock_) != 0)
        return false;

    return true;     
}

bool load (atomic_int_t* atomic_int_v, int* dest) 
{
    if(atomic_int_v == NULL)
        return false;
    
    if(pthread_mutex_lock(&atomic_int_v->lock_) != 0)
        return false;

    printf("Thread %lu: Loaded  value %d from the atomic variable.\n", pthread_self(), *dest);
    *dest = atomic_int_v->value_;

    if(pthread_mutex_unlock(&atomic_int_v->lock_)!=0)
        return false;
    
    return true;

}

bool destroy_atomic_int(atomic_int_t* atomic_int_v) {
    if (atomic_int_v == NULL)
        return false;

    pthread_mutex_destroy(&atomic_int_v->lock_);
    pthread_mutexattr_destroy(&atomic_int_v->lock_attr_);

    return true;
}