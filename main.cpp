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
	// Задача сортировки выбором

	auto vectorSize = 200'000;

	std::cout << "Сортировка выбором вектора из " << vectorSize << " элементов" << std::endl;

	testSelectionSort(vectorSize, false, std::thread::hardware_concurrency());
	testSelectionSort(vectorSize, false, 1);

	return 0;
}
