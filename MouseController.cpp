#include "MouseController.h"


MouseController::MouseController()
{
	ShowCursor = false;
	ShowPickUpItem = false;
	MouseDownStatus = false;
	MouseDown += std::bind(&MouseController::OnMouseDown, this, std::placeholders::_1);
	MouseUp += std::bind(&MouseController::OnMouseUp, this, std::placeholders::_1);
	MouseMove += std::bind(&MouseController::OnMouseMove, this, std::placeholders::_1);
	//MouseWheel += std::bind(&MouseController::OnMouseWheel, this, std::placeholders::_1);
	Position = GetMouseCoordinat();
}


MouseController::~MouseController()
{
}

void MouseController::OnMouseDown(MouseEventArgs const& e)
{
}

void MouseController::OnMouseUp(MouseEventArgs const& e)
{
	MouseClick(e);
}

void MouseController::OnMouseMove(MouseEventArgs const& e)
{
	Position = GetMouseCoordinat();
}

Point MouseController::GetMouseCoordinat()
{
	return Point(0, 0);
}

MouseController_Windows::MouseController_Windows(HWND _hWnd) :hWnd(_hWnd)
{
}

Point MouseController_Windows::GetMouseCoordinat()
{
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);
	GLfloat winX, winY, winZ;
	winX = (float)mouse.x;
	winY = (float)mouse.y;
	return Application::Instance().Graph->GetOGLPos(winX, winY);
}