#pragma once
#include "GUI_Container.h"

class GUI_ActionPanel :
	public GUI_Container
{
public:
	GUI_ActionPanel(int _x, int _y, int _width, int _height);
	~GUI_ActionPanel();

	virtual void AddItemControl(GUI_Object* object);
	virtual void OnMouseClick(MouseEventArgs const& e);
};

