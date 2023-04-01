#include <iostream>
#include <chrono>
#include "thread_pool.h"

using namespace std::chrono;

int main()
{
	thread_pool tp;

	auto task1 = []()
	{
		std::cout << "�������� ������ ���� 1";
		std::this_thread::sleep_for(1s);
		std::cout << "����������� ������ ���� 1";
	};

	auto task2 = []()
	{
		std::cout << "�������� ������ ���� 2";
		std::this_thread::sleep_for(2s);
		std::cout << "����������� ������ ���� 2";
	};

	tp.submit(task1);
	tp.submit(task2);

	return 0;
}
