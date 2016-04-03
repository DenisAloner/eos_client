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

void GUI_ActionPanel::bind(GUI_connectable_i* object)
{
	m_object = object;
	update_binding();
	object->event_update += std::bind(&GUI_ActionPanel::update_binding, this);
}

void GUI_ActionPanel::update_binding()
{
	while (!m_items.empty())
	{
		remove(m_items.front());
	}
	std::list<Action_helper_t> info;
	m_object->get_actions_list(info);
	for (auto item = info.begin(); item != info.end(); ++item)
	{
		GUI_ActionButton* ActionButton = new GUI_ActionButton();
		ActionButton->m_value = (*item);
		add_item_control(ActionButton);
	}
}