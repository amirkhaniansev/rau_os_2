#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "atomic_int.h"

typedef struct input {
    atomic_int_t* obj_;
    size_t        thread_number_;
    int           init_value_;
} input_t;

void* start_thread(void* thread_input)
{
    input_t*      actual_input  = thread_input;
    atomic_int_t* atomic_obj    = actual_input->obj_;
    size_t        thread_number = actual_input->thread_number_;
    int           init_value    = actual_input->init_value_;

    int load_var;
    while (true) {
        if (!load(atomic_obj, &load_var))
            printf("%s", "Unable to load the value.\n");
        usleep(500000);

        if (!store(atomic_obj, init_value++))
            printf("%s", "Unable to store the value.\n");
        usleep(500000);
    }

    return NULL;
}

int main(int argc, char** argv) 
{   
    if (argc == 1) {
        printf("%s", "Please provide the amount of threads!\n");
        return 1;
    }

    const int thread_count = atoi(argv[1]);
    
    atomic_int_t* obj = create_atomic_int(0);
    if (obj == NULL) {
        printf("%s", "Unable to create atomic integer instance.\n");
        return 1;
    }

    pthread_t threads[thread_count];
    for (size_t i = 0; i < thread_count; i++) {
        input_t thread_input;
        thread_input.obj_           = obj;
        thread_input.init_value_    = 0;
        thread_input.thread_number_ = i;

        pthread_create(&threads[i], NULL, start_thread, &thread_input);
    }

    usleep(20000000);

    if (!destroy_atomic_int(obj)) {
        printf("%s", "Unable to destroy atomic integer object!\n");
        return 1;
    }

    return 0;
}