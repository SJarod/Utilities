#include <iostream>

#include <chrono>

#include "utils/multithread/threadpool.hpp"

Utils::ThreadPool::ThreadPool(uint nThreads)
{
	for (uint i = 0; i < nThreads; ++i)
	{
		threads.push_back(std::jthread(std::bind(&ThreadPool::poolRoutine, this, i)));
	}

	running.test_and_set();
}

Utils::ThreadPool::~ThreadPool()
{
	if (running.test())
	{
		while (!workerQueue.empty())
			std::this_thread::yield();

		running.clear();
	}

	// jthreads are auto joining
	threads.clear();
}

void Utils::ThreadPool::addTask(std::function<void()> fct, const bool parallel)
{
	if (parallel)
	{
		std::lock_guard<std::mutex> guard(workerQueueMX);
		workerQueue.push_back(Task(fct));
	}
	else
	{
		std::lock_guard<std::mutex> guard(mainQueueMX);
		mainQueue.push_back(Task(fct));
	}
}

void Utils::ThreadPool::poolRoutine(int id)
{
	while (running.test())
	{
		if (workerQueue.empty())
			continue;

		Task t;
		{
			std::lock_guard<std::mutex> guard(workerQueueMX);

			t = workerQueue.front();
			workerQueue.pop_front();
		}
		t();

		auto now = (float)std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::steady_clock::now().time_since_epoch()).count();
		lastTaskTime.store(now);
		
		//printThreadId(id);
	}
}

void Utils::ThreadPool::pollMainQueue()
{
	std::lock_guard<std::mutex> guard(mainQueueMX);
	
	while (!mainQueue.empty())
	{
		mainQueue.front()();
		mainQueue.pop_front();
	}
}

void Utils::ThreadPool::printThreadId(int id)
{
	std::lock_guard<std::mutex> guard(printMX);
	std::cout << "thread #" << id << std::endl;
}