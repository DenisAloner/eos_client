#include "MouseController.h"


MouseController::MouseController()
: m_position()
{
	m_show_cursor = false;
	m_show_pickup_item = false;
	mouse_down += std::bind(&MouseController::on_mouse_down, this, std::placeholders::_1);
	mouse_up += std::bind(&MouseController::on_mouse_up, this, std::placeholders::_1);
	mouse_move += std::bind(&MouseController::on_mouse_move, this, std::placeholders::_1);
	mouse_wheel += std::bind(&MouseController::on_mouse_wheel, this, std::placeholders::_1);
}


MouseController::~MouseController()
{
}

void MouseController::on_mouse_down(const MouseEventArgs &e)
{
}

void MouseController::on_mouse_up(const MouseEventArgs &e)
{
	mouse_click(e);
}

void MouseController::on_mouse_move(const MouseEventArgs &e)
{
	m_position = e.position;
}

void MouseController::on_mouse_wheel(const MouseEventArgs &e)
{
}
