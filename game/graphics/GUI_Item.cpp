#include "GUI_Item.h"
#include "GUI_Text.h"

GUI_Item::GUI_Item(int x, int y, int width, int height, std::u16string text, tag_t tag)
    : GUI_Container(x, y, width, height)
{

    m_tag = tag;
    const auto m_text = new GUI_Text(text);
    m_text->m_position.x = 2;
    m_text->m_position.y = 2;
    m_size.h = m_text->m_size.h + m_text->m_position.y + 2;
    GUI_Layer::add(m_text);
    auto m_button = new GUI_ItemButton();
    m_button->m_position.x = m_size.w - (m_size.h - 4) - 2;
    m_button->m_position.y = 2;
    m_button->m_size.w = m_size.h - 4;
    m_button->m_size.h = m_size.h - 4;
    GUI_Layer::add_front(m_button);
    m_button->mouse_click += std::bind(&GUI_Item::on_close_button_click, this);
}

GUI_Item::~GUI_Item()
{
}

void GUI_Item::on_close_button_click()
{
    close(this);
}

void GUI_Item::resize(int width, int height)
{
    m_size.w = width;
    //m_size.h = m_text->m_size.h + m_text->m_position.y + 2;
    const auto front = m_items.front();
    front->m_position.x = m_size.w - (m_size.h - 4) - 2;
    front->m_position.y = 2;
    front->m_size.w = m_size.h - 4;
    front->m_size.h = m_size.h - 4;
}

//void  GUI_Item::OnMouseClick(MouseEventArgs const& e)
//{
//	MessageBox(NULL, "Yes", "", MB_OK);
//}