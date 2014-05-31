#pragma once
#include "GraphicalController.h"
#include "Event.h"
#include "GUI_Object.h"
#include <list>

class GraphicalController;

class GUI_Layer:
	public GUI_Object
{
public:

	std::list<GUI_Object*> Items;
	GUI_Object* focus;

	GUI_Layer(void);
	~GUI_Layer(void);

	virtual void RenderAt(GraphicalController* Graph, int px, int py);
	virtual void OnMouseClick(MouseEventArgs const& e);
	virtual void OnMouseDown(MouseEventArgs const& e);
	virtual void OnMouseWheel(MouseEventArgs const& e);
	virtual void OnMouseMove(MouseEventArgs const& e);
	virtual void OnKeyPress(WPARAM w);
	virtual void SetFocus(bool state);

	virtual void OnUnderCursor(MouseEventArgs const& e);

	virtual void add(GUI_Object* object);
	virtual void AddFront(GUI_Object* object);
	virtual void remove(GUI_Object* object);
	
	virtual bool CheckRegion(MouseEventArgs const& e);
	virtual void OnGetFocus(GUI_Object* sender);
	virtual void OnItemGetFocus(GUI_Object* sender);
	virtual MouseEventArgs SetLocalMousePosition(MouseEventArgs const& source);
};

