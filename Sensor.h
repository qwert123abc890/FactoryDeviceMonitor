#pragma once
#include "Device.h"
#include "SensorData.h"
class Sensor :public Device
{
private:
	SensorData data;//传感器数据
public:
	Sensor(const string& a,const string& b,const SensorData& c) : Device(a, b), data(c) {};
    bool is_normal() const override
	{
		if (data.get_accuracy() > 0.9 && data.get_operating_temperature() < 80 && data.get_signal_strength() > 50)
			return true;
		else
			return false;
	}
    void update_data() override
	{
	
	}
};

