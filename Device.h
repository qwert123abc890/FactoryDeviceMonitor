#pragma once
#include<string>
using std::string;
class Device
{
private:
	string id;
	string name;
public:
	Device(string a, string b) : id(a), name(b) {};
    virtual bool is_normal() const = 0;//纯虚函数，判断设备是否正常
	virtual void update_data() = 0;//纯虚函数，更新设备数据
	virtual ~Device() {};//虚析构函数，确保正确析构派生类对象
	string get_id() const { return id; };
	string get_name() const { return name; };
};

