#include "GUI_Layer.h"
#include <Windows.h>

GUI_Layer::GUI_Layer()
{
	focus=nullptr;
}


GUI_Layer::~GUI_Layer(void)
{
}

MouseEventArgs GUI_Layer::SetLocalMousePosition(MouseEventArgs const& source)
{
	return MouseEventArgs(Point(source.Position.x - x, source.Position.y - y), source.Flags);
}

void GUI_Layer::OnMouseClick(MouseEventArgs const& e)
{ 
	MouseEventArgs LocalMouseEventArgs = SetLocalMousePosition(e);
	for(std::list<GUI_Object*>::iterator Current=Items.begin();Current!=Items.end();Current++)
	{
		
		if ((*Current)->CheckRegion(LocalMouseEventArgs))
		{
			(*Current)->MouseClick(LocalMouseEventArgs);
			return;
		}
	}
}

void GUI_Layer::OnMouseDown(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = SetLocalMousePosition(e);
	for (std::list<GUI_Object*>::iterator Current = Items.begin(); Current != Items.end(); Current++)
	{
		if ((*Current)->CheckRegion(LocalMouseEventArgs))
		{
			(*Current)->MouseDown(LocalMouseEventArgs);
			return;
		}
	}
}

void GUI_Layer::OnMouseScroll(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = SetLocalMousePosition(e);
	if (focus)
	{
		focus->MouseScroll(LocalMouseEventArgs);
	}
}

void GUI_Layer::OnMouseMove(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = SetLocalMousePosition(e);
	if (focus)
	{
		focus->MouseMove(LocalMouseEventArgs);
	}
}

void GUI_Layer::OnMouseWheel(MouseEventArgs const& e)
{
	if (focus)
	{
		focus->MouseWheel(e);
	}
}

void GUI_Layer::RenderAt(GraphicalController* Graph, int px, int py)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgramObjectARB(0);
	for (std::list<GUI_Object*>::reverse_iterator Current = Items.rbegin(); Current != Items.rend(); Current++)
	{
		(*Current)->RenderAt(Graph, px + (*Current)->x , py + (*Current)->y );
	}
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

KeyboardEventCallback GUI_Layer::GetKeyboardEventCallback()
{
	return std::bind(&GUI_Layer::OnKeyPress,this,std::placeholders::_1);
}

MouseEventCallback GUI_Layer::GetMouseEventCallback()
{
	return std::bind(&GUI_Layer::OnMouseClick,this,std::placeholders::_1);
}


void GUI_Layer::OnKeyPress(WPARAM w)
{
	if(focus)
	{
		focus->KeyPress(w);
	}
}

void GUI_Layer::SetFocus(bool state)
{
	focused=state;
	if(state)
	{
		if(focus!=nullptr)
		{
			focus->SetFocus(false);
		}
		GetFocus(this);
	} else {
		if(focus!=nullptr)
		{
			focus->SetFocus(false);
			focus=nullptr;
		}
		LoseFocus(this);
	}
}


void GUI_Layer::OnUnderCursor(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = SetLocalMousePosition(e);
	if(focus!=nullptr)
	{
		focus->UnderCursor(LocalMouseEventArgs);
	}
}

void GUI_Layer::add(GUI_Object* object)
{
	Items.push_back(object);
	object->GetFocus+=std::bind(&GUI_Layer::OnItemGetFocus,this,std::placeholders::_1);
	object->Destroy += std::bind(&GUI_Layer::remove, this, std::placeholders::_1);
}

void GUI_Layer::AddFront(GUI_Object* object)
{
	Items.push_front(object);
	object->GetFocus+=std::bind(&GUI_Layer::OnItemGetFocus,this,std::placeholders::_1);
	object->Destroy += std::bind(&GUI_Layer::remove, this, std::placeholders::_1);
}

void GUI_Layer::remove(GUI_Object* object)
{
	if(focus==object)
	{
		focus=nullptr;
	}
	Items.remove(object);
}

bool GUI_Layer::CheckRegion(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = SetLocalMousePosition(e);
	if(this->x<=e.Position.x&&this->x+width>=e.Position.x&&this->y<=e.Position.y&&this->y+height>=e.Position.y)
	{
		for(std::list<GUI_Object*>::iterator Current=Items.begin();Current!=Items.end();Current++)
		{
			if ((*Current)->CheckRegion(LocalMouseEventArgs))
			{
				return true;
			}
		}
	}
	return false;
}

void GUI_Layer::OnGetFocus(GUI_Object* sender)
{
}

void GUI_Layer::OnItemGetFocus(GUI_Object* sender)
{
	if(focus!=sender)
	{
	SetFocus(true);
	focus=sender;
	}
}