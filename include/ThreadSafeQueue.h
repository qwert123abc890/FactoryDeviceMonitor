#pragma once
// 线程之间的通信桥梁（生产者 → 消费者）
/*
一个线程往里放任务（push）
另一个线程从里面拿任务（pop）
如果没有任务 → 自动等待
如果结束 → 自动退出
*/
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

#include<utility>


template<typename T>
class ThreadSafeQueue
{
private:
	std::queue<T> q;
	std::mutex mtx;
	std::condition_variable cv;
	std::atomic<bool> done{ false };
public:
	~ThreadSafeQueue()
	{
		shutdown();
	}
	void push(const T& value)
	{
		{
			std::lock_guard<std::mutex> lock(mtx);
			q.push(value);
		}
		cv.notify_one();
	}
	
//事件驱动
//wait_and_pop()返回true，线程worker继续处理任务，cmd是有效的；
//wait_and_pop()返回false，线程worker退出循环，结束线程。
//如果没有任务，线程worker会自动等待；
//何时返回false,当没有任务且生产者已经结束（done为true）时，返回false，通知消费者线程退出。
	bool empty()
	{
		std::lock_guard<std::mutex> lock(mtx);
		return q.empty();
	}
	bool wait_and_pop(T& value)
	{
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [&] { return !q.empty() || done.load(std::memory_order_acquire); });

		if (q.empty() && done.load(std::memory_order_acquire)) return false; // 没有任务且已结束

		//value = q.front();
		value = std::move(q.front());
		q.pop();

		return true; // 成功拿到任务
	}
	void shutdown()
	{
		done.store(true, std::memory_order_release);
		cv.notify_all();
	}
};
