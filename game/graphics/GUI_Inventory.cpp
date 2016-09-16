#include "GUI_Inventory.h"
#include "game/utils/log.h"

GUI_body_cell::GUI_body_cell(int width, int height, Object_part* item, GUI_Body* owner) : m_item(item), m_owner(owner)
{
	m_size.w = width;
	m_size.h = height;
}

void GUI_body_cell::render(GraphicalController* Graph, int px, int py)
{
	//glColor4d(1.0, 1.0, 1.0, 1.0);
	//glEnable(GL_BLEND);
	//glEnable(GL_TEXTURE_2D);
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glBindTexture(GL_TEXTURE_2D, Graph->Sprites[18]);
	//Graph->draw_sprite(px, py, px, py + height, px + width, py + height, px + width, py);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	if (focused)
	{
		glColor4d(1.0, 1.0, 1.0, 0.75);
		Graph->draw_sprite(px, py, px, py + m_size.h, px + m_size.w, py + m_size.h, px + m_size.w, py);
		glColor4d(0, 0, 0, 1);
	}
	else
	{
		glColor4f(1.0, 1.0, 1.0, 1.0);
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	Graph->center_text(px + 128, py + 32, m_item->m_name, 8, 17);
	glDisable(GL_BLEND);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2d(px, py);
	glVertex2d(px, py + m_size.h);
	glVertex2d(px, py + m_size.h);
	glVertex2d(px + m_size.w, py + m_size.h);
	glVertex2d(px + m_size.w, py + m_size.h);
	glVertex2d(px + m_size.w, py);
	glVertex2d(px + m_size.w, py);
	glVertex2d(px, py);
	glEnd();
	glBegin(GL_LINES);
	glVertex2d(px, py);
	glVertex2d(px, py + m_size.h);
	glVertex2d(px, py + m_size.h);
	glVertex2d(px + 64, py + m_size.h);
	glVertex2d(px + 64, py + m_size.h);
	glVertex2d(px + 64, py);
	glVertex2d(px + 64, py);
	glVertex2d(px, py);
	glEnd();
	if (m_item->m_item)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_icons[m_item->m_item->m_active_state->m_icon]);
		Graph->draw_sprite(px, py, px, py + m_size.h, px + 64, py + m_size.h, px + 64, py);
	}
}

void GUI_body_cell::on_mouse_move(MouseEventArgs const& e)
{
	set_focus(true);
}

void GUI_body_cell::on_mouse_down(MouseEventArgs const& e)
{
	set_focus(true);
	if (Application::instance().m_message_queue.m_reader)
	{
		Parameter* p = new Parameter(parameter_type_e::owner);
		(*p)[0].set(m_item);
		Application::instance().m_message_queue.push(p);
	}
}

void GUI_Body::get_part_predicat(Object_interaction* object)
{
	GUI_body_cell* item;
	if (object->m_interaction_message_type == interaction_message_type_e::part)
	{
		item = new GUI_body_cell(192, 64, static_cast<Object_part*>(object), this);
		add_item_control(item);
	}
}

void GUI_Body::update(Attribute_map* feature)
{
	while (m_items.begin() != m_items.end())
	{
		remove(*m_items.begin());
	}
	for (auto item = feature->m_item.begin(); item != feature->m_item.end(); item++)
	{
		item->second->do_predicat(std::bind(&GUI_Body::get_part_predicat, this, std::placeholders::_1));
	}
}

GUI_Body::GUI_Body(Attribute_map* feature) :GUI_Container(0, 0, 192, 4 * 64)
{
	m_already_active = false;
	update(feature);
}

void GUI_Body::add_item_control(GUI_Object* object)
{
	if (!m_items.empty())
	{
		GUI_body_cell* LastElement = static_cast<GUI_body_cell*>(m_items.back());
		object->m_position.x = 0;
		object->m_position.y = LastElement->m_position.y + LastElement->m_size.h;
		if (object->m_position.y + object->m_size.h>m_size.h)
		{
			m_scroll.y -= object->m_size.h;
		}
	}
	else
	{
		object->m_position.x = 0;
		object->m_position.y = 0;
	}
	GUI_Layer::add(object);
}

void GUI_Body::set_scroll(int dy)
{
	if (!m_items.empty())
	{
		GUI_body_cell* Item;
		if (dy < 0)
		{
			Item = static_cast<GUI_body_cell*>(m_items.back());
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
			Item = static_cast<GUI_body_cell*>(m_items.front());
			if (Item->m_position.y + m_scroll.y + dy > 0)
			{
				m_scroll.y = 0;
				return;
			}
		}
		m_scroll.y += dy;
	}
}

void GUI_Body::on_mouse_wheel(MouseEventArgs const& e)
{
	//#warning FIXME Что за магическое 30?
	set_scroll(e.value/30);
}

void GUI_Body::on_mouse_down(MouseEventArgs const& e)
{
	if (m_already_active) {
		GUI_Container::on_mouse_down(e);
	}
	else {
		m_already_active = true;
	}
}

void GUI_Body::set_focus(bool state)
{
	if (!state)
	{
		m_already_active = false;
	}
	GUI_Container::set_focus(state);
}