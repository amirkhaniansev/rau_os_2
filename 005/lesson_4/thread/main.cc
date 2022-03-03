#include <queue>
#include <iostream>

#include <pthread.h>
#include <unistd.h>

typedef void (*work_t)(void*);

struct input
{
    int left;
    int right;
};

std::queue<std::pair<work_t, void*>> works;
pthread_cond_t canBeExecuted;
pthread_mutex_t lock;

void registerWork(work_t w, void* data)
{
    pthread_mutex_lock(&lock);
    works.push(std::make_pair(w,data));
    pthread_cond_signal(&canBeExecuted);
    pthread_mutex_unlock(&lock);
}

void* startThreadExecute(void* input)
{
    std::cout << (char*)input << std::endl;

    while(true)
    {
        pthread_mutex_lock(&lock);
        while(works.empty())
        {
          pthread_cond_wait(&canBeExecuted, &lock);
        }
        std::pair<work_t, void*> p = works.front();
        p.first(p.second);
        works.pop();
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void greetRAU(void* data)
{
    std::cout << "Hello RAU" << std::endl;
}

void greet005(void* data)
{
    std::cout << "Hello 005" << std::endl;
}

void sum(void* data)
{
    input* i = (input*) data;
    std::cout << i->left + i->right << std::endl;

}
int main(int argc, char** argv)
{
    char *hello = "Hi\0";
    input i;
    i.left = 4;
    i.right = 5;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&canBeExecuted, NULL);
    pthread_t thread;
    pthread_create(&thread, NULL, startThreadExecute, hello);
    registerWork(greetRAU, NULL);
    registerWork(greet005, NULL);
    registerWork(sum, &i);
    sleep(3);
    return 0;
}