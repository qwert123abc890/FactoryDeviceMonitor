#pragma once
#include "Device.h"
#include "MachineToolData.h"
using std::string;
class MachineTool :public Device
{
private:
	MachineToolData data;//机床数据
public:
	MachineTool(const string& a,const string& b,const MachineToolData& c) : Device(a, b), data(c) {};
    bool is_normal() const override
	{
		if (data.get_qualified_rate() > 0.95 && data.get_alarm_type() == "none" && data.get_power_consumption() < "1000W")
			return true;
		else
			return false;
	}
  void update_data() override
	{
		// no-op for demo
	}
};

