#ifndef GUI_TEXTBOX_H
#define	GUI_TEXTBOX_H

#include "GUI_Container.h"
#include <list>
#include "GUI_Text.h"
#include "Definiton.h"


class GUI_TextBox :
	public GUI_Container
{
public:

	GUI_TextBox();
	~GUI_TextBox();

	//virtual void Render(GraphicalController* Graph);
	virtual void add_item_control(GUI_Object* object);
	virtual void on_mouse_wheel(MouseEventArgs const& e);
	void set_scroll(int dy);

};

#endif //GUI_TEXTBOX_H
