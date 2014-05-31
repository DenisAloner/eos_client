#pragma once
#include "Event.h"
#include "Definiton.h"
#include "Timer.h"
#include <thread>
#include <chrono>
#include "glew.h"
#include "Application.h"

class MouseController
{
public:

	GLuint m_cursor; 
	GLuint m_pickup_item;

	GPosition m_position;

	bool m_show_cursor;
	bool m_show_pickup_item;
	
	Event<MouseEventArgs> mouse_down;
	Event<MouseEventArgs> mouse_up;
	Event<MouseEventArgs> mouse_move;
	Event<MouseEventArgs> mouse_click;
	Event<MouseEventArgs> mouse_wheel;

	MouseController();
	~MouseController();

	void on_mouse_down(MouseEventArgs const& e);
	void on_mouse_up(MouseEventArgs const& e);
	void on_mouse_move(MouseEventArgs const& e);
	//void OnMouseWheel(MouseEventArgs const& e);

	virtual GPosition get_mouse_position();

};

class MouseController_Windows :
	public MouseController
{
public:

	HWND hWnd;

	MouseController_Windows(HWND _hWnd);
	virtual GPosition get_mouse_position();

};