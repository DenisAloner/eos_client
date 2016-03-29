#include "GUI_Button_list.h"

GUI_Mainmenu_button::GUI_Mainmenu_button(int x, int y, int width, int height, std::string text, ParameterKind kind) :GUI_button(x, y, width, height, text),m_parameter(new Parameter(kind))
{
}

void GUI_Mainmenu_button::on_mouse_click(MouseEventArgs const& e)
{
	GUI_Object::on_mouse_click(e);
	if (Application::instance().m_message_queue.m_reader)
	{
		Application::instance().m_message_queue.push(m_parameter);
	}
}

GUI_Button_list::GUI_Button_list(int x, int y, int width, int height):GUI_Container(x,y,width,height)
{
}


GUI_Button_list::~GUI_Button_list()
{
}

void GUI_Button_list::add_item_control(GUI_Object* object)
{
	if (!m_items.empty())
	{
		GUI_Object* LastElement = (m_items.back());
		object->m_position.x = 2;
		object->m_position.y = LastElement->m_position.y + LastElement->m_size.h + 2;
		if (object->m_position.y + object->m_size.h>m_size.h)
		{
			m_scroll.y -= object->m_size.h;
		}
	}
	else
	{
		object->m_position.x = 2;
		object->m_position.y = 2;
	}
	GUI_Layer::add(object);
}