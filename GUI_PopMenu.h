#pragma once
#include <list>
#include "GUI_Object.h"
#include "GUI_Container.h"
#include "GameObject.h"
#include "Application.h"

class GameObject;
class Application;

class GUI_PopMenu :
	public GUI_Container
{
public:

	GUI_PopMenu(void);
	~GUI_PopMenu(void);
	
	virtual void add(std::string Text, GameObject* Object);
	virtual void on_lose_focus(GUI_Object* sender);
	virtual void on_under_cursor(MouseEventArgs const& e);
};

class GUI_PopMenuItem :
	public GUI_Object
{
public:

	std::string m_text;
	GameObject* m_object;

	GUI_PopMenu* m_owner;

	GUI_PopMenuItem(void);
	~GUI_PopMenuItem(void);

	virtual void on_mouse_click(MouseEventArgs const& e);
	virtual void on_under_cursor(MouseEventArgs const& e);
	virtual void on_get_focus(GUI_Object* sender);
	virtual void on_lose_focus(GUI_Object* sender);
	virtual void render(GraphicalController* Graph, int px, int py);
};


