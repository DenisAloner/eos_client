#ifndef GUI_CONTAINER_H
#define	GUI_CONTAINER_H

#include "GUI_Layer.h"

class GUI_Container :
	public GUI_Layer
{
public:

	GUI_Container(int x, int y, int width, int height, bool border_visible = true);
	~GUI_Container();

	bool m_border_visible;
	position_t m_scroll;

	bool check_region(MouseEventArgs const& e) override;
	void on_mouse_click(MouseEventArgs const& e) override;
	void render(GraphicalController* Graph, int px, int py) override;
	void on_mouse_wheel(MouseEventArgs const& e) override;
	void on_mouse_down(MouseEventArgs const& e) override;
	void on_mouse_move(MouseEventArgs const& e) override;
	void resize(int _width, int _height) override;
	virtual void set_scroll(int dy);
	MouseEventArgs set_local_mouse_position(MouseEventArgs const& source) override;
};

class GUI_Scrollable_container;

class GUI_Scrollbar_vertical :
	public GUI_Object
{
public:

	GUI_Scrollable_container* m_owner;

	int m_bar_top;
	int m_bar_height;
	int m_pos;

	GUI_Scrollbar_vertical(GUI_Scrollable_container* owner);
	void render(GraphicalController* Graph, int px, int py) override;
	void content_update();
	MouseEventArgs set_local_mouse_control(MouseEventArgs const& source);
	void on_mouse_start_drag(MouseEventArgs const& e) override;
	void on_mouse_drag(MouseEventArgs const& e) override;
	bool check_region(MouseEventArgs const& e) override;

};

class GUI_Scrollable_container :
	public GUI_Container
{

public:
	GUI_Scrollbar_vertical m_scrollbar;

	std::list<GUI_Object*>::iterator start_render;
	std::list<GUI_Object*>::iterator end_render;

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
	void update();
	void render(GraphicalController* Graph, int px, int py) override;
	void resize(int width, int height) override;

};

#endif //GUI_CONTAINER_H
