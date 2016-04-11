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
//	Graph->draw_sprite(x, y, x, y + height, x + width, y + height, x + width, y);
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

void GUI_TextBox::add_item_control(GUI_Object* object)
{
	if (!m_items.empty())
	{
		GUI_Text* LastElement = static_cast<GUI_Text*>(m_items.back());
		object->m_position.x = 2;
		object->m_position.y = LastElement->m_position.y + LastElement->m_size.h;
		if (object->m_position.y + object->m_size.h>m_size.h)
		{
			m_scroll.y -= object->m_size.h;
		}
	}
	else
	{
		object->m_position.x = 2;
		object->m_position.y = 0;
		m_scroll.y = 0;
	}
	GUI_Layer::add(object);
}

void GUI_TextBox::set_scroll(int dy)
{
	if (!m_items.empty())
	{
		GUI_Text* Item;
		if (dy < 0)
		{
			Item = static_cast<GUI_Text*>(m_items.back());
			if (Item->m_position.y + Item->m_size.h + m_scroll.y + dy < m_size.h)
			{
				if (m_scroll.y != 0)
				{
					m_scroll.y = m_size.h - (Item->m_position.y + Item->m_size.h);
				}
				return;
			}
		}
		else 
		{
			Item = static_cast<GUI_Text*>(m_items.front());
			if (Item->m_position.y + m_scroll.y + dy > 0)
			{
				m_scroll.y = 0;
				return;
			}
		}
		m_scroll.y += dy;
	}
}

void GUI_TextBox::on_mouse_wheel(MouseEventArgs const& e)
{
//#warning FIXME Что за магическое 30?
	set_scroll(e.value / 30);
}