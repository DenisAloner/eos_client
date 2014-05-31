#pragma once
#include "GUI_Object.h"
class TGCTextBox :
	public GUI_Object
{
public:
	TGCTextBox(void);
	~TGCTextBox(void);

	char* Text;

	virtual void Render(GraphicalController* Graph);

	virtual void OnKeyPress(WPARAM w);
	virtual void OnMouseClick(MouseEventArgs const& e);
};

