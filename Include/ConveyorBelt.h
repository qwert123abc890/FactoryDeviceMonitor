#pragma once
#include "Device.h"
#include"ConveyorBeltData.h"
class ConveyorBelt :public Device
{
	private:
		ConveyorBeltData data;//传送带数据
	public:
		ConveyorBelt(const string& a,const string& b,const ConveyorBeltData& c) : Device(a, b), data(c) {};
        bool is_normal() const override
		{
			if (data.get_belt_wear_level() < 0.5 && data.get_jam_count() < 5 && data.get_speed() > 0)
				return true;
			else
				return false;
		}
		void update_data() override
		{
			
		}
};

