#ifndef TIMER_H
#define TIMER_H

class Timer {
public:
    int m_tick_limit;
    int m_tick_size;

    Timer(int l, int s);
    ~Timer();

    void next();
    void cycle();
    void set_tick(unsigned int value);
    unsigned int get_tick();

private:
    unsigned int m_tick;
};

#endif //TIMER_H