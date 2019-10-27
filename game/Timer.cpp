#include "Timer.h"
#include <chrono>
#include <thread>

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
    set_tick(m_tick + 1);
}

void Timer::cycle()
{
    while (true) {
        std::chrono::milliseconds Duration(m_tick_size);
        std::this_thread::sleep_for(Duration);
        next();
    }
}

void Timer::set_tick(unsigned int value)
{
    if (value > m_tick_limit - 1) {
        m_tick = 0;
    } else {
        m_tick = value;
    }
}

unsigned int Timer::get_tick()
{
    return m_tick;
}
