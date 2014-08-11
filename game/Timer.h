#ifndef TIMER_H
#define	TIMER_H


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

#endif //TIMER_H