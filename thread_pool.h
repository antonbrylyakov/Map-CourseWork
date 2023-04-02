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
	template <typename F, typename ...Args>
	void submit(F f, Args... a)
	{
		std::function<void(void)> capturePt = [pf = std::make_shared<F>(std::move(f)), tuple = std::make_tuple(std::forward<Args>(a)...)]()
		{
			std::apply((*pf), tuple);
		};

		std::packaged_task<void(void)> parameterless(std::move(capturePt));

		submit(std::move(parameterless));
	}

	void join();
private:
	safe_queue<std::packaged_task<void()>> m_queue;
	std::vector<std::thread> m_threads;
	void work();
	void joinThreads();
};