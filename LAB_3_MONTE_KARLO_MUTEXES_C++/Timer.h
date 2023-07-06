#pragma once
using namespace std;

class Timer
{
	private:
		bool _isRun;
		int _microsseconds;
		int _wait_microseconds_time;
		void callback(bool &_isRun);

	public:
		Timer();
		void start(int _wait_microseconds_time = 1000);
		void stop();
		int get_microseconds_delta();
};