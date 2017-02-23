#include "GUI_Inventory.h"
#include "game/utils/log.h"

GUI_Part_slot::GUI_Part_slot(int width, int height, Object_part* item, GUI_Body* owner) : m_item(item), m_owner(owner)
{
	m_size.w = width;
	m_size.h = height;
	m_name = "";
}

void GUI_Part_slot::render(GraphicalController* Graph, int px, int py)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (focused)
	{
		glColor4d(1.0, 1.0, 1.0, 0.75);
		GraphicalController::rectangle_t rect(px, py, m_size.w, m_size.h);
		Graph->draw_sprite(rect);
		glColor4d(0, 0, 0, 1);
	}
	else
	{
		glColor4f(1.0, 1.0, 1.0, 1.0);
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	Graph->center_text(px + m_size.w/2+32, py + 32, m_name, 8, 17);
	glDisable(GL_BLEND);
	glColor4f(1.0, 1.0, 1.0, 1.0);

	Graph->draw_rectangle(GraphicalController::rectangle_t(px, py, m_size.w, m_size.h));
	Graph->draw_rectangle(GraphicalController::rectangle_t(px, py, 64, m_size.h));

	if (m_item->m_item)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_icons[m_item->m_item->m_active_state->m_icon]);
		GraphicalController::rectangle_t rect(px, py, 64, m_size.h);
		Graph->draw_sprite(rect);
	}
}

void GUI_Part_slot::on_mouse_move(MouseEventArgs const& e)
{
	set_focus(true);
}

void GUI_Part_slot::on_mouse_down(MouseEventArgs const& e)
{
	set_focus(true);
	if (Application::instance().m_message_queue.m_reader)
	{
		Parameter* p = new Parameter(parameter_type_e::owner);
		(*p)[0].set(m_item);
		Application::instance().m_message_queue.push(p);
	}
}

//void GUI_body_cell::add_item_control(GUI_Object* object)
//{
//	if (!m_items.empty())
//	{
//		GUI_Object* LastElement = m_items.back();
//		object->m_position.x = 2;
//		object->m_position.y = LastElement->m_position.y + LastElement->m_size.h;
//		/*if (object->m_position.y + object->m_size.h>m_size.h)
//		{
//			m_scroll.y -= object->m_size.h;
//		}*/
//	}
//	else
//	{
//		object->m_position.x = 2;
//		object->m_position.y = 30;
//	}
//	GUI_Layer::add(object);
//	GUI_Object* LastElement = m_items.back();
//	resize(m_size.w, LastElement->m_position.y + LastElement->m_size.h+1);
//}
//
//GUI_body_cell::GUI_body_cell(int width, int height, Object_part* item, GUI_Body* owner) : GUI_Container(0, 0, width, 0,false),m_items(item), m_owner(owner)
//{
//	m_size.w = width;
//	m_size.h = height;
//	//Object_part* part = m_items;
//	//for (auto slot = part->m_items.begin(); slot != part->m_items.end(); ++slot)
//	//{
//	//	GUI_Part_slot* gui_slot = new GUI_Part_slot(width - 4, 64, (*slot), this);
//	//	add_item_control(gui_slot);
//	//}
//	GUI_Part_slot* gui_slot = new GUI_Part_slot(width - 4, 64, item, this);
//	add_item_control(gui_slot);
//}
//
//void GUI_body_cell::render(GraphicalController* Graph, int px, int py)
//{
//	GUI_Container::render(Graph, px, py);
//	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	Graph->center_text(px + m_size.w/2, py + 12, m_items->m_name, 8, 17);
//}

//void GUI_body_cell::on_mouse_down(MouseEventArgs const& e)
//{
//
//		GUI_Container::on_mouse_down(e);
//
//}
//
//void GUI_body_cell::set_focus(bool state)
//{
//	if (!state)
//	{
//		m_already_active = false;
//	}
//	GUI_Container::set_focus(state);
//}

void GUI_Body::get_part_predicat(Object_interaction* object,bool add_mode)
{
	if (add_mode)
	{
		if (object->m_interaction_message_type == interaction_message_type_e::part)
		{
			
			Object_part* part = dynamic_cast<Object_part*>(object);
			LOG(INFO) << "parts: " << part->m_name;
			m_owner_name.push_front(&part->m_name);
			std::string name = "";
			for (auto i = m_owner_name.begin(); i != m_owner_name.end(); i++)
			{
				if(name == "")
				{
					name += *(*i); 
				}
				else
				{
					name += " < " + *(*i);
				}
			}
			std::size_t s = Application::instance().m_graph->measure_text_width(name);
			if (s > m_max_item_name) { m_max_item_name = s; }
			GUI_Part_slot* gui_item = new GUI_Part_slot(0, 64, part, this);
			gui_item->m_name = name;
			add_item_control(gui_item);
		}
	}
	else
	{
		if (object->m_interaction_message_type == interaction_message_type_e::part)
		{
			m_owner_name.pop_front();
		}
	}
}

void GUI_Body::update(Attribute_map* feature)
{
	while (m_items.begin() != m_items.end())
	{
		remove(*m_items.begin());
	}
	m_max_item_name = 0;
	for (auto item = feature->m_items.begin(); item != feature->m_items.end(); ++item)
	{
		if (item->first == interaction_e::body) 
		{
			item->second->do_predicat_ex(std::bind(&GUI_Body::get_part_predicat, this, std::placeholders::_1, std::placeholders::_2));
		}
	}
	m_max_item_name += 80;
	for (auto item = m_items.begin(); item != m_items.end(); ++item)
	{
		(*item)->resize(m_max_item_name, 64);
	}
	GUI_Object* LastElement = m_items.back();
	int h = LastElement->m_position.y + LastElement->m_size.h + 2;
	if (h > Application::instance().m_GUI->m_main_layer->m_size.h-50)
	{
		h = Application::instance().m_GUI->m_main_layer->m_size.h - 50;
	}
	resize(LastElement->m_size.w+4, h);
}

GUI_Body::GUI_Body(Attribute_map* feature) :GUI_Container(0, 0, 0, 0)
{
	m_already_active = false;
	update(feature);
}

void GUI_Body::add_item_control(GUI_Object* object)
{
	if (!m_items.empty())
	{
		GUI_Object* LastElement = m_items.back();
		object->m_position.x = 2;
		object->m_position.y = LastElement->m_position.y + LastElement->m_size.h+2;
		/*if (object->m_position.y + object->m_size.h>m_size.h)
		{
			m_scroll.y -= object->m_size.h;
		}*/
	}
	else
	{
		object->m_position.x = 2;
		object->m_position.y = 2;
	}
	GUI_Container::add(object);
	GUI_Object* LastElement = m_items.back();
	resize(m_size.w, LastElement->m_position.y + LastElement->m_size.h+2);
}

void GUI_Body::set_scroll(int dy)
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

void GUI_Body::on_mouse_wheel(MouseEventArgs const& e)
{
	//#warning FIXME Что за магическое 30?
	set_scroll(e.value/5);
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