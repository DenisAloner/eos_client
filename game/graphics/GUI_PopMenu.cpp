#include "game/graphics/GUI_PopMenu.h"
#include "game/Parameter.h"

void Select_object_popmenu::on_item_click(Item* sender)
{

	if (Application::instance().m_message_queue.m_reader)
	{
		Parameter* p = new Parameter(parameter_type_e::parameter_kind_object);
		(*p)[0].set(sender->m_object);
		Application::instance().m_message_queue.push(p);
	}
	set_focus(false);
	destroy(this);
}
