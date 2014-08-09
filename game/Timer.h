#pragma once
class Timer
{
public:

	unsigned int m_tick;
	int m_tick_limit;
	int m_tick_size;

	Timer(int l,int s);
	~Timer();

	void next();
	void cycle();

};

