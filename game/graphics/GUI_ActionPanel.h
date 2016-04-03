#ifndef GUI_ACTIONPANEL_H
#define	GUI_ACTIONPANEL_H

#include "GUI_Container.h"
#include "GUI_ActionButton.h"

class GUI_ActionPanel :
	public GUI_Container
{
public:

	GUI_ActionPanel(int _x, int _y, int _width, int _height);
	~GUI_ActionPanel();

	virtual void add_item_control(GUI_Object* object);
	virtual void bind(GUI_connectable_i* m_object);
	void update_binding();

private:
	GUI_connectable_i* m_object;
};

#endif //GUI_ACTIONPANEL_H
