#pragma once
#include <string>
using std::string;
class MachineToolData//机床
{
private:
	float qualified_rate;//合格率
	string alarm_type;//报警类型
	string power_consumption;//功率消耗
public:
	MachineToolData(const float& b,const string& c,const string& d) :  qualified_rate(b), alarm_type(c), power_consumption(d) {};
	float get_qualified_rate() const { return qualified_rate; }
	string get_alarm_type() const { return alarm_type; }
	string get_power_consumption() const { return power_consumption; }
};

