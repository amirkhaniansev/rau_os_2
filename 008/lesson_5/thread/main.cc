#include <queue>
#include <iostream>
#include <functional>
#include <pthread.h>
#include <unistd.h>

pthread_t thread;
pthread_mutex_t mutex_;
pthread_cond_t executed;
typedef std::function<void(void*)> work_t;
struct execution_context{
    work_t work;
    void* input;
};

execution_context make_context(work_t work, void* input)
{
    execution_context context;
    context.work = work;
    context.input = input;
    return context;
}

std::queue<execution_context> works;
void register_work(const execution_context &context){
    pthread_mutex_lock(&mutex_);
    works.push(context);
    pthread_cond_signal(&executed);
    pthread_mutex_unlock(&mutex_);
}

void* execute_thread(void* input)
{
    while(true)
    {
        pthread_mutex_lock(&mutex_);
        while(works.empty())
        {
            pthread_cond_wait(&executed, &mutex_);
        }
        execution_context context = works.front();
        works.pop();        
        context.work(context.input);
        pthread_mutex_unlock(&mutex_);
    }
    return nullptr;
}

void greet_rau(void* data)
{
    std::cout << "Hello RAU" << std::endl;
}

void greet_008(void* data)
{
    std::cout << "Hello 008" << std::endl;
}

struct input {
    int left;
    int right;    
};

void sum(void* s_input)
{
    input* sum_input = (input*)s_input;
    std::cout << sum_input->left + sum_input->right << std::endl;
}

int main(int argc, char** argv)
{
    pthread_mutex_init(&mutex_, NULL);
    pthread_cond_init(&executed, NULL);
    pthread_create(&thread, NULL, execute_thread, NULL);

    register_work(make_context(greet_rau, NULL));
    register_work(make_context(greet_008, NULL));
    register_work(make_context(greet_rau, NULL));
    register_work(make_context(greet_008, NULL));
    register_work(make_context(greet_rau, NULL));
    register_work(make_context(greet_008, NULL));
    register_work(make_context(greet_rau, NULL));
    register_work(make_context(greet_008, NULL));
    
    input sum_input;
    sum_input.left = 4;
    sum_input.right = 5;
    register_work(make_context(sum, (void*)(&sum_input)));
    register_work(make_context(sum, (void*)(&sum_input)));
    register_work(make_context(sum, (void*)(&sum_input)));
    register_work(make_context(sum, (void*)(&sum_input)));
    register_work(make_context(sum, (void*)(&sum_input)));

    char* message = "bye";
    register_work(make_context(pthread_exit, (void*)message));
 
    void* return_value;
    pthread_join(thread, &return_value);
    std::cout << "Exited with " << (char*)return_value << std::endl;
    
    pthread_cond_destroy(&executed);
    pthread_mutex_destroy(&mutex_);

    return 0;
}