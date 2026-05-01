#pragma once
#include<string>
#include<thread>
#include<vector>
#include<iostream>

#include<fstream>

#include<format>

#include"Command.h"
#include"ThreadPool.h"
#include"DeviceManager.h"
#include"ThreadSafeQueue.h"
#include"LogMessage.h"

// 声明 cmd_dispatcher 函数
void cmd_dispatcher(ThreadPool& pool,
    ThreadSafeQueue<Command>& cmd_queue,
    ThreadSafeQueue<LogMessage>& log_message_queue,
    DeviceManager& manager);

// 声明 input_worker 函数
void input_worker(ThreadSafeQueue<Command>& cmd_queue);

// 声明 log_worker 函数
void log_worker(ThreadSafeQueue<LogMessage>& log_message_queue,const std::string& file_name = "log.txt");

