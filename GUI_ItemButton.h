#pragma once
#include "GUI_Object.h"
class GUI_ItemButton :
	public GUI_Object
{
public:
	GUI_ItemButton();
	~GUI_ItemButton();

	virtual bool check_region(MouseEventArgs const& e);
	virtual void render(GraphicalController* Graph, int px, int py);
	virtual void on_mouse_move(MouseEventArgs const& e);
};

