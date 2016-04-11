#include "GUI_Window.h"
#include "Application.h"

GUI_Header::GUI_Header(int x, int y, int width, int height, std::string text) :GUI_Container(x, y, width, height)
{
	GUI_Text* m_text = new GUI_Text(text);
	m_text->m_position.x = 2;
	m_text->m_position.y = 2;
	m_size.h = m_text->m_size.h + m_text->m_position.y + 2;
	GUI_Layer::add(m_text);
	GUI_ItemButton* m_button = new GUI_ItemButton();
	m_button->m_position.x = m_size.w - (m_size.h - 8) - 4;
	m_button->m_position.y = 4;
	m_button->m_size.w = m_size.h - 8;
	m_button->m_size.h = m_size.h - 8;
	GUI_Layer::add_front(m_button);
	m_button->mouse_click += std::bind(&GUI_Header::on_close_button_click, this);
}

GUI_Header::~GUI_Header()
{
}

void GUI_Header::on_close_button_click()
{
	close(this);
}

void GUI_Header::resize(int width, int height)
{
	m_size.w = width;
	m_size.h = height;
	m_items.front()->m_position.x = m_size.w - (m_size.h - 4) - 2;
	m_items.front()->m_position.y = 2;
	m_items.front()->m_size.w = m_size.h - 4;
	m_items.front()->m_size.h = m_size.h - 4;

}

GUI_Window::GUI_Window(int _x, int _y, int _width, int _height, std::string _Name) :GUI_Container(_x, _y, _width, _height)
{
	m_is_moving = false;
	m_header = new GUI_Header(2, 2, m_size.w - 4, 21, _Name);
	m_header->close += std::bind(&GUI_Window::on_header_close, this);
	GUI_Layer::add(m_header);
	start_moving += std::bind(&GUI_Window::on_start_moving, this, std::placeholders::_1);
	move += std::bind(&GUI_Window::on_move, this, std::placeholders::_1);
	end_moving += std::bind(&GUI_Window::on_ending_move, this, std::placeholders::_1);
	close += std::bind(&GUI_Window::on_close, this, std::placeholders::_1);
	Application::instance().m_window_manager->add_front(this);
}

void GUI_Window::add(GUI_Object* object)
{
	object->m_position = position_t(object->m_position.x, object->m_position.y + m_header->m_position.y + m_header->m_size.h);
	GUI_Layer::add(object);
}


GUI_Window::~GUI_Window()
{
}

//void GUI_Window::on_mouse_down(MouseEventArgs const& e)
//{
//	MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
//	if (m_items.front()->check_region(LocalMouseEventArgs))
//	{
//		m_focus = nullptr;
//		set_focus(true);
//		return;
//	}
//	else {
//		for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); Current++)
//		{
//			if ((*Current)->check_region(LocalMouseEventArgs))
//			{
//				(*Current)->mouse_down(LocalMouseEventArgs);
//				return;
//			}
//		}
//		if ((m_focus != this) && (m_focus == nullptr))
//		{
//			set_focus(true);
//		}
//	}
//}

void GUI_Window::on_mouse_move(MouseEventArgs const& e)
{
	if (m_is_moving)
	{
		if (e.key == mk_left)
		{
			move(e);
		}
		else
		{
			end_moving(e);
		}
	}
	else {
		MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
		for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); Current++)
		{
			if ((*Current)->check_region(LocalMouseEventArgs))
			{
				(*Current)->mouse_move(LocalMouseEventArgs);
				if ((*Current) == m_items.front())
				{
					if (e.key == mk_left)
					{
						start_moving(e);
						move(e);
					}
				}
				return;
			}
		}
	}
}

void GUI_Window::on_start_moving(MouseEventArgs const& e)
{
	m_is_moving = true;
	m_initial_position.x = m_position.x - e.position.x;
	m_initial_position.y = m_position.y - e.position.y;
}

void GUI_Window::on_move(MouseEventArgs const& e)
{
	m_position.x = m_initial_position.x + e.position.x;
	m_position.y = m_initial_position.y + e.position.y;
}

void GUI_Window::on_ending_move(MouseEventArgs const& e)
{
	m_is_moving = false;
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

void GUI_Window::on_mouse_wheel(MouseEventArgs const& e)
{
	if (m_focus)
	{
		m_focus->mouse_wheel(e);
	}
}

GUI_description_window::GUI_description_window(int x, int y, int width, int height, std::string Name, GameObject*& object) :GUI_Window(x, y, width, height, Name), m_object(object)
{
	m_textbox = new GUI_TextBox();
	m_textbox->m_position.x = 2;
	m_textbox->m_position.y = 2;
	//m_item->resize(m_size.w - 4, m_size.h - 25 - 2);
	add(m_textbox);
	m_textbox->resize(m_size.w - 4, m_size.h - m_textbox->m_position.y - 2);
	GUI_Layer::add(m_textbox);
	update_info();
	object->event_update += std::bind(&GUI_description_window::update_info, this);
}

void GUI_description_window::update_info()
{
	m_text.clear();
	while (m_textbox->m_items.begin()!= m_textbox->m_items.end())
	{
		m_textbox->GUI_Layer::remove((*m_textbox->m_items.begin()));
	}
	if (m_object->m_active_state)
	{
		m_text.push_back("эффекты:");
		for (auto current = m_object->m_active_state->m_item.begin(); current != m_object->m_active_state->m_item.end(); current++)
		{
			m_text.push_back("." + Application::instance().m_game_object_manager->get_effect_string(current->first) + ":");
			current->second->description(&m_text,2);
		}
	}
	for (auto item = m_text.begin(); item != m_text.end(); item++)
	{
		m_textbox->add_item_control(new GUI_Text((*item), new GUI_TextFormat(8, 17, RGBA_t(0.7, 0.9, 1.0, 1.0))));
	}
}

GUI_body_window::GUI_body_window(int x, int y, int width, int height, std::string Name, GameObject*& object) :GUI_Window(x, y, width, height, Name), m_object(object)
{
	m_item = new GUI_Body(m_object->m_active_state);
	m_item->m_position.x = 2;
	m_item->m_position.y = 2;
	//m_item->resize(m_size.w - 4, m_size.h - 25 - 2);
	add(m_item);
	update_info();
	object->event_update += std::bind(&GUI_body_window::update_info, this);
}

void GUI_body_window::update_info()
{
	m_item->update(m_object->m_active_state);
}