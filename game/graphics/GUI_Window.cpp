#include "GUI_Window.h"
#include "Application.h"

GUI_Header::GUI_Header(int x, int y, int width, int height, std::u16string text) :GUI_Container(x, y, width, height)
{
	GUI_Text* m_text = new GUI_Text(text);
	m_text->m_position.x = 2;
	m_text->m_position.y = 2;
	m_size.h = m_text->m_size.h + m_text->m_position.y + 2;
	GUI_Layer::add(m_text);
	GUI_ItemButton* m_button = new GUI_ItemButton();
	m_button->m_position.x = m_size.w - (m_size.h - 8) - 4;
	m_button->m_position.y = 4;
	m_button->m_size.w = m_size.h - 8;
	m_button->m_size.h = m_size.h - 8;
	GUI_Layer::add_front(m_button);
	m_button->mouse_click += std::bind(&GUI_Header::on_close_button_click, this);
}

GUI_Header::~GUI_Header()
{
}

void GUI_Header::on_close_button_click()
{
	close(this);
}

void GUI_Header::resize(int width, int height)
{
	m_size.w = width;
	m_size.h = m_items.back()->m_size.h + m_items.back()->m_position.y + 2;
	m_items.front()->m_position.x = m_size.w - (m_size.h - 8) - 4;
	m_items.front()->m_position.y = 4;
	m_items.front()->m_size.w = m_size.h - 8;
	m_items.front()->m_size.h = m_size.h - 8;
}

GUI_Window::GUI_Window(int _x, int _y, int _width, int _height, std::u16string _Name) :GUI_Container(_x, _y, _width, _height)
{
	m_header = new GUI_Header(2, 2, m_size.w - 4, 0, _Name);
	m_header->close += std::bind(&GUI_Window::on_header_close, this);
	GUI_Layer::add(m_header);
	close += std::bind(&GUI_Window::on_close, this, std::placeholders::_1);
	Application::instance().m_window_manager->add_front(this);
}

rectangle_t GUI_Window::client_rect()
{
	return rectangle_t(1, m_header->m_position.y + m_header->m_size.h + 1, m_size.w - 2, m_size.h - (m_header->m_position.y + m_header->m_size.h + 1));
}

Visitor_object_description_getter::Visitor_object_description_getter() :m_value(nullptr)
{
}

void Visitor_object_description_getter::handle_simple(std::u16string& value)
{
	if (m_active.empty())
	{
		m_value = new Tree<std::u16string>(value);
	}
	else
	{
		m_active.front()->add_node(value);
	}
}

void Visitor_object_description_getter::handle_simple(std::u16string const& value)
{
	if (m_active.empty())
	{
		m_value = new Tree<std::u16string>(value);
	}
	else
	{
		m_active.front()->add_node(value);
	}
}

void Visitor_object_description_getter::handle_complex(std::u16string& value)
{
	if (m_active.empty())
	{
		m_active.push_front(new Tree<std::u16string>(value));
	}
	else
	{
		auto element = m_active.front()->add_node(value);
		m_active.push_front(element);
	}
	if (!m_value)
	{
		m_value = m_active.front();
	}
}

void Visitor_object_description_getter::handle_complex(std::u16string const& value)
{
	if (m_active.empty())
	{
		m_active.push_front(new Tree<std::u16string>(value));
	}
	else
	{
		auto element = m_active.front()->add_node(value);
		m_active.push_front(element);
	}
	if (!m_value)
	{
		m_value = m_active.front();
	}
}

void Visitor_object_description_getter::visit(Object_state& value)
{
	begin(value);
	Tree<std::u16string>* item = nullptr;
	for (auto current = value.m_items.begin(); current != value.m_items.end(); ++current)
	{
		current->second->apply_visitor(*this);
		std::list<Tree<std::u16string>>& node = m_active.front()->m_nodes;
		if (!node.empty() && item != &node.back())
		{
			item = &node.back();
			node.back().m_value = Parser::m_string_interaction_e[current->first] + u": " + node.back().m_value;
		}
	}
	end(value);
}

void Visitor_object_description_getter::visit(ObjectTag::Label& value)
{
	handle_simple(u"<" + Parser::m_string_object_tag_e[value.m_type] + u">");
}

void Visitor_object_description_getter::begin(GameObject& value)
{
	handle_complex(value.m_name);
}
	
void Visitor_object_description_getter::end(GameObject& value)
{
	m_active.pop_front();
}

void Visitor_object_description_getter::begin(Attribute_map& value)
{
	handle_complex(u"Характеристики: ");
}

void Visitor_object_description_getter::end(Attribute_map& value)
{
	m_active.pop_front();
}

void Visitor_object_description_getter::begin(Object_state& value)
{
	handle_complex(u"Характеристики: ");
}

void Visitor_object_description_getter::end(Object_state& value)
{
	m_active.pop_front();
}

void Visitor_object_description_getter::begin(Interaction_list& value)
{
	handle_complex(u"");
}

void Visitor_object_description_getter::end(Interaction_list& value)
{
	m_active.pop_front();
}

void Visitor_object_description_getter::begin(Parameter_list& value)
{
	handle_complex(Parser::CP866_to_UTF16(std::to_string(value.m_value) + "(" + std::to_string(value.m_basic_value) + ")/" + std::to_string(value.m_limit) + "(" + std::to_string(value.m_basic_limit) + "):"));
}

void Visitor_object_description_getter::end(Parameter_list& value)
{
	m_active.pop_front();
}

void Visitor_object_description_getter::begin(Vision_list& value)
{
	handle_complex(u"");
}

void Visitor_object_description_getter::end(Vision_list& value)
{
	m_active.pop_front();
}

void Visitor_object_description_getter::begin(Vision_component& value)
{
	handle_complex(Parser::CP866_to_UTF16(std::to_string(value.m_value.radius) + "," + std::to_string(value.m_value.start_angle) + "," + std::to_string(value.m_value.end_angle) + "/" + std::to_string(value.m_basic_value.radius) + "," + std::to_string(value.m_basic_value.start_angle) + "," + std::to_string(value.m_basic_value.end_angle)));
}

void Visitor_object_description_getter::end(Vision_component& value)
{
	m_active.pop_front();
}

void Visitor_object_description_getter::begin(Parts_list& value)
{
	handle_complex(u"");
}

void Visitor_object_description_getter::end(Parts_list& value)
{
	m_active.pop_front();
}

void Visitor_object_description_getter::begin(Object_part& value)
{
	handle_complex(value.m_name+u": ");
}

void Visitor_object_description_getter::end(Object_part& value)
{
	m_active.pop_front();
}

void Visitor_object_description_getter::begin(Tag_list& value)
{
	handle_complex(u"");
}

void Visitor_object_description_getter::end(Tag_list& value)
{
	m_active.pop_front();
}

void GUI_Window::add(GUI_Object* object)
{
	object->m_position = position_t(object->m_position.x + 1, object->m_position.y + m_header->m_position.y + m_header->m_size.h + 1);
	GUI_Layer::add(object);
}


GUI_Window::~GUI_Window()
{
}

void GUI_Window::on_mouse_start_drag(MouseEventArgs const& e)
{
	if (m_focus&&m_focus!=m_header)
	{
		MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
		m_focus->mouse_start_drag(LocalMouseEventArgs);
	}
	else
	{
		m_initial_position.x = m_position.x - e.position.x;
		m_initial_position.y = m_position.y - e.position.y;
	}

}

void GUI_Window::on_mouse_drag(MouseEventArgs const& e)
{
	if (m_focus&&m_focus != m_header)
	{
		MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
		m_focus->mouse_drag(LocalMouseEventArgs);
	}
	else
	{
		m_position.x = m_initial_position.x + e.position.x;
		m_position.y = m_initial_position.y + e.position.y;
	}
}

void GUI_Window::on_mouse_end_drag(MouseEventArgs const& e)
{
}

void GUI_Window::on_close(GUI_Object* e)
{
	destroy(this);
}

void GUI_Window::on_header_close()
{
	close(this);
}

void GUI_Window::resize(int _width, int _height)
{
	m_size.w = _width;
	m_size.h = _height;
	m_header->resize(m_size.w - 4, 0);
}

void GUI_Window::on_mouse_wheel(MouseEventArgs const& e)
{
	if (m_focus)
	{
		m_focus->mouse_wheel(e);
	}
}

GUI_description_window::GUI_description_window(int x, int y, int width, int height, std::u16string Name, GameObject*& object) :GUI_Window(x, y, width, height, Name), m_object(object)
{
	m_textbox = new GUI_TextBox();
	m_textbox->m_position.x = 2;
	m_textbox->m_position.y = 2;
	//m_items->resize(m_size.w - 4, m_size.h - 25 - 2);
	add(m_textbox);
	m_textbox->resize(m_size.w - 4, m_size.h - m_textbox->m_position.y - 2);
	GUI_Layer::add(m_textbox);
	update_info();
	object->event_update += std::bind(&GUI_description_window::update_info, this);
}

void GUI_description_window::update_info()
{
	Visitor_object_description_getter info;
	m_object->apply_visitor(info);
	m_text.clear();
	while (m_textbox->m_items.begin()!= m_textbox->m_items.end())
	{
		m_textbox->GUI_Layer::remove((*m_textbox->m_items.begin()));
	}
	add_tree(*info.m_value, 0);
	/*if (m_object->m_active_state)
	{
		m_text.push_back(u"эффекты:");
		for (auto current = m_object->m_active_state->m_items.begin(); current != m_object->m_active_state->m_items.end(); current++)
		{
			m_text.push_back(u"." + Parser::m_string_interaction_e[current->first] + u":");
			current->second->description(&m_text,2);
		}
		if (m_object->m_active_state->m_state == object_state_e::equip)
		{
			Object_state_equip* obj = static_cast<Object_state_equip*>(m_object->m_active_state);
			for (auto current = obj->m_equip.m_items.begin(); current != obj->m_equip.m_items.end(); current++)
			{
				m_text.push_back(u"." + Parser::m_string_interaction_e[current->first] + u":");
				current->second->description(&m_text, 2);
			}
		}
	}
	for (auto item = m_text.begin(); item != m_text.end(); ++item)
	{
		m_textbox->add_item_control(new GUI_Text((*item), new GUI_TextFormat(8, 17, RGBA_t(0.7, 0.9, 1.0, 1.0))));
	}*/

}

void GUI_description_window::add_tree(Tree<std::u16string>& value, int level)
{
	m_textbox->add_item_control(new GUI_Text(std::u16string(level, '.') + value.m_value, new GUI_TextFormat(8, 17, RGBA_t(1.0, 1.0, 1.0, 1.0))));
	for (auto item = value.m_nodes.begin(); item != value.m_nodes.end(); ++item)
	{
		add_tree(*item, level + 1);
	}
}

GUI_body_window::GUI_body_window(int x, int y, int width, int height, std::u16string Name, GameObject*& object) :GUI_Window(x, y, width, height, Name), m_object(object)
{
	m_item = new GUI_Body(m_object->m_active_state);
	m_item->m_position.x = 1;
	m_item->m_position.y = 1;
	//m_items->resize(m_size.w - 4, m_size.h - 25 - 2);
	add(m_item);
	update_info();
	object->event_update += std::bind(&GUI_body_window::update_info, this);
}

void GUI_body_window::update_info()
{
	m_item->update(m_object->m_active_state);
	resize(m_item->m_size.w+4, m_item->m_position.y + m_item->m_size.h + 2);
}