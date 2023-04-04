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
	// ������ ���������� �������

	auto vectorSize = 100'000;

	std::cout << "���������� ������� ������� �� " << vectorSize << " ���������" << std::endl;

	testSelectionSort(vectorSize, false, 6);
	testSelectionSort(vectorSize, false, 1);

	return 0;
}
