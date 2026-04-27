#pragma once
#include "Device.h"	
#include<string>
#include<iostream>

enum CommandType
{
	CHECK,
	REPAIR,
};

class Command
{
private:
	CommandType type;
	std::string device_id;
public:
	//CommandType无法默认初始化 ，不能直接使用 Command()= default; 需要提供一个默认构造函数
	Command() : type(CHECK), device_id("") {};//提供一个默认构造函数，避免编译错误
	Command(CommandType t, const std::string& id) : type(t), device_id(id) {}
	const std::string& get_device_id() const { return device_id; }
	CommandType get_type() const { return type; }

	std::string curr_time() const
	{
		time_t now = time(nullptr);
		struct tm tm_buf {};
		localtime_s(&tm_buf, &now);
		char buf[64];
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_buf);

		return std::string(buf);
		//tm tm_buf{};

	}

};



inline void processCommand(const Command& cmd, Device* device)
{
	if (!device)
	{
		std::cout << "Device with ID " << cmd.get_device_id() << " not found.\n";
		return;
	}
	switch (cmd.get_type())
	{
	case CHECK:
	{
		std::cout << "[CHECK] " << device->get_name() << " (" << device->get_id() << ")\n";
		std::cout << "Status:" << (device->is_normal() ? "normal" : "abnormal") << "\n";
		break;
	}
	case REPAIR:
	{
		std::cout << "[REPAIR] Attempting repair on " << device->get_name() << "\n";

		if (!device->is_normal())
		{
			std::cout << "Repairing...\n";
		}
		else
		{
			std::cout << "Device is already normal.\n";
		}
		break;
	}

	}
}
