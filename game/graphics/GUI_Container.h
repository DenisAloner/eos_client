#ifndef GUI_CONTAINER_H
#define	GUI_CONTAINER_H

#include "GUI_Layer.h"

class GUI_Container :
	public GUI_Layer
{
public:

	GUI_Container(int _x, int _y, int _width, int _height);
	~GUI_Container();

	position_t m_scroll;

	virtual bool check_region(MouseEventArgs const& e);
	virtual void on_mouse_click(MouseEventArgs const& e);
	virtual void render(GraphicalController* Graph, int px, int py);
	virtual void on_mouse_wheel(MouseEventArgs const& e);
	virtual void on_mouse_down(MouseEventArgs const& e);
	virtual void on_mouse_move(MouseEventArgs const& e);
	virtual void resize(int _width, int _height);
	virtual void set_scroll(int dy);
	virtual MouseEventArgs set_local_mouse_position(MouseEventArgs const& source);
};

#endif //GUI_CONTAINER_H
