#pragma once
#include <vector>
#include <memory>
#include "Device.h"
#include "DeviceManager.h"

class RepairService
{
public:
	// 非拥有指针，不会转移设备所有权
	bool checkDevice(const Device* device)
	{
		if (device == nullptr)
			return false;
		return device->is_normal();
	}

	// 检查所有设备，返回是否全部合格。若需日志，可由调用方在检查失败后获取设备 id 并打印。
	bool checkAll(const DeviceManager& manager)
	{
		for (Device* device : manager.getAllDevicePointers())
		{
			if (device == nullptr) continue;
			if (!device->is_normal())
			{
				return false;
			}
		}
		return true;
	}

	// 非拥有指针的修复接口（示例），目前仅检查并返回结果
	bool repairDevice(Device* device)
	{
		if (device == nullptr) return false;
		// TODO: 实现具体修复逻辑
		return device->is_normal();
	}
};

