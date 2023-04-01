#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>
#include <optional>
#include <atomic>
#include <stdexcept>

template <typename T>
class safe_queue
{
public:
	safe_queue() : m_lock(), m_event()
	{

	}

	std::optional<T> pop()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lock;
			if (m_stopped.load())
			{
				return std::optional<T>();
			}

			if (!m_queue.empty())
			{
				auto value = std::move(m_queue.front());
				m_queue.pop();
				return std::move(value);
			}
			else
			{
				m_event.wait(lock);
			}
		}

	}

	void push(T value)
	{
		if (m_stopped.load())
		{
			throw std::runtime_error("Очередь не находится в рабочем состоянии");
		}

		std::lock_guard<std::mutex> lock(m_lock);
		m_queue.push(std::move(value));
		m_event.notify_one();
	}

	void unlock_all()
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_stopped = true;
		m_event.notify_all();
	}

private:
	std::queue<T> m_queue;
	std::condition_variable m_event;
	std::mutex m_lock;
	std::atomic<bool> m_stopped;
};