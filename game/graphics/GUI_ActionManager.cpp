#include "GUI_ActionManager.h"
#include "Action.h"


GUI_ActionManager::GUI_ActionManager(int x, int y, int width, int height,ActionManager* ActionManager) : GUI_Scrollable_container(x, y, width, height)
{
	m_item = ActionManager;
	m_item->add_item_event += std::bind(&GUI_ActionManager::on_item_add, this, std::placeholders::_1);
	m_item->remove_item_event += std::bind(&GUI_ActionManager::on_item_remove, this, std::placeholders::_1);
}


GUI_ActionManager::~GUI_ActionManager(void)
{
}

void GUI_ActionManager::add_item_control(GUI_Object* object)
{
	static_cast<GUI_Item*>(object)->resize(m_size.w - 4-20, 21);
	static_cast<GUI_Item*>(object)->close += std::bind(&GUI_ActionManager::remove_item_from_source, this, std::placeholders::_1);
	GUI_Scrollable_container::add_item_control(object);
}

void GUI_ActionManager::on_item_add(tag_t const& e)
{
	GUI_Item* item = new GUI_Item(0, 0, 0,0,e.task->m_action->get_description(e.task->m_parameter), e);
	add_item_control(item);
}

void GUI_ActionManager::remove_item_control(GUI_Object* object)
{
	for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); ++Current)
	{
		if (static_cast<GUI_Item*>(*Current)== object)
		{
			int lx = (*Current)->m_position.x;
			int ly = (*Current)->m_position.y;
			if (!m_items.empty())
			{
				int cx, cy;
				for (std::list<GUI_Object*>::iterator Other = std::next(Current, 1); Other != m_items.end(); ++Other)
				{
					cx = (*Other)->m_position.x;
					cy = (*Other)->m_position.y;
					(*Other)->m_position.x = lx;
					(*Other)->m_position.y = ly;
					lx = cx;
					ly = cy;
				}
			}
			GUI_Layer::remove(object);
			if (!m_items.empty())
			{
				GUI_Object* LastElement = m_items.back();
				if (LastElement->m_position.y + LastElement->m_size.h+ m_scroll.y<m_size.h)
				{
					m_scroll.y += LastElement->m_size.h;
					if (m_scroll.y > 0) { m_scroll.y=0; }
				}
			}
			break;
		}
	}
	update();
	m_scrollbar.content_update();
}

void GUI_ActionManager::on_item_remove(tag_t const& e)
{
	for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current !=m_items.end(); ++Current)
	{
		if (static_cast<GUI_Item*>(*Current)->m_tag.task == e.task)
		{
			remove_item_control(*Current);
			break;
		}
	}
}

void GUI_ActionManager::remove_item_from_source(GUI_Object* object)
{
	m_item->remove(static_cast<GUI_Item*>(object)->m_tag.task);
}