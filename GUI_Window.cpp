#include "GUI_Window.h"


GUI_Window::GUI_Window(int _x, int _y, int _width, int _height, std::string _Name) :GUI_Container(_x, _y, _width, _height)
{
	m_is_moving = false;
	GUI_Item* Header = new GUI_Item(2, 2, m_size.w- 4, 21, _Name, nullptr);
	Header->close += std::bind(&GUI_Window::on_header_close, this);
	add_managing_control(Header);
	start_moving += std::bind(&GUI_Window::on_start_moving, this, std::placeholders::_1);
	move += std::bind(&GUI_Window::on_move, this, std::placeholders::_1);
	end_moving += std::bind(&GUI_Window::on_ending_move, this, std::placeholders::_1);
	close += std::bind(&GUI_Window::on_close, this, std::placeholders::_1);
}


GUI_Window::~GUI_Window()
{
} 


void GUI_Window::on_mouse_move(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
	for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); Current++)
	{
		if ((*Current)->check_region(LocalMouseEventArgs))
		{
			(*Current)->mouse_move(LocalMouseEventArgs);
			return;
		}
	}
	if (e.flags&MK_LBUTTON)
	{
		if (!m_is_moving)
		{
			start_moving(e);
		}
		move(e);
	}
	else
	{
		if (m_is_moving)
		{
			end_moving(e);
		}
	}
}

void GUI_Window::on_start_moving(MouseEventArgs const& e)
{
	m_is_moving = true;
	m_initial_position.x = m_position.x-e.position.x;
	m_initial_position.y = m_position.y-e.position.y;
	//MessageBox(NULL, "Start", "", MB_OK);
}

void GUI_Window::on_move(MouseEventArgs const& e)
{
	m_position.x = m_initial_position.x + e.position.x;
	m_position.y = m_initial_position.y + e.position.y;
}

void GUI_Window::on_ending_move(MouseEventArgs const& e)
{
	m_is_moving = false;
	//MessageBox(NULL, "End", "", MB_OK);

}

void GUI_Window::on_close(GUI_Object* e)
{
	destroy(this);
}

void GUI_Window::on_header_close()
{
	close(this);
}

void GUI_Window::resize(int _width, int _height)
{
	//width = _width;
	//height = _height;
	//ManagingControls->width = width;
	//ManagingControls->height = height;
	//ItemControls->width = width;
	//ItemControls->height = height;
}