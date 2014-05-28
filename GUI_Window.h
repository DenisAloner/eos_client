#pragma once
#include "GUI_Container.h"
#include "GUI_Item.h"
#include <list>

class GUI_Window :
	public GUI_Container
{
public:

	Event<MouseEventArgs> StartMoving;
	Event<MouseEventArgs> Move;
	Event<MouseEventArgs> EndMoving;
	Event<GUI_Object*> Close;

	GUI_Window(int _x, int _y, int _width, int _height, std::string _Name);
	~GUI_Window();

	bool IsMoving;
	Point InitialPosition= Point(0, 0);

	virtual void OnMouseMove(MouseEventArgs const& e);
	virtual void OnStartMoving(MouseEventArgs const& e);
	virtual void OnMove(MouseEventArgs const& e);
	virtual void OnEndMoving(MouseEventArgs const& e);
	virtual void OnClose(GUI_Object* e);
	virtual void OnHeaderClose();
	virtual void Resize(int _width, int _height);

};

