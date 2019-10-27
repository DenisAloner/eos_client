#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "GUI_Container.h"
#include "GUI_Item.h"
#include <list>

class GUI_Header : public GUI_Container {
public:
    Event<GUI_Object*> close;
    GUI_Header(int x, int y, int width, int height, const std::u16string& text);
    ~GUI_Header();

    void on_close_button_click();
    void resize(int width, int height) override;
};

class GUI_Window : public GUI_Container {
public:
    Event<GUI_Object*> close;

    GUI_Window(int x, int y, int width, int height, const std::u16string& name);
    ~GUI_Window();

    position_t<int> m_initial_position;

    //virtual void on_mouse_down(MouseEventArgs const& e);
    /*virtual void on_mouse_move(MouseEventArgs const& e);*/
    void on_mouse_start_drag(MouseEventArgs const& e) override;
    void on_mouse_drag(MouseEventArgs const& e) override;
    void on_mouse_end_drag(MouseEventArgs const& e) override;
    virtual void on_close(GUI_Object* e);
    virtual void on_header_close();
    void resize(int width, int height) override;
    void on_mouse_wheel(MouseEventArgs const& e) override;
    void add(GUI_Object* object) override;
    virtual rectangle_t<int> client_rect();

private:
    GUI_Header* m_header;
};

class GUI_Body;

class GUI_body_window : public GUI_Window {
public:
    GameObject* m_object;
    GUI_Body* m_item;
    GUI_body_window(int x, int y, int width, int height, const std::u16string& name, GameObject*& object);
    std::list<std::u16string> m_text;
    void update_info();
};

#endif GUI_WINDOW_H
