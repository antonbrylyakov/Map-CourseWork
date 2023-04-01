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
	void submit(std::packaged_task<void()> task);
	template <typename Func, typename ...TArgs>
	void submit(Func&& func, TArgs&&... args)
	{
		auto parameterless = std::bind(func, std::forward<decltype(args)>(args)...);
		submit([moved = std::move(parameterless)]() { moved(); });
	}

	void join();
private:
	safe_queue<std::packaged_task<void()>> m_queue;
	std::vector<std::thread> m_threads;
	void work();
	void joinThreads();
};