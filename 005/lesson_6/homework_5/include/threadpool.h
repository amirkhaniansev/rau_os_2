#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <functional>
#include <vector>
#include <queue>

#include <pthread.h>

namespace RauOs {
    // Typedef for Input Type
    typedef void* Input;

    // Typedef for Output Type
    typedef void* Output;

    // Typedef for function pointer.
    // Since this homework can be written in C++
    // we use C++ constructs and std::function<T> is an example of it.
    // std::function<T> is basically a function pointer equivalent of C.
    // In order to define a function with signature TOut f(TIn1, TIn2, ..., TInN)
    // we can write std::function<TOut(TIn1, TIn2, ..., TinN)>.
    // For our specific instance we want a function with the following signature
    // void* f(void* input), that's why we write std::function<Output(Input)> which
    // is the same as std::function<void*(void*)>
    typedef std::function<Output(Input)> Work;

    // Typedef pthread_t as Worker. This is just a convenience typedef.
    typedef pthread_t Worker;

    // Our main class which represents threadpool implementation.
    class ThreadPool {
        public:
            // Parameterized constructor which creates a new instance 
            // of ThreadPool with the degree of parallelism. This parameter
            // indicates how many threads you threadpool should have.
            ThreadPool(const std::size_t parallelismDegree);

            // Here we tell the compiler NOT to generate copy constructor
            // and assignment operator because our class is not 
            // copy-constructible and assignable.
            ThreadPool(const ThreadPool& other)            = delete;
            ThreadPool& operator=(const ThreadPool& other) = delete;

            // Destructor for ThreadPool.
            // This method should free all the system resources
            // like mutexes, conditional variables, threads, etc.
            ~ThreadPool();
            
            // This method schedules a new function which will be executed
            // on the threadpool. The threadpool should choose one of the free
            // threads and execute this function.
            // Note that as it is mentioned above Work is basically a function
            // pointer with the following signature:
            // void* f(void* input)
            // Hints:
            // The threadpool has maximum desgree parallelism which is the count
            // of threads threadpool uses. Let note it by M. If there are N > M works
            // to execute than N - M tasks should wait untile there is a free thread
            // to execute them.
            void run(const Work& work, const Input input);

            // This method waits for all your threads to exit.
            void wait();

            // This method waits for all your threads for the specified time.
            void wait(const std::size_t milliseconds);

            // This method cancels the execution of threadpool threads.
            // The destructor may call this function.
            void exit();
        protected:
            void increaseParallelism();
            void decreaseParallelism();
            static Output executeThread(Input input);
        private:
            pthread_mutex_t     lock_;
            pthread_cond_t      canBeExecuted_;
            std::vector<Worker> workers_;
            std::queue<Work>    works_;
            std::queue<Input>   inputs_;
            std::size_t         minParallelismDegree_;
    };
};

#endif