#include <thread>
#include <chrono>
#include "Timer.h"

using namespace std;

Timer::Timer()
{
	_isRun = false;
	_microsseconds = 0;
	_wait_microseconds_time = 1000;
}

void Timer::start(int _wait_microseconds_time)
{
	_isRun = true;
	_microsseconds = 0;
	Timer::_wait_microseconds_time = _wait_microseconds_time;

	thread th(&Timer::callback, this, std::ref(_isRun));
	th.detach();
}

void Timer::stop()
{
	_isRun = false;
}

void Timer::callback(bool &_isRun)
{
	while (_isRun)
	{
		this_thread::sleep_for(chrono::microseconds(_wait_microseconds_time));
		_microsseconds += _wait_microseconds_time;
	}
}

int Timer::get_microseconds_delta()
{
	return _microsseconds;
}