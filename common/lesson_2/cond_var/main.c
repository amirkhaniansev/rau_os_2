#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <unistd.h>

#include <pthread.h>

typedef struct input {
    pthread_cond_t   cond_;
    pthread_mutex_t  lock_;
    size_t           count_;
} input_t;

void* increment(void* thread_input)
{
    input_t* actual_input = thread_input;

    pthread_cond_t  cond  = actual_input->cond_;
    pthread_mutex_t lock  = actual_input->lock_;

    size_t* count_ptr = &actual_input->count_;

    while (true) {
        usleep(500000);
        pthread_mutex_lock(&lock);
        if (*count_ptr == 0)
            pthread_cond_signal(&cond);
        size_t before = *count_ptr;
        size_t after  = ++(*count_ptr);
        printf("Thread: %lu | Increment: %zu: %zu\n", pthread_self(), before, after);
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

void* decrement(void* thread_input)
{
    input_t* actual_input = thread_input;

    pthread_cond_t  cond  = actual_input->cond_;
    pthread_mutex_t lock  = actual_input->lock_;

    size_t* count_ptr = &actual_input->count_;

    while (true) {
        usleep(1000000);
        pthread_mutex_lock(&lock);
        while (*count_ptr == 0)
            pthread_cond_wait(&cond, &lock);              
        size_t before = *count_ptr;
        size_t after  = --(*count_ptr);      
        printf("Thread: %lu | Decrement: %zu: %zu\n", pthread_self(), before, after);
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main(int argc, char** argv) 
{
    input_t input;
    input.count_ = 0;
    pthread_cond_init(&input.cond_, NULL);
    pthread_mutex_init(&input.lock_, NULL);

    pthread_t threads[4];
    for (size_t i = 0; i < 4; i += 2) {
        pthread_create(&threads[i],   NULL, increment, &input);
        pthread_create(&threads[i+1], NULL, decrement, &input);
    }   

    usleep(10000000);

    return 0;
}