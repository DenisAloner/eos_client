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

	Application* Game;
	
	virtual void add(std::string Text, GameObject* Object);
	virtual void OnLoseFocus(GUI_Object* sender);
	virtual void OnUnderCursor(MouseEventArgs const& e);
};

class GUI_PopMenuItem :
	public GUI_Object
{
public:

	std::string Text;
	GameObject* Object;

	GUI_PopMenu* Owner;

	GUI_PopMenuItem(void);
	~GUI_PopMenuItem(void);

	virtual void OnMouseClick(MouseEventArgs const& e);
	virtual void OnUnderCursor(MouseEventArgs const& e);
	virtual void OnGetFocus(GUI_Object* sender);
	virtual void OnLoseFocus(GUI_Object* sender);
	virtual void RenderAt(GraphicalController* Graph, int px, int py);
};


