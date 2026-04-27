#pragma once
#include<functional>
#include<vector>
#include<utility>//使用move触发移动语义
#include<thread>
#include<memory>
//#include<atomic>
#include<future>
#include"ThreadSafeQueue.h"
#include"DeviceManager.h"
#include"Command.h"
/*
任务 = 一个对象
	 = 数据 + 行为
	 = lambda
*/
using Task = std::function<void()>;
//创建几个线程,
class ThreadPool
{
private:
	ThreadSafeQueue<Task> task_queue;
	std::vector < std::thread > mission;
	//std::atomic<bool> done{ false };

	//调用线程完成任务
	void worker_loop()
	{
		Task task;
		while (task_queue.wait_and_pop(task))
		{
			if (task)
			{
				task();
			}
		}
	}
public:
	//咋初始化
	ThreadPool(size_t n)
	{
		for (size_t i = 0; i < n; i++)
		{
			mission.emplace_back(&ThreadPool::worker_loop,this);
	//此时&用来取成员函数地址,不是引用
			//成员函数能访问成员变量本质是隐含了this指针

		}
	}
	/*
	线程池里的每个线程，本来就应该反复干这个事： 等任务,取任务,执行,队列关了就退出
	*/
	//咋析构
	~ThreadPool()
	{
		task_queue.shutdown();
		for (auto& t : mission)
		{
	//有些 std::thread 对象可能并没有真正绑定线程，或者已经被 join 过了,不能直接对所有线程都join()。
			if (t.joinable())//返回true表示该线程还在运行(或可运行)
			{
	//让线程与资源的生命周期匹配
				t.join();//让主线程等待线程执行完毕,确保:所有任务都已处理,线程不会再访问已销毁的资源
			}
		}
	}
	//创建新线程   函数(任务),参数(设备,即需要manager调用指定设备)等不需要，只要可执行的代码
	//构造线程究竟需要什么
	//提交任务
	void submit(Task task)
//function本身只存在一个指针或引用,指向堆上的"实际文件"，并有管理信息，内部用引用计数来管理堆上的"实际文件"
//非const引用不能绑定到临时对象
	{
		task_queue.push(std::move(task));
	}
		
	void submit(Command cmd,DeviceManager& manager)
	{
		task_queue.push(
	//任务可能要延迟执行，最好按值捕获
			[cmd, &manager]() {
				Device* device = manager.getDevice(cmd.get_device_id());
				if (device)
				{
				processCommand(cmd, device);
				}
			}
		);
	}
	template<typename Func, typename... Args>
	std::future< typename std::invoke_result<Func, Args...>::type >  submit(Func&& f, Args&&... args)
	{
		using ReturnType = typename std::invoke_result<Func, Args...>::type;

		//auto task = std::make_shared < std::packaged_task<ReturnType()> > (std::bind(f, args...));
		/*
		make_shared 写法永远是：std::make_shared<类型>(构造参数)
		*/
		auto task = std::make_shared<std::packaged_task<ReturnType()> >
			(std::bind(std::forward<Func>(f), std::forward<Args>(args)...));

		std::future<ReturnType> result = task->get_future();
		task_queue.push([task]() {
			(*task)();
				});
		//std::future 里面装的是“结果类型”，不是“函数类型”。

		return result;
	}
		
};

