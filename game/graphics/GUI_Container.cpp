#include "GUI_Container.h"


GUI_Container::GUI_Container(int x, int y, int width, int height, bool border_visible) :GUI_Layer(x, y, width, height)
{
	m_border_visible = border_visible;
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
	for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); ++Current)
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
		GraphicalController::rectangle_t rect(px, py, m_size.w, m_size.h);
		Graph->draw_sprite(rect);
		glEnable(GL_TEXTURE_2D);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		for (auto current : m_items)
		{
			(current)->render(Graph, px + current->m_position.x + m_scroll.x, py + current->m_position.y + m_scroll.y);
		}
		Graph->remove_scissor();
		if (m_border_visible)
		{
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glColor4f(1.0, 1.0, 1.0, 1.0);
			Graph->draw_rectangle(GraphicalController::rectangle_t(px, py, m_size.w, m_size.h));
		}
	}
}

void GUI_Container::on_mouse_wheel(MouseEventArgs const& e)
{
//#warning FIXME Создать дефайн или лучше статическую константу для значения 30, обозвать понятным образом
	set_scroll(e.value / 30);
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

MouseEventArgs GUI_Container::set_local_mouse_position(MouseEventArgs const& source)
{
	return MouseEventArgs(position_t(source.position.x - m_position.x - m_scroll.x, source.position.y - m_position.y - m_scroll.y), source.key, source.value);
}

void GUI_Container::resize(int _width, int _height)
{
	m_size.w = _width;
	m_size.h = _height;
}

GUI_Scrollbar_vertical::GUI_Scrollbar_vertical(GUI_Scrollable_container* owner) :m_owner(owner)
{
}

void GUI_Scrollbar_vertical::render(GraphicalController* Graph, int px, int py)
{
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor4d(1.0, 1.0, 1.0, 0.15);
	GraphicalController::rectangle_t rect(px, py, m_size.w, m_size.h);
	Graph->draw_sprite(rect);
	glColor4d(1.0, 1.0, 1.0, 0.35);
	rect = GraphicalController::rectangle_t(px, py + m_bar_top, m_size.w, m_bar_height);
	Graph->draw_sprite(rect);
}

void GUI_Scrollbar_vertical::content_update()
{
	if (m_owner->m_items.empty())
	{
		m_bar_top = 0;
		m_bar_height = m_size.h;
	}
	else
	{
		m_bar_top = std::abs(m_owner->m_scroll.y / static_cast<float>(m_owner->m_content_size.h)*m_size.h);
		m_bar_height = m_owner->m_size.h / static_cast<float>(m_owner->m_content_size.h)*m_size.h;
	}
}

MouseEventArgs GUI_Scrollbar_vertical::set_local_mouse_control(MouseEventArgs const& source)
{
	return MouseEventArgs(position_t(source.position.x - m_position.x, source.position.y - m_position.y), source.key, source.value);
}

void GUI_Scrollbar_vertical::set_scroll_top(int value)
{
	m_bar_top = value;
	if (m_bar_top < 0) { m_bar_top = 0; }
	if (m_bar_top + m_bar_height > m_size.h) { m_bar_top = m_size.h - m_bar_height; }
	//GUI_Object& LastElement = *m_owner->m_items.back();
	//int total = LastElement.m_position.y + LastElement.m_size.h + 2;
	m_owner->set_scroll2(-(float)m_bar_top / (float)m_size.h*m_owner->m_content_size.h);
}

void GUI_Scrollbar_vertical::on_mouse_click(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = set_local_mouse_control(e);
	if(LocalMouseEventArgs.position.y<m_bar_top)
	{
		set_scroll_top(LocalMouseEventArgs.position.y);
	} else if (LocalMouseEventArgs.position.y>m_bar_top+m_bar_height)
	{
		set_scroll_top(LocalMouseEventArgs.position.y - m_bar_height);
	}
	set_focus(true);
}


void GUI_Scrollbar_vertical::on_mouse_start_drag(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = set_local_mouse_control(e);
	m_pos = LocalMouseEventArgs.position.y;
}

void GUI_Scrollbar_vertical::on_mouse_drag(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = set_local_mouse_control(e);
	set_scroll_top(m_bar_top - m_pos + LocalMouseEventArgs.position.y);
	m_pos = LocalMouseEventArgs.position.y;
}

bool GUI_Scrollbar_vertical::check_region(MouseEventArgs const& e)
{
	if (this->m_position.x <= e.position.x&&this->m_position.x + m_size.w >= e.position.x&&this->m_position.y <= e.position.y&&this->m_position.y + m_size.h >= e.position.y)
	{
		return true;
	}
	return false;
}


GUI_Scrollable_container::GUI_Scrollable_container(int x, int y, int width, int height, bool border_visible) :GUI_Container(x, y, width, height), m_scrollbar(this)
{
	m_scrollbar.get_focus += std::bind(&GUI_Scrollable_container::on_item_get_focus, this, std::placeholders::_1);
	start_render = m_items.begin();
	end_render = m_items.end();
	m_scrollbar.m_position = position_t(width - 20, 2);
	m_scrollbar.resize(18, height - 3);
}

void GUI_Scrollable_container::add_item_control(GUI_Object* object)
{
	if (!m_items.empty())
	{
		GUI_Object* LastElement = m_items.back();
		object->m_position.x = 2;
		object->m_position.y = LastElement->m_position.y + LastElement->m_size.h + 2;
		if (object->m_position.y + object->m_size.h + m_scroll.y + 2 > m_size.h)
		{
			m_scroll.y = (m_size.h - object->m_position.y - object->m_size.h - 2);
		}
		m_content_size.h = object->m_position.y + object->m_size.h + 2;
	}
	else
	{
		object->m_position.x = 2;
		object->m_position.y = 2;
		m_scroll.y = 0;
		m_content_size.h = 2 + object->m_size.h + 2;
	}
	GUI_Layer::add(object);
	update();
	m_scrollbar.content_update();
}

void GUI_Scrollable_container::set_scroll(int dy)
{
	if (!m_items.empty())
	{
		if (dy < 0)
		{
			if (m_content_size.h + m_scroll.y + dy < m_size.h)
			{
				m_scroll.y = m_size.h - m_content_size.h;
				if (m_scroll.y > 0)
				{
					m_scroll.y = 0;
				}
				update();
				m_scrollbar.content_update();
				return;
			}
		}
		else
		{
			GUI_Object * Item = m_items.front();
			if (Item->m_position.y + m_scroll.y + dy > 0)
			{
				m_scroll.y = 0;
				update();
				m_scrollbar.content_update();
				return;
			}
		}
		m_scroll.y += dy;
		update();
		m_scrollbar.content_update();
	}
}

void GUI_Scrollable_container::set_scroll2(int dy)
{
	if (!m_items.empty())
	{
		if (dy < 0)
		{
			if (m_content_size.h + dy < m_size.h)
			{
				m_scroll.y = m_size.h - m_content_size.h;
				if (m_scroll.y > 0)
				{
					m_scroll.y = 0;
				}
				update();
				return;
			}
		}
		else
		{
			GUI_Object * Item = m_items.front();
			if (Item->m_position.y + dy > 0)
			{
				m_scroll.y = 0;
				update();
				return;
			}
		}
		m_scroll.y = dy;
		update();
	}
}

void GUI_Scrollable_container::update()
{
	if (m_content_size.h + m_scroll.y < m_size.h)
	{
		m_scroll.y = m_size.h - m_content_size.h;
	}
	if (m_scroll.y > 0) { m_scroll.y = 0; }
	start_render = m_items.begin();
	end_render = m_items.end();
	for (auto i = m_items.begin(); i != m_items.end(); ++i)
	{
		if ((*i)->m_position.y + (*i)->m_size.h + m_scroll.y+2>0)
		{
			start_render = i;
			for (auto j = i; j != m_items.end(); ++j)
			{
				if ((*j)->m_position.y + (*j)->m_size.h + m_scroll.y +2> m_size.h)
				{
					end_render = ++j;
					return;
				}
			}
			end_render = m_items.end();
			return;
		}
	}
}


void GUI_Scrollable_container::render(GraphicalController* Graph, int px, int py)
{

	glEnable(GL_SCISSOR_TEST);
	if (Graph->add_scissor(frectangle_t((float)px, (float)py, (float)m_size.w, (float)m_size.h)))
	{
		Graph->blur_rect(px, py, m_size.w, m_size.h);
		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor4d(0.0, 0.0, 0.0, 0.5);
		GraphicalController::rectangle_t rect(px, py, m_size.w, m_size.h);
		Graph->draw_sprite(rect);
		glEnable(GL_TEXTURE_2D);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		for (auto i = start_render; i != end_render; ++i)
		{
			(*i)->render(Graph, px + (*i)->m_position.x + m_scroll.x, py + (*i)->m_position.y + m_scroll.y);
		}
		m_scrollbar.render(Graph, px + m_scrollbar.m_position.x, py + m_scrollbar.m_position.y);
		Graph->remove_scissor();
		if (m_border_visible)
		{
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glColor4f(1.0, 1.0, 1.0, 1.0);
			Graph->draw_rectangle(GraphicalController::rectangle_t(px, py, m_size.w, m_size.h));
		}
	}
}

void GUI_Scrollable_container::resize(int width, int height)
{
	m_size.w = width;
	m_size.h = height;
	m_scrollbar.m_position = position_t(width - 20, 2);
	m_scrollbar.resize(18, height - 3);
}

void GUI_Scrollable_container::on_mouse_wheel(MouseEventArgs const& e)
{
	//#warning FIXME Что за магическое 30?
	set_scroll(e.value / 5);
}


void GUI_Scrollable_container::on_mouse_click(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = MouseEventArgs(position_t(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value);
	if (m_scrollbar.check_region(LocalMouseEventArgs))
	{
		m_scrollbar.mouse_click(LocalMouseEventArgs);
		return;
	}
	LocalMouseEventArgs = set_local_mouse_position(e);
	for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); ++Current)
	{
		if ((*Current)->check_region(LocalMouseEventArgs))
		{
			(*Current)->mouse_click(LocalMouseEventArgs);
			return;
		}
	}
	if ((m_focus != this) && (m_focus == nullptr))
	{
		set_focus(true);
	}
}

void GUI_Scrollable_container::on_mouse_down(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = MouseEventArgs(position_t(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value);
	if (m_scrollbar.check_region(LocalMouseEventArgs))
	{
		m_scrollbar.mouse_down(LocalMouseEventArgs);
		return;
	}
	LocalMouseEventArgs = set_local_mouse_position(e);
	for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); ++Current)
	{
		if ((*Current)->check_region(LocalMouseEventArgs))
		{
			(*Current)->mouse_down(LocalMouseEventArgs);
			return;
		}
	}
	if ((m_focus != this) && (m_focus == nullptr))
	{
		set_focus(true);
	}
}

void GUI_Scrollable_container::on_mouse_start_drag(MouseEventArgs const& e)
{
	if (m_focus)
	{
		if (m_focus == &m_scrollbar)
		{
			MouseEventArgs LocalMouseEventArgs = MouseEventArgs(position_t(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value);
			m_focus->mouse_start_drag(LocalMouseEventArgs);
		}
		else
		{
			MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
			m_focus->mouse_start_drag(LocalMouseEventArgs);
		}
	}
}

void GUI_Scrollable_container::on_mouse_drag(MouseEventArgs const& e)
{
	if (m_focus)
	{
		if (m_focus == &m_scrollbar)
		{
			MouseEventArgs LocalMouseEventArgs = MouseEventArgs(position_t(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value);
			m_focus->mouse_drag(LocalMouseEventArgs);
		}
		else
		{
			MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
			m_focus->mouse_drag(LocalMouseEventArgs);
		}
	}
}

void GUI_Scrollable_container::on_mouse_move(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = MouseEventArgs(position_t(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value);
	if (m_scrollbar.check_region(LocalMouseEventArgs))
	{
		m_scrollbar.mouse_move(LocalMouseEventArgs);
		return;
	}
	LocalMouseEventArgs = set_local_mouse_position(e);
	for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); ++Current)
	{
		if ((*Current)->check_region(LocalMouseEventArgs))
		{
			(*Current)->mouse_move(LocalMouseEventArgs);
			return;
		}
	}
}