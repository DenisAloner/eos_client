#include "GUI_Window.h"
#include "Application.h"

GUI_Header::GUI_Header(int x, int y, int width, int height, std::u16string text) :GUI_Container(x, y, width, height)
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
	m_size.h = m_items.back()->m_size.h + m_items.back()->m_position.y + 2;
	m_items.front()->m_position.x = m_size.w - (m_size.h - 8) - 4;
	m_items.front()->m_position.y = 4;
	m_items.front()->m_size.w = m_size.h - 8;
	m_items.front()->m_size.h = m_size.h - 8;
}

GUI_Window::GUI_Window(int _x, int _y, int _width, int _height, std::u16string _Name) :GUI_Container(_x, _y, _width, _height)
{
	m_header = new GUI_Header(2, 2, m_size.w - 4, 0, _Name);
	m_header->close += std::bind(&GUI_Window::on_header_close, this);
	GUI_Layer::add(m_header);
	close += std::bind(&GUI_Window::on_close, this, std::placeholders::_1);
	Application::instance().m_window_manager->add_front(this);
}

rectangle_t GUI_Window::client_rect()
{
	return rectangle_t(1, m_header->m_position.y + m_header->m_size.h + 1, m_size.w - 2, m_size.h - (m_header->m_position.y + m_header->m_size.h + 1));
}

GUI_body_window::GUI_body_window(int x, int y, int width, int height, std::u16string Name, GameObject*& object) :GUI_Window(x, y, width, height, Name), m_object(object)
{
	m_item = new GUI_Body(m_object->m_active_state);
	m_item->m_position.x = 1;
	m_item->m_position.y = 1;
	//m_items->resize(m_size.w - 4, m_size.h - 25 - 2);
	add(m_item);
	update_info();
	object->event_update += std::bind(&GUI_body_window::update_info, this);
}

void GUI_body_window::update_info()
{
	m_item->update(m_object->m_active_state);
	resize(m_item->m_size.w+4, m_item->m_position.y + m_item->m_size.h + 2);
}