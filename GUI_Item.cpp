#include "GUI_Item.h"


GUI_Item::GUI_Item(int _x, int _y, int _width, int _height, std::string text, GameTask* tag) :GUI_Container(_x, _y, _width, _height)
{

	m_tag = tag;
	GUI_Text* m_text = new GUI_Text(text);
	m_text->m_position.x = 2;
	m_text->m_position.y = 2;
	add_item_control(m_text);
	GUI_ItemButton* m_button = new GUI_ItemButton();
	m_button->m_position.x = m_size.x - (m_size.y - 4) - 2;
	m_button->m_position.y = 2;
	m_button->m_size.x = m_size.y - 4;
	m_button->m_size.y = m_size.y - 4;
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
	m_size.x = _width;
	m_size.y = _height;
	m_managing_controls->m_size.x = m_size.x;
	m_managing_controls->m_size.y = m_size.y;
	m_item_controls->m_size.x = m_size.x;
	m_item_controls->m_size.y = m_size.y;
	m_managing_controls->m_items.front()->m_position.x = m_size.x - (m_size.y - 4) - 2;
	m_managing_controls->m_items.front()->m_position.y = 2;
	m_managing_controls->m_items.front()->m_size.x = m_size.y - 4;
	m_managing_controls->m_items.front()->m_size.y = m_size.y - 4;

}

//void  GUI_Item::OnMouseClick(MouseEventArgs const& e)
//{
//	MessageBox(NULL, "Yes", "", MB_OK);
//}