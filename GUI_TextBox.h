#pragma once
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
	virtual void AddItemControl(GUI_Object* object);

	virtual void OnMouseWheel(MouseEventArgs const& e);
	void SetScroll(int dy);

};

