#pragma once
#include "ThreadSafeQueue.h"
#include <string>
#include <iostream>
#include <thread>
#include <sstream>
#include <ctime>

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

	LogMessage() : level(LogLevel::INFO), text(""), thread_id(std::this_thread::get_id()) {};
	LogMessage(LogLevel lvl, const std::string& txt, std::thread::id tid) : level(lvl), text(txt), thread_id(tid) {};

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

template<>
struct std::formatter<LogMessage> : std::formatter<std::string>
{
	auto format(const LogMessage& log, std::format_context& context) const
	{
		std::string string_formatted = "[" + log.get_level_string(log.level) + "] : " + log.text + " (thread_id: " + log.get_threadID_string(log.thread_id) + ")" + "(time: " + log.curr_time() + ")";
		return std::formatter<std::string>::format(string_formatted, context);
	}
};



