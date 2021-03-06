#include "GUI_Item.h"


GUI_Item::GUI_Item(int _x, int _y, int _width, int _height, std::string text, GameTask* tag) :GUI_Container(_x, _y, _width, _height)
{

	m_tag = tag;
	GUI_Text* m_text = new GUI_Text(text);
	m_text->m_position.x = 2;
	m_text->m_position.y = 2;
	add_item_control(m_text);
	GUI_ItemButton* m_button = new GUI_ItemButton();
	m_button->m_position.x = m_size.w - (m_size.h - 4) - 2;
	m_button->m_position.y = 2;
	m_button->m_size.w = m_size.h - 4;
	m_button->m_size.h = m_size.h - 4;
	add_managing_control(m_button);
	m_button->mouse_click += std::bind(&GUI_Item::on_close_button_click, this);
}


GUI_Item::~GUI_Item()
{
}

void GUI_Item::on_close_button_click()
{
	close(this);
}

void GUI_Item::resize(int _width, int _height)
{
	m_size.w = _width;
	m_size.h = _height;
	m_managing_controls->m_size.w = m_size.w;
	m_managing_controls->m_size.h = m_size.h;
	m_item_controls->m_size.w = m_size.w;
	m_item_controls->m_size.h = m_size.h;
	m_managing_controls->m_items.front()->m_position.x = m_size.w - (m_size.h - 4) - 2;
	m_managing_controls->m_items.front()->m_position.y = 2;
	m_managing_controls->m_items.front()->m_size.w = m_size.h - 4;
	m_managing_controls->m_items.front()->m_size.h = m_size.h - 4;

}

//void  GUI_Item::OnMouseClick(MouseEventArgs const& e)
//{
//	MessageBox(NULL, "Yes", "", MB_OK);
//}