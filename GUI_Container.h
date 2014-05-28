#pragma once
#include "GUI_Layer.h"

class GUI_Container :
	public GUI_Layer
{
public:

	GUI_Container(int _x, int _y, int _width, int _height);
	~GUI_Container();

	GUI_Layer* ManagingControls;
	GUI_Layer* ItemControls;

	Point Scroll = Point(0, 0);

	virtual bool CheckRegion(MouseEventArgs const& e);
	virtual void OnMouseClick(MouseEventArgs const& e);
	virtual void RenderAt(GraphicalController* Graph, int px, int py);
	virtual void OnMouseWheel(MouseEventArgs const& e);
	void SetScroll(int dy);
	virtual MouseEventArgs SetLocalMousePosition(MouseEventArgs const& source);
	virtual void OnMouseDown(MouseEventArgs const& e);
	virtual void OnMouseMove(MouseEventArgs const& e);
	virtual void AddItemControl(GUI_Object* object);
	virtual void RemoveItemControl(GUI_Object* object);
	virtual void AddManagingControl(GUI_Object* object);
	virtual void Resize(int _width, int _height);
};

