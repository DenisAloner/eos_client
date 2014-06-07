#pragma once
#include "Event.h"
#include "GraphicalController.h"
#include "Definiton.h"
#include <string>

class GraphicalController;

class GUI_Object
{
public:

	position_t m_position;
	dimension_t m_size;

	bool focused;

	GUI_Object(void);
	~GUI_Object(void);

	Event<MouseEventArgs> mouse_click;
	Event<MouseEventArgs> mouse_down;
	Event<MouseEventArgs> mouse_wheel;
	Event<MouseEventArgs> mouse_move;
	Event<WPARAM> key_press;
	Event<GUI_Object*> destroy;
	Event<GUI_Object*> get_focus;
	Event<GUI_Object*> lose_focus;

	virtual void render(GraphicalController* Graph,int px,int py);
	virtual void on_mouse_click(MouseEventArgs const& e);
	virtual void on_mouse_down(MouseEventArgs const& e);
	virtual void on_mouse_wheel(MouseEventArgs const& e);
	virtual void on_mouse_move(MouseEventArgs const& e);
	virtual void on_key_press(WPARAM w);
	virtual void on_get_focus(GUI_Object* sender);
	virtual void on_lose_focus(GUI_Object* sender);
	virtual bool check_region(MouseEventArgs const& e);
	virtual void set_focus(bool state);

};

