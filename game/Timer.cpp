#include "Timer.h"
#include <thread>
#include <chrono>


Timer::Timer(int l, int s)
{
	m_tick = 0;
	m_tick_limit = l;
	m_tick_size = s;
}


Timer::~Timer()
{
}

void Timer::next()
{
	if (m_tick > m_tick_limit - 2)
		m_tick = 0;
	else
		m_tick += 1;
}

void Timer::cycle()
{
	while (true)
	{
		std::chrono::milliseconds Duration(m_tick_size);
		std::this_thread::sleep_for(Duration);
		next();
	}
}
