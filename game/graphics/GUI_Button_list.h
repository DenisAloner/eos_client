#ifndef GUI_BUTTON_LIST_H
#define GUI_BUTTON_LIST_H

#include "GUI_Container.h"

class GUI_Button_list : public GUI_Container {
public:
    GUI_Button_list(int x, int y, int width, int height);
    ~GUI_Button_list();

    virtual void add_item_control(GUI_Object* object);
};

#endif //GUI_BUTTON_LIST_H