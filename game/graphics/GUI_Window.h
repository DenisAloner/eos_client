#ifndef GUI_WINDOW_H
#define	GUI_WINDOW_H

#include "GUI_Container.h"
#include "GUI_Item.h"
#include <list>
#include "GUI_TextBox.h"

class GUI_Header :
	public GUI_Container
{
public:

	Event<GUI_Object*> close;
	GUI_Header(int x, int y, int width, int height, std::string text);
	~GUI_Header();

	void on_close_button_click();
	virtual void resize(int width, int height);

};

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

	//virtual void on_mouse_down(MouseEventArgs const& e);
	virtual void on_mouse_move(MouseEventArgs const& e);
	virtual void on_start_moving(MouseEventArgs const& e);
	virtual void on_move(MouseEventArgs const& e);
	virtual void on_ending_move(MouseEventArgs const& e);
	virtual void on_close(GUI_Object* e);
	virtual void on_header_close();
	virtual void resize(int _width, int _height);
	virtual void on_mouse_wheel(MouseEventArgs const& e);
	virtual void add(GUI_Object* object);
	virtual rectangle_t client_rect();

private:
	GUI_Header* m_header;

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

class GUI_Body;

class GUI_body_window :
	public GUI_Window
{
public:

	GameObject* m_object;
	GUI_Body* m_item;
	GUI_body_window(int x, int y, int width, int height, std::string Name, GameObject*& object);
	std::list<std::string> m_text;
	void update_info();

};

#endif GUI_WINDOW_H
