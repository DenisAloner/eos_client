#include "GUI_PopMenu.h"


GUI_PopMenu::GUI_PopMenu(void) :GUI_Container(0, 0, 0, 0)
{
	Name="PopMenu";
}

GUI_PopMenu::~GUI_PopMenu(void)
{
}

void GUI_PopMenu::add(std::string Text, GameObject* Object)
{
	GUI_PopMenuItem* object;
	object = new GUI_PopMenuItem();
	object->Text = Text;
	object->Owner = this;
	object->Object = Object;
	if (!ItemControls->Items.empty())
	{
		GUI_Object* Back = ItemControls->Items.back();
		object->x = 0;
		object->y = Back->y + Back->height;
	/*	if (object->y + object->height>height)
		{
			Scroll.y -= object->height;
		}*/
	}
	else
	{
		object->x = 0;
		object->y = 0;
	}
	ItemControls->add(object);
	std::size_t maxlen = 0;
	for (std::list<GUI_Object*>::iterator Current = ItemControls->Items.begin(); Current != ItemControls->Items.end(); Current++)
	{
		object = (GUI_PopMenuItem*)(*Current);
		if (object->Text.length()>maxlen)
		{
			maxlen = object->Text.length();
		}
	}
	Resize(maxlen * 9+2, ItemControls->Items.size() * 18);
	for (std::list<GUI_Object*>::iterator Current = ItemControls->Items.begin(); Current != ItemControls->Items.end(); Current++)
	{
		(*Current)->width = width-1;
	}
}

void GUI_PopMenu::OnLoseFocus(GUI_Object* sender)
{
	Destroy(this);
}

void GUI_PopMenu::OnUnderCursor(MouseEventArgs const& e)
{
	for (std::list<GUI_Object*>::iterator Current = ItemControls->Items.begin(); Current != ItemControls->Items.end(); Current++)
	{
		if ((*Current)->CheckRegion(MouseEventArgs(Point(e.Position.x - x, e.Position.y - y), e.Flags)))
		{
			(*Current)->UnderCursor(MouseEventArgs(Point(e.Position.x - x, e.Position.y - y), e.Flags));
			return;
		}
	}
}


GUI_PopMenuItem::GUI_PopMenuItem(void)
{
	height = 18;
}

GUI_PopMenuItem::~GUI_PopMenuItem(void)
{
}


void GUI_PopMenuItem::OnMouseClick(MouseEventArgs const& e)
{
	SetFocus(true);
	if (Owner->Game->MessageQueue.Reader)
	{
		Parameter_GameObject* p = new Parameter_GameObject();
		p->object = Object;
		Owner->Game->MessageQueue.Push(p);
	}
	//Owner->Destroy(Owner);
	//Owner->Game->GUI->MapViewer->SetFocus(true);
	//Owner->Game->GUI->MapViewer->UnderCursor(MouseEventArgs(Point(e.Position.x + Owner->x, e.Position.y + Owner->y), e.Flags));
	//Owner->Game->GUI->MapViewer->MouseClick(MouseEventArgs(Point(e.Position.x + Owner->x, e.Position.y + Owner->y), e.Flags));
	SetFocus(false);
	Owner->Destroy(Owner);
	/*char buf2[32];
	itoa(e.Position.y + Owner->y, buf2, 10);
	MessageBox(NULL, buf2, "Down", MB_OK);*/
}


void GUI_PopMenuItem::RenderAt(GraphicalController* Graph, int px, int py)
{
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	if (focused)
	{
		glColor4d(1.0, 1.0,1.0, 0.75);
		Graph->DrawSprite(px, py, px, py + height, px + width, py + height, px + width, py);
		glColor4d(0, 0, 0, 1);
	}
	else {
		glColor4d(1, 1, 1, 1);
		//glColor4d(1, 1, 1, 0.25);
		//Graph->DrawSprite(px, py, px, py + height, px + width, py + height, px + width, py);
	}
	glEnable(GL_TEXTURE_2D);
	Graph->STextXY(px, py, Text, 8, 17);
}

void GUI_PopMenuItem::OnUnderCursor(MouseEventArgs const& e)
{
	SetFocus(true);
}

void GUI_PopMenuItem::OnGetFocus(GUI_Object* sender)
{
	Object->selected = true;
}

void GUI_PopMenuItem::OnLoseFocus(GUI_Object* sender)
{
	Object->selected = false;
}
