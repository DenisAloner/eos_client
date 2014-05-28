#include "GUI_TextBox.h"


GUI_TextBox::GUI_TextBox() :GUI_Container(0, 0, 0, 0)
{
}


GUI_TextBox::~GUI_TextBox()
{
}

//void GUI_TextBox::Render(GraphicalController* Graph)
//{
//	glEnable(GL_SCISSOR_TEST);
//	glScissor(x - 1, 1024 - y - height-3, width - 10, height - 11);
//	glEnable(GL_BLEND);
//	glDisable(GL_TEXTURE_2D);
//	glColor4d(1.0,1.0, 1.0, 0.5);
//	Graph->DrawSprite(x, y, x, y + height, x + width, y + height, x + width, y);
//	glEnable(GL_TEXTURE_2D);
//	glColor4d(1.0, 1.0, 1.0, 1.0);
//	for (auto current : Items)
//	{
//		static_cast<GUI_Text*>(current)->RenderAt(Graph, x + current->x + Scroll.x, y + current->y+Scroll.y);
//	}
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
//	glDisable(GL_SCISSOR_TEST);
//}

void GUI_TextBox::AddItemControl(GUI_Object* object)
{
	if (!ItemControls->Items.empty())
	{
		GUI_Text* LastElement = static_cast<GUI_Text*>(ItemControls->Items.back());
		object->x = 0;
		object->y = LastElement->y + LastElement->height;
		if (object->y + object->height>height)
		{
			Scroll.y -= object->height;
		}
	}
	else
	{
		object->x =0;
		object->y =0;
	}
	ItemControls->add(object);
}

void GUI_TextBox::SetScroll(int dy)
{
	if (!ItemControls->Items.empty())
	{
		GUI_Text* Item;
		if (dy < 0)
		{
			Item = static_cast<GUI_Text*>(ItemControls->Items.back());
			if (Item->y + Item->height + Scroll.y + dy< height)
			{
				if (Scroll.y!=0)
				{
					Scroll.y = height - (Item->y + Item->height);
				}
				return;
			}
		}
		else{
			Item = static_cast<GUI_Text*>(ItemControls->Items.front());
			if (Item->y + Scroll.y + dy > 0)
			{
				Scroll.y = 0;
				return;
			}
		}
		Scroll.y += dy;
	}
}

void GUI_TextBox::OnMouseWheel(MouseEventArgs const& e)
{
	int delta = GET_WHEEL_DELTA_WPARAM(e.Flags);
	SetScroll(delta/30);
}