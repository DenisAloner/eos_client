#ifndef GUI_SCROLLABLE_CONTAINER_H
#define GUI_SCROLLABLE_CONTAINER_H

#include "GUI_Container.h"
#include "gui_scrollbar.h"

class GUI_Scrollable_container : public GUI_Container {

public:
    GUI_Scrollbar_vertical m_scrollbar;

    std::list<GUI_Object*>::iterator start_render;
    std::list<GUI_Object*>::iterator end_render;

    dimension_t<int> m_content_size;

    GUI_Scrollable_container(int x, int y, int width, int height, bool border_visible = true);

    virtual void add_item_control(GUI_Object* object);
    void on_mouse_wheel(MouseEventArgs const& e) override;
    void on_mouse_click(MouseEventArgs const& e) override;
    void on_mouse_down(MouseEventArgs const& e) override;
    void on_mouse_start_drag(MouseEventArgs const& e) override;
    void on_mouse_drag(MouseEventArgs const& e) override;
    void on_mouse_move(MouseEventArgs const& e) override;
    void set_scroll(int dy) override;
    void set_scroll2(int dy);
    virtual void update();
    void render(GraphicalController* graph, int px, int py) override;
    void resize(int width, int height) override;
};

#endif //GUI_SCROLLABLE_CONTAINER_H
