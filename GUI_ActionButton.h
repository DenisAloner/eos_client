#pragma once
#include "GUI_Object.h"
#include "ActionsDescription.h"
#include "Application.h"

class Application;
class TAction;

class GUI_ActionButton :
	public GUI_Object
{
public:

	TAction* Action;
	GUI_ActionButton();
	~GUI_ActionButton();

	virtual void OnMouseClick(MouseEventArgs const& e);
	virtual void RenderAt(GraphicalController* Graph, int px, int py);	
	virtual bool CheckRegion(MouseEventArgs const& e);
};

