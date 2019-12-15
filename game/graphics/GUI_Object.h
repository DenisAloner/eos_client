#ifndef GUI_OBJECT_H
#define GUI_OBJECT_H

#include "Definiton.h"
#include "Event.h"

class GraphicalController;

class GUI_Object {
public:
    position_t<int> m_position;
    dimension_t<int> m_size;

    bool focused;

    GUI_Object();
    ~GUI_Object();

    Event<MouseEventArgs> mouse_start_drag;
    Event<MouseEventArgs> mouse_drag;
    Event<MouseEventArgs> mouse_end_drag;
    Event<MouseEventArgs> mouse_click;
    Event<MouseEventArgs> mouse_down;
    Event<MouseEventArgs> mouse_wheel;
    Event<MouseEventArgs> mouse_move;
    Event<WPARAM> key_press;
    Event<GUI_Object*> destroy;
    Event<GUI_Object*> get_focus;
    Event<GUI_Object*> lose_focus;

    virtual void render(GraphicalController* graph, int px, int py) = 0;
    virtual void render_on_canvas();
    virtual void on_mouse_click(MouseEventArgs const& e);
    virtual void on_mouse_start_drag(MouseEventArgs const& e);
    virtual void on_mouse_drag(MouseEventArgs const& e);
    virtual void on_mouse_end_drag(MouseEventArgs const& e);
    virtual void on_mouse_down(MouseEventArgs const& e);
    virtual void on_mouse_wheel(MouseEventArgs const& e);
    virtual void on_mouse_move(MouseEventArgs const& e);
    virtual void on_key_press(WPARAM w);
    virtual void on_get_focus(GUI_Object* sender);
    virtual void on_lose_focus(GUI_Object* sender);
    virtual bool check_region(MouseEventArgs const& e);
    virtual void set_focus(bool state);
    virtual void resize(int width, int height);
    void execute_in_render_thread(std::function<void()>&& func);
};

#endif //GUI_OBJECT_H
