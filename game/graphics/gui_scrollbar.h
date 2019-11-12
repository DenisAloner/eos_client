#ifndef GUI_SCROLLBAR_H
#define GUI_SCROLLBAR_H

#include "GUI_Object.h"

class GUI_Scrollable_container;

class GUI_Scrollbar_vertical : public GUI_Object {
public:
    GUI_Scrollable_container* m_owner;

    int m_bar_top;
    int m_bar_height;
    int m_pos;

    GUI_Scrollbar_vertical(GUI_Scrollable_container* owner);
    void render(GraphicalController* graph, int px, int py) override;
    void content_update();
    MouseEventArgs set_local_mouse_control(MouseEventArgs const& source);
    void on_mouse_click(MouseEventArgs const& e) override;
    void on_mouse_start_drag(MouseEventArgs const& e) override;
    void on_mouse_drag(MouseEventArgs const& e) override;
    bool check_region(MouseEventArgs const& e) override;
    void set_scroll_top(int value);
};

#endif //GUI_SCROLLBAR_H
