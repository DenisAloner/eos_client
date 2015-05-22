#include "GUI_ActionPanel.h"


GUI_ActionPanel::GUI_ActionPanel(int _x, int _y, int _width, int _height) :GUI_Container(_x, _y, _width, _height)
{
}


GUI_ActionPanel::~GUI_ActionPanel()
{
}

void GUI_ActionPanel::add_item_control(GUI_Object* object)
{
	if (!m_items.empty())
	{
		GUI_Object* Back = m_items.back();
		object->m_position.y = 4;
		object->m_position.x = Back->m_position.x + Back->m_size.w + 4;
		if (object->m_position.x + object->m_size.w>m_size.w)
		{
			m_scroll.x -= object->m_size.w;
		}
	}
	else
	{
		object->m_position.x = 4;
		object->m_position.y = 4;
	}
	GUI_Layer::add(object);
}