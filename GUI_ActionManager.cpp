#include "GUI_ActionManager.h"


GUI_ActionManager::GUI_ActionManager(TActionManager* ActionManager) :GUI_Container(0, 0, 0, 0)
{
	Item = ActionManager;
	Item->AddEvent += std::bind(&GUI_ActionManager::OnItemAdd, this, std::placeholders::_1);
	Item->RemoveEvent += std::bind(&GUI_ActionManager::OnItemRemove, this, std::placeholders::_1);
}


GUI_ActionManager::~GUI_ActionManager(void)
{
}

//void GUI_ActionManager::Render(GraphicalController* Graph)
//{
//	glEnable(GL_BLEND);
//	glDisable(GL_TEXTURE_2D);
//	glColor4d(1.0, 1.0, 1.0, 0.5);
//	Graph->DrawSprite(x,y,x,y+height,x+width,y+height,x+width,y);
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

void GUI_ActionManager::AddItemControl(GUI_Object* object)
{
	static_cast<GUI_Item*>(object)->Resize(width - 4, 21);
	/*object->width = width - 4;
	object->height = 21;*/
	if (!ItemControls->Items.empty())
	{
		GUI_Item* LastElement = static_cast<GUI_Item*>(ItemControls->Items.back());
		object->x = 2;
		object->y = LastElement->y + LastElement->height + 2;
		if (object->y + object->height>height)
		{
			Scroll.y -= object->height;
		}
	}
	else
	{
		object->x = 2;
		object->y = 2;
	}
	static_cast<GUI_Item*>(object)->Close += std::bind(&GUI_ActionManager::RemoveItemFromSource, this, std::placeholders::_1);
	ItemControls->add(object);
}

void GUI_ActionManager::OnItemAdd(GameTask* const& e)
{
	GUI_Item* item = new GUI_Item(0, 0, 0,0, e->Action->GetDescription(e->Parameter), e);
	item->Name = "Item";
	AddItemControl(item);
}

void GUI_ActionManager::RemoveItemControl(GUI_Object* object)
{
	for (std::list<GUI_Object*>::iterator Current = ItemControls->Items.begin(); Current != ItemControls->Items.end(); Current++)
	{
		if (static_cast<GUI_Item*>(*Current)== object)
		{
			int lx = (*Current)->x;
			int ly = (*Current)->y;
			if (!Items.empty())
			{
				int cx, cy;
				for (std::list<GUI_Object*>::iterator Other = std::next(Current, 1); Other != ItemControls->Items.end(); Other++)
				{
					cx = (*Other)->x;
					cy = (*Other)->y;
					(*Other)->x = lx;
					(*Other)->y = ly;
					lx = cx;
					ly = cy;
				}
			}
			ItemControls->remove(object);
			break;
		}
	}
}

void GUI_ActionManager::OnItemRemove(GameTask* const& e)
{
	for (std::list<GUI_Object*>::iterator Current = ItemControls->Items.begin(); Current != ItemControls->Items.end(); Current++)
	{
		if (static_cast<GUI_Item*>(*Current)->Tag == e)
		{
			RemoveItemControl((*Current));
			break;
		}
	}
}

void GUI_ActionManager::RemoveItemFromSource(GUI_Object* object)
{
	Item->Remove(static_cast<GUI_Item*>(object)->Tag);
}