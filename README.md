# FactoryDeviceMonitor
A C++ backend framework for factory device monitoring and control.
--Multi-threaded architecture with thread pool and thread-safe queue
--Asynchronous structured logging with timestamp and thread ID
--Device management and command dispatcher for check/repair tasks
--Supports multiple device types(machine tools,conveyors,sensors)

 
A multi-threaded industrial device monitoring and control system implemented in C++.
 
  Project Overview
 
This project simulates a factory device monitoring system, designed with a modular, multi-threaded architecture.
It supports command-line operations for device check/repair, asynchronous logging, and concurrent task execution.
 
Key features:
 
- Multi-threaded command dispatching and task execution

- Thread-safe queue and thread pool

- Structured, asynchronous logging with timestamp and thread ID

- Unified device management for different equipment types
 
 
 
  System Architecture
 
The system is divided into four core layers:
 
1. Input Layer
Console command parser, receiving commands like  CHECK  and  REPAIR .

2. Dispatcher Layer
Distributes commands to the thread pool via a thread-safe queue.

3. Execution Layer
Thread pool processes tasks concurrently for different devices.

4. Logging Layer
A dedicated log thread writes structured logs to file, including timestamp, thread ID, device ID, and command.

![Uploading 754f67cb285a5828196cbcf1caac0020.jpg…]()
