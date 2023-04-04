#include "thread_pool.h"
#include <stdexcept>

thread_pool::thread_pool(): thread_pool(0)
{
}

thread_pool::thread_pool(int size): m_queue(), m_threads()
{
	if (size < 0)
	{
		throw std::invalid_argument("Количество потоков в пуле должно быть неотрицательным. 0 - автовыбор");
	}

	auto threadCount =
		size > 0 
		? size 
		:(std::thread::hardware_concurrency() > 1 
			? std::thread::hardware_concurrency()
			: 1);

	for (size_t i = 0; i < threadCount; ++i)
	{
		m_threads.push_back(std::thread(&thread_pool::work, this));
	}
}

thread_pool::~thread_pool()
{
	m_queue.stop();
	joinThreads();
}

void thread_pool::submit(std::function<void()> func)
{
	std::packaged_task<void()> task([func] { func(); });
	m_queue.push(std::move(task));
}

void thread_pool::submit(std::packaged_task<void()>&& task)
{
	m_queue.push(std::move(task));
}

void thread_pool::join()
{
	m_queue.stopWhenEmpty();
	joinThreads();
}

size_t thread_pool::getThreadCount()
{
	return m_threads.size();
}

void thread_pool::work()
{
	bool terminated;
	do
	{
		auto task = m_queue.pop();
		terminated = !task.has_value();
		if (!terminated)
		{
			task.value()();
		}
	} while (!terminated);
}

void thread_pool::joinThreads()
{
	for (auto& t : m_threads)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
}
