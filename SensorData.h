#pragma once
#include<string>
using std::string;
class SensorData
{
private:
	float accuracy;//精度
	float operating_temperature;
	int signal_strength;
public:
	SensorData(const float& b,const float& c,const int& d) :  accuracy(b), operating_temperature(c), signal_strength(d) {};
	float get_accuracy() const { return accuracy; }
	float get_operating_temperature() const { return operating_temperature; }
	int get_signal_strength() const { return signal_strength; }
};

