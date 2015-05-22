#include "GUI_ActionManager.h"
#include "Action.h"


GUI_ActionManager::GUI_ActionManager(ActionManager* ActionManager) :GUI_Container(0, 0, 0, 0)
{
	m_item = ActionManager;
	m_item->add_item_event += std::bind(&GUI_ActionManager::on_item_add, this, std::placeholders::_1);
	m_item->remove_item_event += std::bind(&GUI_ActionManager::on_item_remove, this, std::placeholders::_1);
}


GUI_ActionManager::~GUI_ActionManager(void)
{
}

//void GUI_ActionManager::Render(GraphicalController* Graph)
//{
//	glEnable(GL_BLEND);
//	glDisable(GL_TEXTURE_2D);
//	glColor4d(1.0, 1.0, 1.0, 0.5);
//	Graph->draw_sprite(x,y,x,y+height,x+width,y+height,x+width,y);
//	glEnable(GL_TEXTURE_2D);
//	int i=0;
//	glColor4d(0.0, 0.0, 0.0, 1.0);
//	for(auto current:Item->Actions)
//	{
//		Graph->TextXY(x+5,y+i,current->Action->GetDescription(current->Parameter),8,17);
//		i+=18;
//	}
//	//Graph->CenterText(x+width/2,y+height/2,Text,8,17);
//	glDisable(GL_BLEND);
//	glDisable(GL_TEXTURE_2D);
//	glBegin(GL_LINES);
//	glColor4f(1.0, 1.0, 1.0, 1.0);
//	glVertex2d(x, y);
//	glVertex2d(x, y + height);
//	glVertex2d(x, y + height);
//	glVertex2d(x + width, y + height);
//	glVertex2d(x + width, y + height);
//	glVertex2d(x + width, y);
//	glVertex2d(x + width, y);
//	glVertex2d(x, y);
//	glEnd();
//}

void GUI_ActionManager::add_item_control(GUI_Object* object)
{
	static_cast<GUI_Item*>(object)->resize(m_size.w - 4, 21);
	/*object->width = width - 4;
	object->height = 21;*/
	if (!m_items.empty())
	{
		GUI_Item* LastElement = static_cast<GUI_Item*>(m_items.back());
		object->m_position.x = 2;
		object->m_position.y = LastElement->m_position.y + LastElement->m_size.h + 2;
		if (object->m_position.y + object->m_size.h>m_size.h)
		{
			m_scroll.y -= object->m_size.h;
		}
	}
	else
	{
		object->m_position.x = 2;
		object->m_position.y = 2;
	}
	static_cast<GUI_Item*>(object)->close += std::bind(&GUI_ActionManager::remove_item_from_source, this, std::placeholders::_1);
	GUI_Layer::add(object);
}

void GUI_ActionManager::on_item_add(tag_t const& e)
{
	GUI_Item* item = new GUI_Item(0, 0, 0,0,e.object->m_name+": "+ e.task->m_action->get_description(e.task->m_parameter), e);
	add_item_control(item);
}

void GUI_ActionManager::remove_item_control(GUI_Object* object)
{
	for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); Current++)
	{
		if (static_cast<GUI_Item*>(*Current)== object)
		{
			int lx = (*Current)->m_position.x;
			int ly = (*Current)->m_position.y;
			if (!m_items.empty())
			{
				int cx, cy;
				for (std::list<GUI_Object*>::iterator Other = std::next(Current, 1); Other != m_items.end(); Other++)
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
			break;
		}
	}
}

void GUI_ActionManager::on_item_remove(tag_t const& e)
{
	for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current !=m_items.end(); Current++)
	{
		if (static_cast<GUI_Item*>(*Current)->m_tag.object == e.object && static_cast<GUI_Item*>(*Current)->m_tag.task == e.task)
		{
			remove_item_control((*Current));
			break;
		}
	}
}

void GUI_ActionManager::remove_item_from_source(GUI_Object* object)
{
	m_item->remove(static_cast<GUI_Item*>(object)->m_tag.object,static_cast<GUI_Item*>(object)->m_tag.task);
}