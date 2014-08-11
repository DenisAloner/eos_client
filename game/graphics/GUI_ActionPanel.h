#ifndef GUI_ACTIONPANEL_H
#define	GUI_ACTIONPANEL_H

#include "GUI_Container.h"

class GUI_ActionPanel :
	public GUI_Container
{
public:

	GUI_ActionPanel(int _x, int _y, int _width, int _height);
	~GUI_ActionPanel();

	virtual void add_item_control(GUI_Object* object);
};

#endif //GUI_ACTIONPANEL_H
