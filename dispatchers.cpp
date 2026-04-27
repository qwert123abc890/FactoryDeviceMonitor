#include"dispatchers.h"
#include <sstream>
/*
input_worker 输入 EXIT
cmd_queue.shutdown()
command_worker 把剩余命令处理完后跳出循环
command_worker 调 log_queue.shutdown()
log_worker 退出
main 的 join() 全结束
*/


void input_worker(ThreadSafeQueue<Command>& cmd_queue)
{
	std::string option, id;
	while (true)
	{
		std::cin >> option;
		if (option == "EXIT")
		{
			cmd_queue.shutdown();
			break;
		}
		CommandType cmd_type;
		if (option == "CHECK")  cmd_type = CommandType::CHECK;
		else if (option == "REPAIR") cmd_type = CommandType::REPAIR;
		else continue;

		std::cin >> id;

		cmd_queue.push({ cmd_type, id });
	}
}
/*
输入 EXIT
→ input_worker 关闭 cmd_queue
→ cmd_dispatcher 发现 cmd_queue 结束
→ cmd_dispatcher 等待线程池任务全部完成
→ cmd_dispatcher 关闭 log_queue
→ log_worker 退出
→ main 中 join 全部完成
*/

/*
但是如果你把任务交给 ThreadPool 后，dispatcher 很快退出，而线程池任务还没执行完，那么日志线程可能提前退出
以 dispatcher 要保存 futures，退出前等任务完成：
*/
void cmd_dispatcher(ThreadPool& pool, ThreadSafeQueue<Command>& cmd_queue, ThreadSafeQueue<LogMessage>& log_message_queue, DeviceManager& manager)
{
	std::vector<std::future<void>> future_results;
	while (true)
	{
		Command cmd;
		if (cmd_queue.wait_and_pop(cmd))
		{
			auto f = pool.submit([cmd, &manager, &log_message_queue]()
				{
					processCommand(cmd, manager.getDevice(cmd.get_device_id()));
					log_message_queue.push({ LogLevel::INFO,
							"Processed command for device " + cmd.get_device_id(),
							std::this_thread::get_id() }); }
			);

			future_results.push_back(std::move(f));

		}
		else
		{
			for (auto& f : future_results)
			{
				try
				{
					f.get();
				}
				catch (const std::exception& e)
				{
					log_message_queue.push({
						LogLevel::ERROR, std::string("Task exception:") + e.what(),
						//e.what() 提供了异常的描述信息,返回的是一个 const char*
						std::this_thread::get_id()
						});
				}
				/*
				这个 catch 语句是一个通用的异常捕获语句，用于捕获所有类型的异常，
				包括那些不是 std::exception 类及其派生类的异常。它可以捕获任何类型的异常，
				甚至是自定义的异常类型，或者是一些非 C++ 标准异常（例如，C 风格的异常）。
				*/
				catch (...)
				{
					log_message_queue.push({
					LogLevel::ERROR,
						std::string("Task exception: ") ,
						std::this_thread::get_id(),
						});

				}
			
				//futures.get() 主要的目的是：确保所有任务都执行完毕后，再去关闭日志队列，避免日志丢失。
			}
			break;
		}

	}
	log_message_queue.shutdown();
}

void log_worker(ThreadSafeQueue<LogMessage>& log_queue, const std::string& file_name)
{
	std::ofstream file(file_name, std::ios::app);
	if (!file.is_open())
	{
		std::cerr << "Unable to open the file!";
		LogMessage msg;
		while (log_queue.wait_and_pop(msg))
		{
			std::cout << std::format("log : {}", msg);
		}

	}
	else
	{
		LogMessage msg;
		while (log_queue.wait_and_pop(msg))
		{
		
			const std::string s = std::format("log : {}", msg);
			file << s << "\n";
			std::cout << s << "\n";
		}
	}

	file.close();
}



/*
原来 command_worker 结束后没有 log_queue.shutdown()，这就是最容易导致“程序不终止”的点。
*/

/*input_worker 是 cmd_queue 的生产者
所以它负责 cmd_queue.shutdown()
command_worker 是 log_queue 的生产者
所以它负责 log_queue.shutdown()
*/





