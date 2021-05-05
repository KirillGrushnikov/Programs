#pragma once
#include <chrono>
#include <iostream>

class CodeTimer
{
	private:
	std::chrono::time_point<std::chrono::steady_clock> start, finish;
	public:
	CodeTimer()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	~CodeTimer()
	{
		finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = finish - start;
		std::cout << "Lead time: " << duration.count() << " seconds" << std::endl;
	}
};