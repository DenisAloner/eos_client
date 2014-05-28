#include "GUI_Object.h"


GUI_Object::GUI_Object(void)
{
	focused=false;
	KeyPress+=std::bind(&GUI_Object::OnKeyPress,this,std::placeholders::_1);
	MouseClick+=std::bind(&GUI_Object::OnMouseClick,this,std::placeholders::_1);
	UnderCursor+=std::bind(&GUI_Object::OnUnderCursor,this,std::placeholders::_1);
	GetFocus+=std::bind(&GUI_Object::OnGetFocus,this,std::placeholders::_1);
	LoseFocus+=std::bind(&GUI_Object::OnLoseFocus,this,std::placeholders::_1);
	MouseDown += std::bind(&GUI_Object::OnMouseDown, this, std::placeholders::_1);
	MouseScroll += std::bind(&GUI_Object::OnMouseScroll, this, std::placeholders::_1);
	MouseWheel += std::bind(&GUI_Object::OnMouseWheel, this, std::placeholders::_1);
	MouseMove += std::bind(&GUI_Object::OnMouseMove, this, std::placeholders::_1);
}


GUI_Object::~GUI_Object(void)
{
}

void GUI_Object::RenderAt(GraphicalController* Graph, int px, int py)
{
}

MouseEventCallback GUI_Object::GetMouseEventCallback()
{
	return std::bind(&GUI_Object::OnMouseClick,this,std::placeholders::_1);
}

void GUI_Object::OnMouseClick(MouseEventArgs const& e)
{
	SetFocus(true);
}

void GUI_Object::OnMouseDown(MouseEventArgs const& e)
{
	SetFocus(true);
}

void GUI_Object::OnMouseScroll(MouseEventArgs const& e)
{
}

void GUI_Object::OnMouseWheel(MouseEventArgs const& e)
{
}

void GUI_Object::OnMouseMove(MouseEventArgs const& e)
{
}

KeyboardEventCallback GUI_Object::GetKeyboardEventCallback()
{
	return std::bind(&GUI_Object::OnKeyPress,this,std::placeholders::_1);
}

void GUI_Object::OnKeyPress(WPARAM w)
{
}

void  GUI_Object::SetFocus(bool state)
{
	focused=state;
	if(state)
	{
		GetFocus(this);
	} else {
		LoseFocus(this);
	}
}

void GUI_Object::OnUnderCursor(MouseEventArgs const& e)
{
}

bool GUI_Object::CheckRegion(MouseEventArgs const& e)
{
	if(this->x<=e.Position.x&&this->x+width>=e.Position.x&&this->y<=e.Position.y&&this->y+height>=e.Position.y)
	{
		return true;
	}
	return false;
}

void GUI_Object::OnGetFocus(GUI_Object* sender)
{
}

void GUI_Object::OnLoseFocus(GUI_Object* sender)
{
}
