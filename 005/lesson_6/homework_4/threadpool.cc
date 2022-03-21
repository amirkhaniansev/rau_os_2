#include "threadpool.h"
#include <stdexcept>
#include <iostream>

#include <unistd.h>

namespace RauOs {

ThreadPool::ThreadPool(const std::size_t parallelismDegree)
{
	parallelismDegree_ = parallelismDegree;
	for (size_t i = 0; i < parallelismDegree; ++i)
	{
		Worker worker;
		if (pthread_create(&worker, nullptr, executeThread, this) != 0)
		{
			throw std::runtime_error("Unable to create thread");
		}
		if (pthread_detach(worker) != 0)
		{
			throw std::runtime_error("Unable to detach the thread");
		}
		workers_.push_back(worker);		
	}
	pthread_mutex_init(&lock_, nullptr);
	pthread_cond_init(&canBeExecuted_, nullptr);	
}

void ThreadPool::run(const Work& work, const Input input){
	pthread_mutex_lock(&lock_);
	works_.push(work);
	inputs_.push(input);
	pthread_cond_signal(&canBeExecuted_);
	pthread_mutex_unlock(&lock_);
}

void* ThreadPool::executeThread(void* input) {
	ThreadPool* t = (ThreadPool*)input;
	while (true) {
		pthread_mutex_lock(&t->lock_);
		while (t->works_.empty()) {
			pthread_cond_wait(&t->canBeExecuted_, &t->lock_);
		}
		Work current_work = t->works_.front();
		t->works_.pop();
		Input current_input = t->inputs_.front();
		t->inputs_.pop();
		pthread_mutex_unlock(&t->lock_);
		current_work(current_input);
	}
	return NULL;
}

void ThreadPool::wait()
{
	for (const auto& worker : workers_)
	{
		pthread_join(worker, nullptr);
	}
}

void ThreadPool::wait(const std::size_t milliseconds)
{
	usleep(milliseconds * 1000UL);
}

ThreadPool::~ThreadPool()
{	
	pthread_cond_destroy(&canBeExecuted_);
	pthread_mutex_destroy(&lock_);
}

}