#include "GUI_Action_bar.h"

GUI_Action_bar::GUI_Action_bar(int x, int y, int width, int height) : GUI_Scrollable_container(x, y, width, height)
{
}

GUI_Action_bar::~GUI_Action_bar()
{
}

void GUI_Action_bar::bind(GUI_connectable_i* object)
{
	m_object = object;
	update_binding();
	object->event_update += std::bind(&GUI_Action_bar::update_binding, this);
}

void GUI_Action_bar::update_binding()
{
	while (!m_items.empty())
	{
		remove(m_items.front());
	}
	std::list<Action_helper_t> info;
	m_object->get_actions_list(info);
	for (auto item = info.begin(); item != info.end(); ++item)
	{
		GUI_ActionButton* ActionButton = new GUI_ActionButton(m_size.w - 4, 40);
		ActionButton->m_value = (*item);
		add_item_control(ActionButton);
	}
}