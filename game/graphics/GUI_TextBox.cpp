#include "GUI_TextBox.h"


GUI_Scrollbar_vertical::GUI_Scrollbar_vertical(GUI_TextBox* owner):m_owner(owner)
{
	m_is_moving = false;
	start_moving += std::bind(&GUI_Scrollbar_vertical::on_start_moving, this, std::placeholders::_1);
	move += std::bind(&GUI_Scrollbar_vertical::on_move, this, std::placeholders::_1);
	end_moving += std::bind(&GUI_Scrollbar_vertical::on_end_moving, this, std::placeholders::_1);
}

void GUI_Scrollbar_vertical::render(GraphicalController* Graph, int px, int py)
{
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor4d(1.0, 1.0, 1.0, 0.15);
	GraphicalController::rectangle_t rect(px, py, m_size.w, m_size.h);
	Graph->draw_sprite(rect);
	glColor4d(1.0, 1.0, 1.0, 0.35);
	rect=GraphicalController::rectangle_t(px, py+m_bar_top, m_size.w, m_bar_height);
	Graph->draw_sprite(rect);
}

void GUI_Scrollbar_vertical::content_update()
{
	if (m_owner->m_items.empty())
	{
		m_bar_top = 0;
		m_bar_height =m_size.h;
	}
	else
	{
		GUI_Object& LastElement = *m_owner->m_items.back();
		int total = LastElement.m_position.y + LastElement.m_size.h;
		m_bar_top = std::abs((float)m_owner->m_scroll.y / (float)total*m_size.h);
		m_bar_height = (float)m_owner->m_size.h / (float)total*m_size.h;
	}
	
}

MouseEventArgs GUI_Scrollbar_vertical::set_local_mouse_control(MouseEventArgs const& source)
{
	return MouseEventArgs(position_t(source.position.x - m_position.x, source.position.y - m_position.y), source.key, source.value);
}

void GUI_Scrollbar_vertical::on_mouse_move(MouseEventArgs const& e)
{
	if (e.key == mk_left)
	{
		if (!m_is_moving) start_moving(e);
		MouseEventArgs LocalMouseEventArgs = set_local_mouse_control(e);
		move(LocalMouseEventArgs);
	}
	else
	{
		if (m_is_moving) end_moving(e);
	}
}

void GUI_Scrollbar_vertical::on_start_moving(MouseEventArgs const& e)
{
	m_is_moving = true;
	MouseEventArgs LocalMouseEventArgs = set_local_mouse_control(e);
	LOG(INFO) << "INITIAL  " << std::to_string(LocalMouseEventArgs.position.y)<<"    "<< std::to_string(e.position.y);
	m_pos = LocalMouseEventArgs.position.y;
}

void GUI_Scrollbar_vertical::on_move(MouseEventArgs const& e)
{
	/*position_t p = local_xy(position_t(e.position.x, e.position.y));
	
	m_center.x = m_initial_position.x + p.x;
	m_center.y = m_initial_position.y + p.y;*/
	LOG(INFO) << "MOVE  "<<std::to_string(e.position.y);
	m_bar_top += -m_pos + e.position.y;
	m_pos = e.position.y;
	if (m_bar_top < 0) { m_bar_top = 0; }
	if (m_bar_top + m_bar_height > m_size.h) { m_bar_top= m_size.h- m_bar_height; }
	GUI_Object& LastElement = *m_owner->m_items.back();
	int total = LastElement.m_position.y + LastElement.m_size.h;
	m_owner->set_scroll2(-(float)m_bar_top/(float)m_size.h*total);
	
}

void GUI_Scrollbar_vertical::on_end_moving(MouseEventArgs const& e)
{
	m_is_moving = false;
}

GUI_TextBox::GUI_TextBox() :GUI_Container(0, 0, 0, 0), m_scrollbar(this)
{
	start_render = m_items.begin();
	end_render = m_items.end();
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
	update();
	m_scrollbar.content_update();
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
				update();
				m_scrollbar.content_update();
				return;
			}
		}
		else 
		{
			Item = static_cast<GUI_Text*>(m_items.front());
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

void GUI_TextBox::set_scroll2(int dy)
{
	if (!m_items.empty())
	{
		//GUI_Text* Item;
		//if (dy < 0)
		//{
		//	Item = static_cast<GUI_Text*>(m_items.back());
		//	if (Item->m_position.y + Item->m_size.h + dy < m_size.h)
		//	{
		//		if (m_scroll.y != 0)
		//		{
		//			m_scroll.y = m_size.h - (Item->m_position.y + Item->m_size.h);
		//		}
		//		update();
		//		return;
		//	}
		//}
		//else
		//{
		//	Item = static_cast<GUI_Text*>(m_items.front());
		//	if (Item->m_position.y + m_scroll.y + dy > 0)
		//	{
		//		m_scroll.y = 0;
		//		update();
		//		m_scrollbar.content_update();
		//		return;
		//	}
		//}
		m_scroll.y = dy;
		update();
	}
}

void GUI_TextBox::update()
{
	for(auto i=m_items.begin();i!=m_items.end();++i)
	{
		if((*i)->m_position.y+ (*i)->m_size.h+ m_scroll.y>0)
		{
			start_render = i;
			for (auto j = i; j != m_items.end(); ++j)
			{
				if ((*j)->m_position.y + (*j)->m_size.h + m_scroll.y > m_size.h)
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


void GUI_TextBox::render(GraphicalController* Graph, int px, int py)
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

void GUI_TextBox::resize(int width, int height)
{
	m_size.w = width;
	m_size.h = height;
	m_scrollbar.m_position = position_t(width - 20, 2);
	m_scrollbar.resize(18, height-3);
}

void GUI_TextBox::on_mouse_wheel(MouseEventArgs const& e)
{
//#warning FIXME Что за магическое 30?
	set_scroll(e.value / 5);
}


void GUI_TextBox::on_mouse_click(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
	if (m_scrollbar.check_region(LocalMouseEventArgs))
	{
		m_scrollbar.mouse_click(LocalMouseEventArgs);
		return;
	}
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

void GUI_TextBox::on_mouse_down(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
	if (m_scrollbar.check_region(LocalMouseEventArgs))
	{
		m_scrollbar.mouse_down(LocalMouseEventArgs);
		return;
	}
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

void GUI_TextBox::on_mouse_move(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = MouseEventArgs(position_t(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value);
	m_scrollbar.mouse_move(LocalMouseEventArgs);
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

GUI_game_console::GUI_game_console(Game_log& log) :m_log(log) 
{
	m_styles.push_back(GUI_TextFormat(10, 19, RGBA_t(0.0, 0.8, 0.0, 1.0)));
	m_styles.push_back(GUI_TextFormat(10, 19, RGBA_t(0.3, 0.8, 0.8, 1.0)));
	m_styles.push_back(GUI_TextFormat(10, 19, RGBA_t(0.8, 0.0, 0.0, 1.0)));
	m_log.add_item += std::bind(&GUI_game_console::on_log_add_item, this,std::placeholders::_1);
};

void GUI_game_console::on_log_add_item(game_log_message_t& e)
{
	add_item_control(new GUI_Text(e.value,&m_styles[e.type]));
}