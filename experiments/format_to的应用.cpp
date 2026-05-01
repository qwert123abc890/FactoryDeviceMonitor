#include<thread>
#include<iostream>
#include<string>
#include<vector>
#include<format>
#include<sstream>
enum class LogLevel
{
	DEBUG,
	INFO,
	WARNING,
	ERROR,
};


struct LogMessage
{
	LogLevel level;
	std::string text;
	std::thread::id thread_id;

public:
	std::string get_level_string(const LogLevel& level) const
	{
		switch (level)
		{
		case LogLevel::DEBUG:  return "DEBUG";
		case LogLevel::INFO:   return "INFO";
		case LogLevel::WARNING: return "WARNING";
		case LogLevel::ERROR:  return "ERROR";
		default:             return "UNKNOWN";
		}

		return "UNKNOWN";
	}

	std::string get_threadID_string(const std::thread::id& tid) const
	{
		std::stringstream ss;
		ss << tid;
		return ss.str();
	}


};

int main()
{
	std::string buf;
	auto it = std::format_to(std::back_inserter(buf), "Hello, {}! The answer is {}.", "world", 42);
	std::format_to(it, " This is a formatted string example.");
	//std::ostringstream oss;  //性能较差，使用std::format_to更高效
	//oss << "Current thread ID: " << std::this_thread::get_id() << ".";
	//std::format_to(std::back_inserter(buf), " Current thread ID: {}.", oss.str());
	std::vector<char> buffer;
	//buffer.data() 返回 char*
	std::format_to(std::back_inserter(buffer), "device_id: {},device_category: {}", 12, "conveyor");
	std::cout.write(buffer.data(), buffer.size());
}

