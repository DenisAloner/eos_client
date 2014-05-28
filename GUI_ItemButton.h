#pragma once
#include "GUI_Object.h"
class GUI_ItemButton :
	public GUI_Object
{
public:
	GUI_ItemButton();
	~GUI_ItemButton();

	virtual bool CheckRegion(MouseEventArgs const& e);
	virtual void RenderAt(GraphicalController* Graph, int px, int py);
	virtual void OnMouseMove(MouseEventArgs const& e);
};

