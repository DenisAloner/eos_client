#pragma once
#include "GUI_Container.h"
#include "GUI_button.h"
#include "Parameter.h"

class GUI_Mainmenu_button :
	public GUI_button

{
	public:
	Parameter* m_parameter;
	GUI_Mainmenu_button(int x, int y, int width, int height, std::string text, ParameterKind kind);

	virtual void on_mouse_click(MouseEventArgs const& e);
};


class GUI_Button_list :
	public GUI_Container
{
public:

	GUI_Button_list(int x, int y, int width, int height);
	~GUI_Button_list();

	virtual void add_item_control(GUI_Object* object);

};

