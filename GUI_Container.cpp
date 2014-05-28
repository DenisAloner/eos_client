#include "GUI_Container.h"


GUI_Container::GUI_Container(int _x, int _y, int _width, int _height)
{
	x = _x;
	y = _y;
	width = _width;
	height = _height;
	Name = "Container";
	ManagingControls = new GUI_Layer();
	ManagingControls->x = 0;
	ManagingControls->y = 0;
	ManagingControls->width = width;
	ManagingControls->height = height;
	ManagingControls->Name = "ManagingLayer";
	add(ManagingControls);
	ItemControls = new GUI_Layer();
	ItemControls->x = 0;
	ItemControls->y = 0;
	ItemControls->width = width;
	ItemControls->height = height;
	ItemControls->Name = "ItemLayer";
	add(ItemControls);
}


GUI_Container::~GUI_Container()
{
}

bool GUI_Container::CheckRegion(MouseEventArgs const& e)
{
	if (this->x <= e.Position.x&&this->x + width >= e.Position.x&&this->y <= e.Position.y&&this->y + height >= e.Position.y)
	{
		return true;
	}
	return false;
}

void GUI_Container::OnMouseClick(MouseEventArgs const& e)
{
	/*char buf2[32];
	itoa(SetLocalMousePosition(e).Position.x, buf2, 10);
	MessageBox(NULL, buf2, "eos", MB_OK);*/
	GUI_Layer::OnMouseClick(e);
	if ((focus != this) && (focus == nullptr))
	{
		SetFocus(true);
	}
}

void GUI_Container::OnMouseDown(MouseEventArgs const& e)
{
	GUI_Layer::OnMouseDown(e);
	if ((focus != this) && (focus == nullptr))
	{
		SetFocus(true);
	}
}

void GUI_Container::OnMouseMove(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = SetLocalMousePosition(e);
	for (std::list<GUI_Object*>::iterator Current = Items.begin(); Current != Items.end(); Current++)
	{
		if ((*Current)->CheckRegion(LocalMouseEventArgs))
		{
			(*Current)->MouseMove(LocalMouseEventArgs);
			return;
		}
	}
}

void GUI_Container::RenderAt(GraphicalController* Graph, int px, int py)
{
	
	glEnable(GL_SCISSOR_TEST);
	if (Graph->AddScissor(new GLint[4]{px, py, width, height}))
	{
		
		Graph->BlurRect(px, py, width, height);

		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor4d(0.0, 0.0, 0.0, 0.5);
		Graph->DrawSprite(px, py, px, py + height, px + width, py + height, px + width, py);
		glEnable(GL_TEXTURE_2D);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		for (auto current : Items)
		{
			(current)->RenderAt(Graph, px + current->x + Scroll.x, py + current->y + Scroll.y);
		}
		Graph->RemoveScissor();
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glVertex2d(px, py);
		glVertex2d(px, py + height);
		glVertex2d(px, py + height);
		glVertex2d(px + width, py + height);
		glVertex2d(px + width, py + height);
		glVertex2d(px + width, py);
		glVertex2d(px + width, py);
		glVertex2d(px, py);
		glEnd();
	}
}

void GUI_Container::OnMouseWheel(MouseEventArgs const& e)
{
	int delta = GET_WHEEL_DELTA_WPARAM(e.Flags);
	SetScroll(delta / 30);
}

void GUI_Container::SetScroll(int dy)
{
	if (!Items.empty())
	{
		GUI_Object* Item;
		if (dy < 0)
		{
			Item = Items.back();
			if (Item->y + Item->height + Scroll.y + dy< height)
			{
				if (Scroll.y != 0)
				{
					Scroll.y = height - (Item->y + Item->height);
				}
				return;
			}
		}
		else{
			Item = Items.front();
			if (Item->y + Scroll.y + dy > 0)
			{
				Scroll.y = 0;
				return;
			}
		}
		Scroll.y += dy;
	}
}

void GUI_Container::AddItemControl(GUI_Object* object)
{
	ItemControls->add(object);
}

void GUI_Container::RemoveItemControl(GUI_Object* object)
{
	ItemControls->remove(object);
}

void GUI_Container::AddManagingControl(GUI_Object* object)
{
	ManagingControls->add(object);
}

MouseEventArgs GUI_Container::SetLocalMousePosition(MouseEventArgs const& source)
{
	return MouseEventArgs(Point(source.Position.x - x - Scroll.x, source.Position.y - y - Scroll.y), source.Flags);
}

void GUI_Container::Resize(int _width, int _height)
{
	width = _width;
	height = _height;
	ManagingControls->width = width;
	ManagingControls->height = height;
	ItemControls->width = width;
	ItemControls->height = height;
}