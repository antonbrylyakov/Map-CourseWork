#ifdef _WIN32
#include "windows.h"
#endif

#include <iostream>
#include <chrono>
#include <string>
#include "thread_pool.h"

using namespace std::chrono;

int main()
{
	setlocale(LC_ALL, "Russian");
#ifdef _WIN32
	SetConsoleCP(1251);
#endif

	thread_pool tp;
	std::mutex lk;

	auto threadSafePrint = [&lk](const std::string& str)
	{
		std::lock_guard<std::mutex> lg(lk);
		std::cout << str << std::endl;
	};

	auto task1 = [&threadSafePrint](int iteration)
	{
		threadSafePrint("Стартует задача типа 1, итерация " + std::to_string(iteration));
		std::this_thread::sleep_for(1s);
		threadSafePrint("Завершается задача типа 1, итерация " + std::to_string(iteration));
	};

	auto task2 = [&threadSafePrint](int iteration)
	{
		threadSafePrint("Стартует задача типа 2, итерация " + std::to_string(iteration));
		std::this_thread::sleep_for(2s);
		threadSafePrint("Завершается задача типа 2, итерация " + std::to_string(iteration));
	};

	for (auto i = 0; i < 10; ++i)
	{
		tp.submit(task1, i);
		std::packaged_task<void(int)> pTask2(task2);
		tp.submit(std::move(pTask2), i);
		//tp.submit(task2, i);
	}

	tp.join();

	return 0;
}
