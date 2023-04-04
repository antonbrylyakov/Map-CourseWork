#pragma once
#include <functional>
#include <thread>
#include <future>
#include "safe_queue.hpp"

class thread_pool
{
public:
	//  онструктор по умолчанию создает пул потоков с количеством hardware_concurrency() - 1
	thread_pool();

	//  онструктор с указанием количества потоков в пуле
	explicit thread_pool(int size);

	~thread_pool();
	// ѕомещение задачи без параметров в очередь
	void submit(std::function<void()> func);

	// ѕомещение задачи без параметров в очередь
	void submit(std::packaged_task<void()>&& task);

	// Ўаблон, формирующий обертку над задачей с параметрами и помещающий в очередь
	template <typename F, typename ...Args>
	void submit(F f, Args... a)
	{
		// «ахватываем входные параметры с помощью замыкани€. ¬озможно, не лучший способ, другого не придумал и не нашел.
		// «десь захватить packaged_task мы можем только через move,
		// т.к. это не копируемый тип. Ќо если замыкание не копируетс€, то и л€мбда выражение не копируетс€,
		// а std::function - копируемый тип. ѕоэтому, если вместо make_shared использовать make_unique, код не
		// скомпилируетс€.
		std::function<void(void)> capturePt = [pf = std::make_shared<F>(std::move(f)), tuple = std::make_tuple(std::forward<Args>(a)...)]()
		{
			std::apply((*pf), tuple);
		};

		// —оздаем задачу без параметров и помещаем в очередь
		std::packaged_task<void(void)> parameterless(std::move(capturePt));
		submit(std::move(parameterless));
	}

	// ћетод ожидани€ потоков пула. »спользуетс€ перед завершением работы, если нужно дождатьс€ освобождени€ потоков.
	void join();
	size_t getThreadCount();
private:
	safe_queue<std::packaged_task<void()>> m_queue;
	std::vector<std::thread> m_threads;
	void work();
	void joinThreads();
};