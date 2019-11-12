#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H

#include "GUI_Layer.h"

class GUI_Container : public GUI_Layer {
public:
    GUI_Container(int x, int y, int width, int height, bool border_visible = true);
    ~GUI_Container();

    bool m_border_visible;
    position_t<int> m_scroll;

    bool check_region(MouseEventArgs const& e) override;
    void on_mouse_click(MouseEventArgs const& e) override;
    void render(GraphicalController* graph, int px, int py) override;
    void on_mouse_wheel(MouseEventArgs const& e) override;
    void on_mouse_down(MouseEventArgs const& e) override;
    void on_mouse_move(MouseEventArgs const& e) override;
    void resize(int width, int height) override;
    virtual void set_scroll(int dy);
    MouseEventArgs set_local_mouse_position(MouseEventArgs const& source) override;
};

#endif //GUI_CONTAINER_H
