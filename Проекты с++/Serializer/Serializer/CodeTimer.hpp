#pragma once
#include <chrono>

class CodeTimer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> _start, _finish;
	std::chrono::duration<float> duration;

public:
	void start()
	{
		_start = std::chrono::high_resolution_clock::now();
	}

	void stop()
	{
		_finish = std::chrono::high_resolution_clock::now();
		duration = _finish - _start;
	}

	std::chrono::duration<float> get() const
	{
		return duration;
	}
};