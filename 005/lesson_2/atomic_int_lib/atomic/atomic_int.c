#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <sys/types.h>

#include "atomic_int.h"

atomic_int_t* create_atomic_int(const int value)
{
    atomic_int_t* obj = malloc(sizeof(atomic_int_t));
    if (obj == NULL) 
        return NULL;

    if (pthread_mutexattr_init(&obj->lock_attr_) != 0) {
        free(obj);
        return NULL;
    }

    if (pthread_mutex_init(&obj->lock_, &obj->lock_attr_) != 0) {
        pthread_mutexattr_destroy(&obj->lock_attr_);
        free(obj);
        return NULL;
    }

    obj->value_ = value;
    return obj;
}

bool store(atomic_int_t* atomic_int_v, const int value)
{
    if (pthread_mutex_lock(&atomic_int_v->lock_) != 0)
        return false;
    
    atomic_int_v->value_ = value;
    printf("Thread %lu: Storing value %d in   the atomic variable.\n", pthread_self(), value);

    if (pthread_mutex_unlock(&atomic_int_v->lock_) != 0)
        return false;

    return true;
}

bool load (atomic_int_t* atomic_int_v, int* dest) 
{
    if (pthread_mutex_lock(&atomic_int_v->lock_)!=0)
       return false;
    *dest = atomic_int_v->value_;
    printf("Thread %lu: Loaded  value %d from the atomic variable.\n", pthread_self(), *dest);
    if (pthread_mutex_unlock(&atomic_int_v->lock_) !=0)
        return false;
    return true;
}

bool destroy_atomic_int(atomic_int_t* atomic_int_v) {
    if(atomic_int_v == NULL)
      return false;
    if(pthread_mutex_destroy(&atomic_int_v->lock_) != 0)
      return false;
    if (pthread_mutexattr_destroy(&atomic_int_v->lock_attr_) != 0)
      return false;
    free(atomic_int_v);
    return true;
}