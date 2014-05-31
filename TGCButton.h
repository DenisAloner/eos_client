#pragma once
#include "GUI_Object.h"
class TGCButton :
	public GUI_Object
{
public:
	
	char* Text;

	TGCButton(void);
	~TGCButton(void);

	virtual void Render(GraphicalController* Graph);
	virtual void OnMouseClick(MouseEventArgs const& e);
};

