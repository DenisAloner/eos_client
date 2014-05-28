#pragma once
#include "Event.h"
#include "Definiton.h"
#include "Timer.h"
#include <thread>
#include <chrono>
#include "glew.h"
#include "Application.h"

class MouseController
{
public:

	GLuint Cursor; 
	GLuint PickUpItem;

	Point Position = Point(0, 0);

	bool ShowCursor;
	bool ShowPickUpItem;
	bool MouseDownStatus;
	
	Event<MouseEventArgs> MouseDown;
	Event<MouseEventArgs> MouseUp;
	Event<MouseEventArgs> MouseMove;
	Event<MouseEventArgs> MouseClick;
	Event<MouseEventArgs> MouseWheel;

	MouseController();
	~MouseController();

	void OnMouseDown(MouseEventArgs const& e);
	void OnMouseUp(MouseEventArgs const& e);
	void OnMouseMove(MouseEventArgs const& e);
	//void OnMouseWheel(MouseEventArgs const& e);

	virtual Point GetMouseCoordinat();

};

class MouseController_Windows :
	public MouseController
{
public:

	HWND hWnd;

	MouseController_Windows(HWND _hWnd);
	virtual Point GetMouseCoordinat();

};