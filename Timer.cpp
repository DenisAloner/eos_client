#include "Timer.h"
#include <thread>
#include <chrono>


Timer::Timer(int l, int s)
{
	tick = 0;
	tick_limit = l;
	tick_size = s;
}


Timer::~Timer()
{
}

void Timer::next()
{
	if (tick > tick_limit-2)
	{
		tick = 0;
	} else
	{
		tick += 1;
	}
}

void Timer::cycle()
{
	while (true)
	{
		std::chrono::milliseconds Duration(tick_size);
		std::this_thread::sleep_for(Duration);
		next();
	}
}
