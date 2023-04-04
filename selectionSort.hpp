#include <future>
#include <optional>
#include <random>
#include <chrono>
#include "thread_pool.h"

using namespace std::chrono;

template <typename TIt>
void selectionSort(TIt begin, TIt end, int maxParallelism = 1)
{
	auto minFunc = [](TIt localBegin, TIt localEnd)
	{
		return std::min_element(localBegin, localEnd);
	};

	std::unique_ptr<thread_pool> tp;
	if (maxParallelism > 1)
	{
		tp = std::make_unique<thread_pool>(maxParallelism);
	}

	std::vector<std::future<TIt>> futures(maxParallelism);

	while (begin != end)
	{
		// Делим массив на части, в каждой части ищем минимум параллельно
		// потом ищем минимум из найденных минимумов.
		auto size = std::distance(begin, end);
		auto batchCount = size < 10000 ? 1 : maxParallelism;
		auto wholeBatchSize = size / batchCount;
		auto additionalItems = size % batchCount;
		auto iterationBegin = begin;
		auto localBegin = begin;
		
		for (size_t i = 0; i < batchCount; ++i)
		{
			auto currentBatchSize = wholeBatchSize;
			if (additionalItems > 0)
			{
				++currentBatchSize;
				--additionalItems;
			}

			auto localEnd = localBegin;
			std::advance(localEnd, currentBatchSize);

			std::packaged_task<TIt(TIt, TIt)> minPt(minFunc);
			futures[i] = minPt.get_future();

			if (i == batchCount - 1)
			{
				minPt(localBegin, localEnd);
			}
			else
			{
				tp->submit(std::move(minPt), localBegin, localEnd);
			}

			localBegin = localEnd;
		}

		std::optional<TIt> min;

		for (int i = 0; i < batchCount; ++i)
		{
			auto currIt = futures[i].get();
			if (!min.has_value() || *min.value() > *currIt)
			{
				min = currIt;
			}
		}

		std::swap(*min.value(), *iterationBegin);
		++begin;
	}
}


void testSelectionSort(size_t vectorSize, bool print, size_t maxParallelism)
{

		std::mt19937 gen(steady_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<int> dis(-100, 100);
		auto rand_num([&dis, &gen]() mutable { return dis(gen); });
		std::vector<int> vec(vectorSize);
		std::generate(vec.begin(), vec.end(), rand_num);

		auto start = steady_clock::now();
		selectionSort(vec.begin(), vec.end(), maxParallelism);
		auto end = steady_clock::now();
		std::chrono::duration<double> duration = end - start;

		if (print)
		{
			std::cout << "Отсортированный вектор" << std::endl;
			std::for_each(vec.cbegin(), vec.cend(), [](int v) { std::cout << v << " ";  });
			std::cout << std::endl;
		}

		std::cout << "Время выполнения в " << maxParallelism << " потоках: " << duration.count() << "s" << std::endl;
}