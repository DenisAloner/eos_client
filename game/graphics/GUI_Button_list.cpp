#include "GUI_Button_list.h"

GUI_Button_list::GUI_Button_list(int x, int y, int width, int height)
    : GUI_Container(x, y, width, height)
{
}

GUI_Button_list::~GUI_Button_list()
{
}

void GUI_Button_list::add_item_control(GUI_Object* object)
{
    if (!m_items.empty()) {
	    const auto last_element = (m_items.back());
        object->m_position.x = 2;
        object->m_position.y = last_element->m_position.y + last_element->m_size.h + 2;
        if (object->m_position.y + object->m_size.h > m_size.h) {
            m_scroll.y -= object->m_size.h;
        }
    } else {
        object->m_position.x = 2;
        object->m_position.y = 2;
    }
    GUI_Layer::add(object);
}