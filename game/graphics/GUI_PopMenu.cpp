#include "game/graphics/GUI_PopMenu.h"
#include "game/Parameter.h"

void Select_object_popmenu::on_item_click(Item* sender)
{

	if (Application::instance().m_message_queue.m_reader)
	{
		P_object* p = new P_object();
		p->m_object = sender->m_object;
		Application::instance().m_message_queue.push(p);
	}
	set_focus(false);
	destroy(this);
}
