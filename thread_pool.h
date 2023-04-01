#pragma once
#include <functional>
#include <thread>
#include <future>
#include "safe_queue.hpp"

class thread_pool
{
public:
	thread_pool();
	explicit thread_pool(int size);
	~thread_pool();
	void submit(std::function<void()> func);
	void submit(std::packaged_task<void()>&& task);
	void join();
private:
	safe_queue<std::packaged_task<void()>> m_queue;
	std::vector<std::thread> m_threads;
	void work();
	void joinThreads();
};