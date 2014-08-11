#include "MouseController.h"


MouseController::MouseController()
{
	m_show_cursor = false;
	m_show_pickup_item = false;
	mouse_down += std::bind(&MouseController::on_mouse_down, this, std::placeholders::_1);
	mouse_up += std::bind(&MouseController::on_mouse_up, this, std::placeholders::_1);
	mouse_move += std::bind(&MouseController::on_mouse_move, this, std::placeholders::_1);
	//MouseWheel += std::bind(&MouseController::OnMouseWheel, this, std::placeholders::_1);
	m_position = get_mouse_position();
}


MouseController::~MouseController()
{
}

void MouseController::on_mouse_down(MouseEventArgs const& e)
{
}

void MouseController::on_mouse_up(MouseEventArgs const& e)
{
	mouse_click(e);
}

void MouseController::on_mouse_move(MouseEventArgs const& e)
{
	m_position = get_mouse_position();
}

position_t MouseController::get_mouse_position()
{
	return position_t(0, 0);
}

MouseController_Windows::MouseController_Windows(HWND _hWnd) :hWnd(_hWnd)
{
}

position_t MouseController_Windows::get_mouse_position()
{
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);
	GLfloat winX, winY;
	winX = static_cast<GLfloat>(mouse.x);
	winY = static_cast<GLfloat>(mouse.y);
	return Application::instance().m_graph->get_OpenGL_position(winX, winY);
}