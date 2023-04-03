#include "thread_pool.h"
#include <stdexcept>

thread_pool::thread_pool(): thread_pool(std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() : 1)
{
}

thread_pool::thread_pool(int size): m_queue(), m_threads()
{
	if (size < 1)
	{
		throw std::invalid_argument("Количество потоков в пуле не может быть менее одного");
	}

	for (size_t i = 0; i < size; ++i)
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
		t.join();
	}

	m_threads.clear();
}
