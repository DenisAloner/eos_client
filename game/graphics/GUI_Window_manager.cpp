#include "GUI_Window_manager.h"
#include "utils/log.h"


GUI_Window_manager::GUI_Window_manager()
{
}


GUI_Window_manager::~GUI_Window_manager()
{
}

void GUI_Window_manager::on_item_get_focus(GUI_Object* sender)
{
	if (m_focus != sender)
	{
		set_focus(true);
		m_focus = sender;
		m_items.remove(sender);
		m_items.push_front(sender);
	}
}