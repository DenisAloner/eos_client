#pragma once
#include "Event.h"
#include "GraphicalController.h"
#include "Definiton.h"
#include <string>

class GraphicalController;

class GUI_Object
{
public:

	std::string Name;

	int x;
	int y;
	int width;
	int height;

	bool focused;

	GUI_Object(void);
	~GUI_Object(void);

	Event<MouseEventArgs> MouseClick;
	Event<MouseEventArgs> MouseDown;
	Event<MouseEventArgs> MouseWheel;
	Event<MouseEventArgs> MouseMove;
	Event<WPARAM> KeyPress;
	Event<MouseEventArgs> UnderCursor;
	Event<GUI_Object*> Destroy;
	Event<GUI_Object*> GetFocus;
	Event<GUI_Object*> LoseFocus;

	virtual void RenderAt(GraphicalController* Graph,int px,int py);
	virtual void OnMouseClick(MouseEventArgs const& e);
	virtual void OnMouseDown(MouseEventArgs const& e);
	virtual void OnMouseWheel(MouseEventArgs const& e);
	virtual void OnMouseMove(MouseEventArgs const& e);
	virtual void OnKeyPress(WPARAM w);
	virtual void SetFocus(bool state);
	virtual void OnUnderCursor(MouseEventArgs const& e);
	virtual bool CheckRegion(MouseEventArgs const& e);
	virtual void OnGetFocus(GUI_Object* sender);
	virtual void OnLoseFocus(GUI_Object* sender);

};

