#include"ThreadPool.h"
#include"ThreadSafeQueue.h"
//#include"workers.h"
#include"Command.h"

#include"ConveyorBeltData.h"
#include"MachineToolData.h"
#include"SensorData.h"
#include"MachineTool.h"
#include"ConveyorBelt.h"
#include"Sensor.h"

#include"DeviceManager.h"
#include"dispatchers.h"

#include<future>
#include<string>
#include<iostream>
int main()
{
	MachineToolData mt_good(0.98f, "none", "900W");
	MachineToolData mt_bad(0.80f, "overheat", "1200W");

	ConveyorBeltData cb_good(100, 1, 0.2f);
	ConveyorBeltData cb_bad(0, 6, 0.7f);

	SensorData s_good(0.95f, 40.0f, 80);
	SensorData s_bad(0.85f, 90.0f, 30);

	DeviceManager manager;
	manager.addDevice(std::make_unique<MachineTool>("MT01", "CNC Mill", mt_good));
	manager.addDevice(std::make_unique<MachineTool>("MT02", "Lathe", mt_bad));
	manager.addDevice(std::make_unique<ConveyorBelt>("CB01", "LineA", cb_good));
	manager.addDevice(std::make_unique<ConveyorBelt>("CB02", "LineB", cb_bad));
	manager.addDevice(std::make_unique<Sensor>("S01", "TempSensor", s_good));
	manager.addDevice(std::make_unique<Sensor>("S02", "VibeSensor", s_bad));


	ThreadPool  pool(6);
	std::future<int> add_value = pool.submit([](int a, int b) {
		return a + b;
		}, 4, 8);
	std::future<std::string> hello = pool.submit([](std::string s) {
		std::cout << s << std::endl;
		return std::string("Task completed");
		}, std::string("hello"));
	std::cout << add_value.get() << "\n";
	std::cout << hello.get() << "\n";

	ThreadSafeQueue<Command> cmd_queue;
	ThreadSafeQueue<LogMessage> log_message_queue;
	std::thread input_thread(input_worker, std::ref(cmd_queue));
	std::thread cmd_thread(cmd_dispatcher, std::ref(pool), std::ref(cmd_queue), std::ref(log_message_queue),std::ref(manager));
	std::thread log_thread(static_cast<void(*)(ThreadSafeQueue<LogMessage>&, const std::string&) > (log_worker), std::ref(log_message_queue), std::string("log.txt"));


	if (input_thread.joinable()) input_thread.join();
	if (cmd_thread.joinable()) cmd_thread.join();
	if (log_thread.joinable()) log_thread.join();

	return 0;
	//
}
//当std::thread 对象析构时仍处于joinable()状态时（未join或detach)，c++标准强制触发std::terminate()。需确保所有线程对象在析构前完成join或detach.
