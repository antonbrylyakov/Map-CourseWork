#include <iostream>
#include <chrono>
#include "thread_pool.h"

using namespace std::chrono;

int main()
{
	thread_pool tp;

	auto task1 = []()
	{
		std::cout << "Стартует задача типа 1";
		std::this_thread::sleep_for(1s);
		std::cout << "Завершается задача типа 1";
	};

	auto task2 = []()
	{
		std::cout << "Стартует задача типа 2";
		std::this_thread::sleep_for(2s);
		std::cout << "Завершается задача типа 2";
	};

	tp.submit(task1);
	tp.submit(task2);

	return 0;
}
