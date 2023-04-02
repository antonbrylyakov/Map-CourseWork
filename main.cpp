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
		threadSafePrint("������ ���� 1, �������� " + std::to_string(iteration));
	};

	auto task2 = [&threadSafePrint](int iteration)
	{
		threadSafePrint("������ ���� 2, �������� " + std::to_string(iteration));
	};

	for (auto i = 0; i < 100; ++i)
	{
		std::this_thread::sleep_for(1s);
		// ������ �������� �������
		tp.submit(task1, i);

		// ������ �������� packaged_task
		std::packaged_task<void(int)> pTask2(task2);
		tp.submit(std::move(pTask2), i);
	}

	thread_pool tp2(std::move(tp));

	tp.join();

	return 0;
}
