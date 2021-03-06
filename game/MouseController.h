#ifndef MOUSECONTROLLER_H
#define	MOUSECONTROLLER_H

#include <chrono>
#include "Event.h"
#include "Definiton.h"
#include "Timer.h"
#include "Application.h"

class MouseController
{
public:
	GLuint m_cursor; 
	GLuint m_pickup_item;

	bool m_show_cursor;
	bool m_show_pickup_item;
	
	Event<MouseEventArgs> mouse_down;
	Event<MouseEventArgs> mouse_up;
	Event<MouseEventArgs> mouse_move;
	Event<MouseEventArgs> mouse_click;
	Event<MouseEventArgs> mouse_wheel;

	MouseController();
	~MouseController();

	void on_mouse_down(const MouseEventArgs &e);
	void on_mouse_up(const MouseEventArgs &e);
	void on_mouse_move(const MouseEventArgs &e);
	void on_mouse_wheel(const MouseEventArgs &e);

	inline position_t get_mouse_position() { return m_position; }

private:
	position_t m_position;		///< ������� ������� ��������� ������������ ���� � ������ ������� �������
};

#endif //MOUSECONTROLLER_H
