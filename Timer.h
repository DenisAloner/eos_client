#pragma once
class Timer
{
public:

	unsigned int tick;
	int tick_limit;
	int tick_size;

	Timer(int l,int s);
	~Timer();

	void next();
	void cycle();

};

