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

	// Метод получения значения из очереди. Если в череди нет значений, 
	// вызывающий поток ожидает их появления.
	// Если возвращается "пустое" значение, значит очередь переведена в режим завершения работы.
	std::optional<T> pop()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lock(m_lock);
			if (m_stopped)
			{
				return std::optional<T>();
			}

			if (!m_queue.empty())
			{
				auto value = std::move(m_queue.front());
				m_queue.pop();
				return std::move(value);
			}
			else if (m_waitingEmpty)
			{
				return std::optional<T>();
			}
			else
			{
				m_event.wait(lock);
			}
		}

	}

	// Метод помещения значения в очередь
	// Уведомляется один из потоков, ожидающих значения
	void push(T value)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		if (m_stopped || m_waitingEmpty)
		{
			throw std::runtime_error("Очередь в остановленном состоянии");
		}
		m_queue.push(std::move(value));
		m_event.notify_one();
	}

	// Метод завершения работы очереди.
	// ожидающие потоки уведомляются, и возвращается "пустое" значение
	void stop()
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_stopped = true;
		m_event.notify_all();
	}

	// Метод завершения работы в случае, если очередь пуста.
	// Если значения в очереди есть, то один или несколько потоков получат эти значния,
	// пока очередь не опустеет.
	void stopWhenEmpty()
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_waitingEmpty = true;
		m_event.notify_all();
	}

private:
	std::queue<T> m_queue;
	std::condition_variable m_event;
	std::mutex m_lock;
	// Флаг остановки очереди
	bool m_stopped = false;
	// флаг ожидания опустошения очереди для завершения работы
	bool m_waitingEmpty = false;
};