#pragma once
#include "GraphicalController.h"
#include "Event.h"
#include "GUI_Object.h"
#include <list>

class GraphicalController;

class GUI_Layer:
	public GUI_Object
{
public:

	std::list<GUI_Object*> m_items;
	GUI_Object* m_focus;

	GUI_Layer(void);
	~GUI_Layer(void);

	virtual void render(GraphicalController* Graph, int px, int py);
	virtual void on_mouse_click(MouseEventArgs const& e);
	virtual void on_mouse_down(MouseEventArgs const& e);
	virtual void on_mouse_wheel(MouseEventArgs const& e);
	virtual void on_mouse_move(MouseEventArgs const& e);
	virtual void on_key_press(WPARAM w);
	virtual void set_focus(bool state);

	virtual void on_under_cursor(MouseEventArgs const& e);

	virtual void add(GUI_Object* object);
	virtual void add_front(GUI_Object* object);
	virtual void remove(GUI_Object* object);
	
	virtual bool check_region(MouseEventArgs const& e);
	virtual void on_get_focus(GUI_Object* sender);
	virtual void on_item_get_focus(GUI_Object* sender);
	virtual MouseEventArgs set_local_mouse_position(MouseEventArgs const& source);
};

