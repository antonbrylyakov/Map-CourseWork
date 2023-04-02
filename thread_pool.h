#pragma once
#include <functional>
#include <thread>
#include <future>
#include "safe_queue.hpp"

class thread_pool
{
public:
	// ����������� �� ��������� ������� ��� ������� � ����������� hardware_concurrency() - 1
	thread_pool();

	// ������ � ������ �� ����������, ������� ����������� ����������� � �������� ����������� ������������ ������
	//thread_pool(const thread_pool&) = delete;
	//thread_pool& operator=(const thread_pool&) = delete;
	//thread_pool(thread_pool&& src) = default;
	//thread_pool& operator=(thread_pool&& src) = default;

	// ����������� � ��������� ���������� ������� � ����
	explicit thread_pool(int size);

	~thread_pool();
	// ��������� ������ ��� ���������� � �������
	void submit(std::function<void()> func);

	// ��������� ������ ��� ���������� � �������
	void submit(std::packaged_task<void()>&& task);

	// ������, ����������� ������� ��� ������� � ����������� � ���������� � �������
	template <typename F, typename ...Args>
	void submit(F f, Args... a)
	{
		// ����������� ������� ��������� � ������� ���������. ��������, �� ������ ������, ������� �� �������� � �� �����.
		// ����� ��������� packaged_task �� ����� ������ ����� move,
		// �.�. ��� �� ���������� ���. �� ���� ��������� �� ����������, �� � ������ ��������� �� ����������,
		// � std::function - ���������� ���. �������, ���� ������ make_shared ������������ make_unique, ��� ��
		// ��������������.
		std::function<void(void)> capturePt = [pf = std::make_shared<F>(std::move(f)), tuple = std::make_tuple(std::forward<Args>(a)...)]()
		{
			std::apply((*pf), tuple);
		};

		// ������� ������ ��� ���������� � �������� � �������
		std::packaged_task<void(void)> parameterless(std::move(capturePt));
		submit(std::move(parameterless));
	}

	// ����� �������� ������� ����. ������������ ����� ����������� ������, ���� ����� ��������� ������������ �������.
	void join();
private:
	safe_queue<std::packaged_task<void()>> m_queue;
	std::vector<std::thread> m_threads;
	void work();
	void joinThreads();
};