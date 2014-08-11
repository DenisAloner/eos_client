#ifndef GUI_ACTIONBUTTON_H
#define	GUI_ACTIONBUTTON_H

#include "GUI_Object.h"
#include "Action.h"
#include "Application.h"

class Application;
class Action;

class GUI_ActionButton :
	public GUI_Object
{
public:

	Action* m_action;

	GUI_ActionButton();
	~GUI_ActionButton();

	virtual void on_mouse_click(MouseEventArgs const& e);
	virtual void render(GraphicalController* Graph, int px, int py);	
	virtual bool check_region(MouseEventArgs const& e);
};

#endif //GUI_ACTIONBUTTON_H
