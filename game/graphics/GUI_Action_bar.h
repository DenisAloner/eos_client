#ifndef GUI_ACTION_BAR_H
#define	GUI_ACTION_BAR_H

#include "GUI_Container.h"
#include "GUI_ActionButton.h"

class GUI_Action_bar :
	public GUI_Scrollable_container
{
public:

	GUI_Action_bar(int x, int y, int width, int height);
	~GUI_Action_bar();

	/*virtual void add_item_control(GUI_Object* object);*/
	virtual void bind(GUI_connectable_i* m_object);
	void update_binding();

private:
	GUI_connectable_i* m_object;
};

#endif //GUI_ACTION_BAR_H