#include "Process.h"

std::thread::id Process::get_thread_id() { return thread_id; }
void Process::set_thread_id(std::thread::id thread_id) { Process::thread_id = thread_id; }
long long Process::get_start_time() { return start_time; }
void Process::set_start_time(long long start_time) { Process::start_time = start_time; }

long long Process::get_wait_time(long long current_time)
{
	wait_time = current_time - start_time;
	return wait_time;
}

void Process::set_finish_time(long long finish_time)
{
	Process::finish_time = finish_time;
}

long long Process::get_finish_time() { return finish_time; }