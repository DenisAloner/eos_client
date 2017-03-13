#include "GUI_TextBox.h"


GUI_TextBox::GUI_TextBox() :GUI_Container(0, 0, 0, 0)
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
					end_render = j;
					break;
				}
			}
			break;
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

void GUI_TextBox::on_mouse_wheel(MouseEventArgs const& e)
{
//#warning FIXME Что за магическое 30?
	set_scroll(e.value / 5);
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