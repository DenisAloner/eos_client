#include "GUI_Container.h"


GUI_Container::GUI_Container(int _x, int _y, int _width, int _height)
{
	m_position.x = _x;
	m_position.y = _y;
	m_size.w = _width;
	m_size.h = _height;
	m_managing_controls = new GUI_Layer();
	m_managing_controls->m_position.x = 0;
	m_managing_controls->m_position.y = 0;
	m_managing_controls->m_size.w = m_size.w;
	m_managing_controls->m_size.h = m_size.h;
	add(m_managing_controls);
	m_item_controls = new GUI_Layer();
	m_item_controls->m_position.x = 0;
	m_item_controls->m_position.y = 0;
	m_item_controls->m_size.w = m_size.w;
	m_item_controls->m_size.h = m_size.h;
	add(m_item_controls);
}


GUI_Container::~GUI_Container()
{
}

bool GUI_Container::check_region(MouseEventArgs const& e)
{
	if (this->m_position.x <= e.position.x&&this->m_position.x + m_size.w >= e.position.x&&this->m_position.y <= e.position.y&&this->m_position.y + m_size.h >= e.position.y)
	{
		return true;
	}
	return false;
}

void GUI_Container::on_mouse_click(MouseEventArgs const& e)
{
	/*char buf2[32];
	itoa(SetLocalMousePosition(e).Position.x, buf2, 10);
	MessageBox(NULL, buf2, "eos", MB_OK);*/
	GUI_Layer::on_mouse_click(e);
	if ((m_focus != this) && (m_focus == nullptr))
	{
		set_focus(true);
	}
}

void GUI_Container::on_mouse_down(MouseEventArgs const& e)
{
	GUI_Layer::on_mouse_down(e);
	if ((m_focus != this) && (m_focus == nullptr))
	{
		set_focus(true);
	}
}

void GUI_Container::on_mouse_move(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
	for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); Current++)
	{
		if ((*Current)->check_region(LocalMouseEventArgs))
		{
			(*Current)->mouse_move(LocalMouseEventArgs);
			return;
		}
	}
}

void GUI_Container::render(GraphicalController* Graph, int px, int py)
{
	
	glEnable(GL_SCISSOR_TEST);
	if (Graph->add_scissor(frectangle_t((float)px, (float)py, (float)m_size.w, (float)m_size.h)))
	{
		
		Graph->blur_rect(px, py, m_size.w, m_size.h);

		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor4d(0.0, 0.0, 0.0, 0.5);
		Graph->draw_sprite(px, py, px, py + m_size.h, px + m_size.w, py + m_size.h, px + m_size.w, py);
		glEnable(GL_TEXTURE_2D);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		for (auto current : m_items)
		{
			(current)->render(Graph, px + current->m_position.x + m_scroll.x, py + current->m_position.y + m_scroll.y);
		}
		Graph->remove_scissor();
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glVertex2d(px, py);
		glVertex2d(px, py + m_size.h);
		glVertex2d(px, py + m_size.h);
		glVertex2d(px + m_size.w, py + m_size.h);
		glVertex2d(px + m_size.w, py + m_size.h);
		glVertex2d(px + m_size.w, py);
		glVertex2d(px + m_size.w, py);
		glVertex2d(px, py);
		glEnd();
	}
}

void GUI_Container::on_mouse_wheel(MouseEventArgs const& e)
{
	int delta = GET_WHEEL_DELTA_WPARAM(e.flags);
	set_scroll(delta / 30);
}

void GUI_Container::set_scroll(int dy)
{
	if (!m_items.empty())
	{
		GUI_Object* Item;
		if (dy < 0)
		{
			Item = m_items.back();
			if (Item->m_position.y + Item->m_size.h + m_scroll.y + dy< m_size.h)
			{
				if (m_scroll.y != 0)
				{
					m_scroll.y = m_size.h - (Item->m_position.y + Item->m_size.h);
				}
				return;
			}
		}
		else{
			Item = m_items.front();
			if (Item->m_position.y + m_scroll.y + dy > 0)
			{
				m_scroll.y = 0;
				return;
			}
		}
		m_scroll.y += dy;
	}
}

void GUI_Container::add_item_control(GUI_Object* object)
{
	m_item_controls->add(object);
}

void GUI_Container::remove_item_control(GUI_Object* object)
{
	m_item_controls->remove(object);
}

void GUI_Container::add_managing_control(GUI_Object* object)
{
	m_managing_controls->add(object);
}

MouseEventArgs GUI_Container::set_local_mouse_position(MouseEventArgs const& source)
{
	return MouseEventArgs(position_t(source.position.x - m_position.x - m_scroll.x, source.position.y - m_position.y - m_scroll.y), source.flags);
}

void GUI_Container::resize(int _width, int _height)
{
	m_size.w = _width;
	m_size.h = _height;
	m_managing_controls->m_size.w = m_size.w;
	m_managing_controls->m_size.h = m_size.h;
	m_item_controls->m_size.w = m_size.w;
	m_item_controls->m_size.h = m_size.h;
}