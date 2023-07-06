#pragma once
#include <chrono>
#include <thread>

class Process
{
public:
	std::thread::id get_thread_id();
	void set_thread_id(std::thread::id thread_id);
	long long get_start_time();
	void set_start_time(long long start_time);
	long long get_wait_time(long long current_time);
	void set_finish_time(long long finish_time);
	long long get_finish_time();

private:
	std::thread::id thread_id;
	long long start_time;
	long long wait_time;
	long long finish_time;
};