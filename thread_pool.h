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
	template <typename Func, typename ...TArgs>
	void submit(Func&& func, TArgs&&... args)
	{
		auto argsTuple = std::make_tuple(std::forward<TArgs>(args)...);
		auto parameterless = [f = std::move(func), args = std::move(argsTuple)]
		{
			std::apply(std::move(f), std::move(args));
		};

		//std::packaged_task<void()> task(std::move(parameterless));
		//submit(std::move(task));
	}

	/*template <typename ...TArgs>
	void submit1(std::packaged_task<void(TArgs&&...)>&& func, TArgs&&... args)
	{
	}*/

	void join();
private:
	safe_queue<std::packaged_task<void()>> m_queue;
	std::vector<std::thread> m_threads;
	void work();
	void joinThreads();
};