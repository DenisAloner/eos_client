#include "GUI_Object.h"


GUI_Object::GUI_Object(void)
{
	focused=false;
	key_press += std::bind(&GUI_Object::on_key_press, this, std::placeholders::_1);
	mouse_click+=std::bind(&GUI_Object::on_mouse_click,this,std::placeholders::_1);
	mouse_down += std::bind(&GUI_Object::on_mouse_down, this, std::placeholders::_1);
	mouse_wheel += std::bind(&GUI_Object::on_mouse_wheel, this, std::placeholders::_1);
	mouse_move += std::bind(&GUI_Object::on_mouse_move, this, std::placeholders::_1);
	get_focus+=std::bind(&GUI_Object::on_get_focus,this,std::placeholders::_1);
	lose_focus+=std::bind(&GUI_Object::on_lose_focus,this,std::placeholders::_1);
}


GUI_Object::~GUI_Object(void)
{
}

void GUI_Object::render(GraphicalController* Graph, int px, int py)
{
}


void GUI_Object::on_mouse_click(MouseEventArgs const& e)
{
	set_focus(true);
}

void GUI_Object::on_mouse_down(MouseEventArgs const& e)
{
	set_focus(true);
}

void GUI_Object::on_mouse_wheel(MouseEventArgs const& e)
{
}

void GUI_Object::on_mouse_move(MouseEventArgs const& e)
{
}

void GUI_Object::on_key_press(WPARAM w)
{
}

void  GUI_Object::set_focus(bool state)
{
	focused=state;
	if(state)
	{
		get_focus(this);
	} else {
		lose_focus(this);
	}
}

bool GUI_Object::check_region(MouseEventArgs const& e)
{
	if (this->m_position.x <= e.position.x&&this->m_position.x + m_size.w >= e.position.x&&this->m_position.y <= e.position.y&&this->m_position.y + m_size.h >= e.position.y)
	{
		return true;
	}
	return false;
}

void GUI_Object::on_get_focus(GUI_Object* sender)
{
}

void GUI_Object::on_lose_focus(GUI_Object* sender)
{
}
