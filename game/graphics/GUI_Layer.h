#ifndef GUI_LAYER_H
#define GUI_LAYER_H

#include "Event.h"
#include "GUI_Object.h"
#include "GraphicalController.h"
#include <list>

class GraphicalController;

class GUI_Layer : public GUI_Object {
public:
    std::list<GUI_Object*> m_items;
    GUI_Object* m_focus;

    GUI_Layer(int x, int y, int width, int height);
    ~GUI_Layer();

    void render(GraphicalController* graph, int px, int py) override;
    void on_mouse_click(MouseEventArgs const& e) override;
    void on_mouse_down(MouseEventArgs const& e) override;
    void on_mouse_wheel(MouseEventArgs const& e) override;
    void on_mouse_move(MouseEventArgs const& e) override;
    void on_mouse_start_drag(MouseEventArgs const& e) override;
    void on_mouse_drag(MouseEventArgs const& e) override;
    void on_mouse_end_drag(MouseEventArgs const& e) override;
    void on_key_press(WPARAM w) override;
    void set_focus(bool state) override;
    virtual void add(GUI_Object* object);
    virtual void add_front(GUI_Object* object);
    virtual void remove(GUI_Object* object);

    bool check_region(MouseEventArgs const& e) override;
    void on_get_focus(GUI_Object* sender) override;
    virtual void on_item_get_focus(GUI_Object* sender);
    virtual MouseEventArgs set_local_mouse_position(MouseEventArgs const& source);
};

#endif //GUI_LAYER_H
