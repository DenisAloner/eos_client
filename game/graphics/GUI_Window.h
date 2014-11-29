#ifndef GUI_WINDOW_H
#define	GUI_WINDOW_H

#include "GUI_Container.h"
#include "GUI_Item.h"
#include <list>
#include "GUI_TextBox.h"

class GUI_Window :
	public GUI_Container
{
public:

	Event<MouseEventArgs> start_moving;
	Event<MouseEventArgs> move;
	Event<MouseEventArgs> end_moving;
	Event<GUI_Object*> close;

	GUI_Window(int _x, int _y, int _width, int _height, std::string _Name);
	~GUI_Window();

	bool m_is_moving;
	position_t m_initial_position;

	virtual void on_mouse_move(MouseEventArgs const& e);
	virtual void on_start_moving(MouseEventArgs const& e);
	virtual void on_move(MouseEventArgs const& e);
	virtual void on_ending_move(MouseEventArgs const& e);
	virtual void on_close(GUI_Object* e);
	virtual void on_header_close();
	virtual void resize(int _width, int _height);
	virtual void on_mouse_wheel(MouseEventArgs const& e);

};

class GUI_description_window :
	public GUI_Window
{
public:

	GameObject* m_object;
	GUI_TextBox* m_textbox;
	GUI_description_window(int x, int y, int width, int height, std::string Name, GameObject*& object);
	std::list<std::string> m_text;
	void update_info();

};

#endif GUI_WINDOW_H
