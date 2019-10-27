#ifndef GUI_TIMER_H
#define GUI_TIMER_H

#include "GUI_Container.h"

class GUI_Timer : public GUI_Container {
public:
    GUI_Timer(int _x, int _y, int _width, int _height, int time);
    ~GUI_Timer();

    void update(int time);
};

#endif //GUI_TIMER_H