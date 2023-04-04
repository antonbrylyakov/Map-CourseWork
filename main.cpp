#ifdef _WIN32
#include "windows.h"
#endif

#include <iostream>
#include <chrono>
#include <string>
#include "thread_pool.h"
#include "selectionSort.hpp"

using namespace std::chrono;

int main()
{
	setlocale(LC_ALL, "Russian");
#ifdef _WIN32
	SetConsoleCP(1251);
#endif
// Задача одновременного вывода на экран
/*
	thread_pool tp;
	std::mutex lk;

	auto threadSafePrint = [&lk](const std::string& str)
	{
		std::lock_guard<std::mutex> lg(lk);
		std::cout << str << std::endl;
	};

	auto task1 = [&threadSafePrint](int iteration)
	{
		threadSafePrint("Задача типа 1, итерация " + std::to_string(iteration));
	};

	auto task2 = [&threadSafePrint](int iteration)
	{
		threadSafePrint("Задача типа 2, итерация " + std::to_string(iteration));
	};

	for (auto i = 0; i < 20; ++i)
	{
		std::this_thread::sleep_for(1s);
		// Пример передачи функции
		tp.submit(task1, i);

		// Пример передачи packaged_task
		std::packaged_task<void(int)> pTask2(task2);
		tp.submit(std::move(pTask2), i);
	}

	tp.join();
	*/

	// Задача сортировки выбором

	auto vectorSize = 500'000;

	testSelectionSort(vectorSize, 6);
	testSelectionSort(vectorSize, 1);

	return 0;
}
