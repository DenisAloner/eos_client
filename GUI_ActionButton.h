#pragma once
#include "GUI_Object.h"
#include "ActionsDescription.h"
#include "Application.h"

class Application;
class TAction;

class GUI_ActionButton :
	public GUI_Object
{
public:

	TAction* m_action;

	GUI_ActionButton();
	~GUI_ActionButton();

	virtual void on_mouse_click(MouseEventArgs const& e);
	virtual void render(GraphicalController* Graph, int px, int py);	
	virtual bool check_region(MouseEventArgs const& e);
};

