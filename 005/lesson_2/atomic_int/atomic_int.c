#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <sys/types.h>

#include "atomic_int.h"

atomic_int_t* create_atomic_int(const int value)
{
    // TODO
}

bool store(atomic_int_t* atomic_int_v, const int value)
{
    // TODO
    printf("Thread %lu: Storing value %d in   the atomic variable.\n", pthread_self(), value);
}

bool load (atomic_int_t* atomic_int_v, int* dest) 
{
    // TODO
    printf("Thread %lu: Loaded  value %d from the atomic variable.\n", pthread_self(), *dest);

}

bool destroy_atomic_int(atomic_int_t* atomic_int_v) {
    // TODO
}