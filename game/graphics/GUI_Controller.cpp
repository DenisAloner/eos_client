#include "GUI_controller.h"
#include <ApplicationGUI.h>

GUI_Controller::GUI_Controller():m_dragged(false),m_GUI(nullptr)
{
}


GUI_Controller::~GUI_Controller()
{
}

void GUI_Controller::key_press(WPARAM w)
{
	if (m_GUI)
	{
		LOG(INFO) << "yes";
		m_GUI->key_press(w);
	}
}

void GUI_Controller::mouse_click(MouseEventArgs const& e)
{
	if (m_GUI)
	{
		if (m_dragged&&e.key == mk_left)
		{
			m_dragged = false;
			m_GUI->mouse_end_drag(e);
		}
		else
		{
			m_GUI->mouse_click(e);
		}
	}
}

void GUI_Controller::mouse_down(MouseEventArgs const& e)
{
	if (m_GUI) { m_GUI->mouse_down(e); }
}

void GUI_Controller::mouse_move(MouseEventArgs const& e)
{
	if (m_GUI)
	{
		if (m_dragged)
		{
			m_GUI->mouse_drag(e);
		}
		else
		{
			if(e.key == mk_left)
			{
				m_dragged = true;
				m_GUI->mouse_start_drag(e);
			}
			else
			{
				m_GUI->mouse_move(e);
			}
		}
	}
}

void GUI_Controller::mouse_wheel(MouseEventArgs const& e)
{
	if (m_GUI) { m_GUI->mouse_wheel(e); }
}