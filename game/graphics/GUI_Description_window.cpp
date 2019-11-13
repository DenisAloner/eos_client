#include "GUI_Description_window.h"
#include <utility>

Visitor_object_description_getter::Visitor_object_description_getter()
    : m_value(nullptr)
{
}

void Visitor_object_description_getter::handle_simple(std::u16string& value)
{
    if (m_active.empty()) {
        m_value = new Tree<std::u16string>(value);
    } else {
        m_active.front()->add_node(value);
    }
}

void Visitor_object_description_getter::handle_simple(std::u16string const& value)
{
    if (m_active.empty()) {
        m_value = new Tree<std::u16string>(value);
    } else {
        m_active.front()->add_node(value);
    }
}

void Visitor_object_description_getter::handle_complex(std::u16string& value)
{
    if (m_active.empty()) {
        m_active.push_front(new Tree<std::u16string>(value));
    } else {
        const auto element = m_active.front()->add_node(value);
        m_active.push_front(element);
    }
    if (!m_value) {
        m_value = m_active.front();
    }
}

void Visitor_object_description_getter::handle_complex(std::u16string const& value)
{
    if (m_active.empty()) {
        m_active.push_front(new Tree<std::u16string>(value));
    } else {
        const auto element = m_active.front()->add_node(value);
        m_active.push_front(element);
    }
    if (!m_value) {
        m_value = m_active.front();
    }
}

void Visitor_object_description_getter::visit(Object_state& value)
{
    handle_complex(u"Характеристики: ");
    Tree<std::u16string>* item = nullptr;
    for (auto current = value.m_items.begin(); current != value.m_items.end(); ++current) {
        current->second->apply_visitor(*this);
        auto& node = m_active.front()->m_nodes;
        if (!node.empty() && item != &node.back()) {
            item = &node.back();
            node.back().m_value = Dictionaries::m_string_interaction_e[current->first] + u": " + node.back().m_value;
        }
    }
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(ObjectTag::Label& value)
{
    handle_simple(u"<" + Dictionaries::m_string_object_tag_e[value.m_type] + u">");
}

void Visitor_object_description_getter::visit(ObjectTag::Requirements_to_object& value)
{
    handle_complex(u"требования к экипировке: ");
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(ObjectTag::Mortal& value)
{
    handle_simple(u"<" + Dictionaries::m_string_object_tag_e[value.m_type] + u">");
}

void Visitor_object_description_getter::visit(Action_pick& value)
{
    handle_simple(u"<" + value.get_description(nullptr) + u">");
}

void Visitor_object_description_getter::visit(GameObject& value)
{
    handle_complex(value.m_name);
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Attribute_map& value)
{
    handle_complex(u"Характеристики: ");
    Tree<std::u16string>* item = nullptr;
    for (auto current = value.m_items.begin(); current != value.m_items.end(); ++current) {
        current->second->apply_visitor(*this);
        auto& node = m_active.front()->m_nodes;
        if (!node.empty() && item != &node.back()) {
            item = &node.back();
            node.back().m_value = Dictionaries::m_string_interaction_e[current->first] + u": " + node.back().m_value;
        }
    }
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Interaction_list& value)
{
    handle_complex(u"");
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Parameter_list& value)
{
    handle_complex(cp1251_to_utf16(std::to_string(value.m_value) + "(" + std::to_string(value.m_basic_value) + ")/" + std::to_string(value.m_limit) + "(" + std::to_string(value.m_basic_limit) + "):"));
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Vision_list& value)
{
    handle_complex(u"");
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Vision_component& value)
{
    handle_complex(cp1251_to_utf16(std::to_string(value.m_value.radius) + "," + std::to_string(value.m_value.start_angle) + "," + std::to_string(value.m_value.end_angle) + "/" + std::to_string(value.m_basic_value.radius) + "," + std::to_string(value.m_basic_value.start_angle) + "," + std::to_string(value.m_basic_value.end_angle)));
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Parts_list& value)
{
    handle_complex(u"");
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Object_part& value)
{
    handle_complex(value.m_name + u": ");
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Tag_list& value)
{
    handle_complex(u"");
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Action_list& value)
{
    handle_complex(u"");
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Instruction_check_tag& value)
{
    handle_simple(u"метка <" + Dictionaries::m_string_object_tag_e[value.m_value] + u">");
}

void Visitor_object_description_getter::visit(Instruction_slot_link& value)
{
    handle_complex(u"<тип параметра:" + Dictionaries::m_string_interaction_e[value.m_subtype] + u">:");
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Slot_set_state& value)
{
    handle_simple(u"изменить состояние");
}

void Visitor_object_description_getter::visit(Interaction_copyist& value)
{
    handle_complex(u"<тип параметра:" + Dictionaries::m_string_interaction_e[value.m_subtype] + u">:");
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Interaction_prefix& value)
{
    handle_complex(u"<тип эффекта:" + cp1251_to_utf16(Application::instance().m_game_object_manager->get_effect_prefix_string(value.m_subtype)) + u">:");
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Interaction_addon& value)
{
    handle_complex(u"<наложение эффекта:" + Dictionaries::m_string_interaction_e[value.m_subtype] + u">:");
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Interaction_time& value)
{
    handle_complex(u"<длительное наложение эффекта:" + cp1251_to_utf16(std::to_string(value.m_turn)) + u">:");
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Interaction_timer& value)
{
    handle_complex(u"<цикличное наложение эффекта:" + cp1251_to_utf16(std::to_string(value.m_turn) + "(" + std::to_string(value.m_period) + ")>:"));
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void Visitor_object_description_getter::visit(Effect& value)
{
    handle_simple(u":" + cp1251_to_utf16(std::to_string(value.m_value)));
}

void Visitor_object_description_getter::visit(Instruction_slot_parameter& value)
{
    handle_complex(u"Instruction_slot_parameter:");
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void GUI_Window::add(GUI_Object* object)
{
    object->m_position = position_t<int>(object->m_position.x + 1, object->m_position.y + m_header->m_position.y + m_header->m_size.h + 1);
    GUI_Layer::add(object);
}

GUI_Window::~GUI_Window()
{
}

void GUI_Window::on_mouse_start_drag(MouseEventArgs const& e)
{
    if (m_focus && m_focus != m_header) {
        const auto local_mouse_event_args = set_local_mouse_position(e);
        m_focus->mouse_start_drag(local_mouse_event_args);
    } else {
        m_initial_position.x = m_position.x - e.position.x;
        m_initial_position.y = m_position.y - e.position.y;
    }
}

void GUI_Window::on_mouse_drag(MouseEventArgs const& e)
{
    if (m_focus && m_focus != m_header) {
        const auto local_mouse_event_args = set_local_mouse_position(e);
        m_focus->mouse_drag(local_mouse_event_args);
    } else {
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

void GUI_Window::resize(int width, int height)
{
    m_size.w = width;
    m_size.h = height;
    m_header->resize(m_size.w - 4, 0);
}

void GUI_Window::on_mouse_wheel(MouseEventArgs const& e)
{
    if (m_focus) {
        m_focus->mouse_wheel(e);
    }
}

GUI_TreeElement::GUI_TreeElement(std::u16string text, GUI_TextFormat* format, int level)
    : level(level)
    , m_format(format)
    , m_text(std::move(text))
    , m_hide(false)
{
    m_size.h = Application::instance().m_graph->max_symbol_height_for_current_font();
    m_size.w = Application::instance().m_graph->measure_text_width(m_text) + 10 + 2;
}

void GUI_TreeElement::render(GraphicalController* graph, int px, int py)
{
    glEnable(GL_BLEND);
    glColor4d(m_format->m_color.r, m_format->m_color.g, m_format->m_color.b, m_format->m_color.a);
    glDisable(GL_TEXTURE_2D);
    switch (m_kind) {
    case kind_e::node: {
        rectangle_t<int> rect(px + 2, py + ((m_size.h - 11) / 2), 11, 11);
        graph->draw_rectangle(rect);
        if (m_hide) {
            rectangle_t<int> rect(px + 4, py + ((m_size.h - 6) / 2), 6, 6);
            graph->draw_sprite(rect);
        }
        switch (m_element_position) {
        case element_position_e::begin: {
            glBegin(GL_LINES);
            glVertex2d(px + 7, py);
            glVertex2d(px + 7, py + ((m_size.h - 11) / 2));
            glVertex2d(px + 7, py + ((m_size.h - 11) / 2) + 11);
            glVertex2d(px + 7, py + m_size.h + 2);
            glEnd();
            break;
        }
        case element_position_e::middle: {
            glBegin(GL_LINES);
            glVertex2d(px + 7, py);
            glVertex2d(px + 7, py + ((m_size.h - 11) / 2));
            glVertex2d(px + 7, py + ((m_size.h - 11) / 2) + 11);
            glVertex2d(px + 7, py + m_size.h + 2);
            glEnd();
            break;
        }
        case element_position_e::end: {
            glBegin(GL_LINES);
            glVertex2d(px + 7, py);
            glVertex2d(px + 7, py + ((m_size.h - 11) / 2));
            glEnd();
            break;
        }
        case element_position_e::single: {
            glBegin(GL_LINES);
            glVertex2d(px + 7, py);
            glVertex2d(px + 7, py + ((m_size.h - 11) / 2));
            glEnd();
            break;
        }
        }
        break;
    }
    case kind_e::element: {
        glBegin(GL_LINES);
        glVertex2d(px + 7, py + m_size.h / 2);
        glVertex2d(10 + 6 + px, py + m_size.h / 2);
        glEnd();
        switch (m_element_position) {
        case element_position_e::begin: {
            glBegin(GL_LINES);
            glVertex2d(px + 7, py);
            glVertex2d(px + 7, py + m_size.h + 2);
            glEnd();
            break;
        }
        case element_position_e::middle: {
            glBegin(GL_LINES);
            glVertex2d(px + 7, py);
            glVertex2d(px + 7, py + m_size.h + 2);
            glEnd();
            break;
        }
        case element_position_e::end: {
            glBegin(GL_LINES);
            glVertex2d(px + 7, py);
            glVertex2d(px + 7, py + m_size.h / 2);
            glEnd();
            break;
        }
        case element_position_e::single: {
            glBegin(GL_LINES);
            glVertex2d(px + 7, py);
            glVertex2d(px + 7, py + m_size.h / 2);
            glEnd();
            break;
        }
        }
    } break;
    }
    glDisable(GL_TEXTURE_2D);
    auto owner = m_owner;
    for (int i = 0; i < level; ++i) {
        if (owner->m_element_position == element_position_e::end || owner->m_element_position == element_position_e::single) {
            owner = owner->m_owner;
            continue;
        }
        glBegin(GL_LINES);
        glVertex2d(px - 9 - i * 16, py);
        glVertex2d(px - 9 - i * 16, py + m_size.h + 2);
        glEnd();
        owner = owner->m_owner;
    }
    glEnable(GL_TEXTURE_2D);
    graph->output_text(10 + 6 + px, py, m_text, m_format->m_symbol_size.w, m_format->m_symbol_size.h);
}

GUI_TreeView::GUI_TreeView(int x, int y, int width, int height, GameObject*& object)
    : GUI_Scrollable_container(x, y, width, height)
    , m_object(object)
{
}

void GUI_TreeView::on_mouse_click(MouseEventArgs const& e)
{
    auto local_mouse_event_args = MouseEventArgs(position_t<int>(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value);
    if (m_scrollbar.check_region(local_mouse_event_args)) {
        m_scrollbar.mouse_click(local_mouse_event_args);
        return;
    }
    local_mouse_event_args = set_local_mouse_position(e);
    for (auto current = m_items.begin(); current != m_items.end(); ++current) {
        if ((*current)->check_region(local_mouse_event_args)) {
            (*current)->mouse_click(local_mouse_event_args);
            {
                const auto temp = static_cast<GUI_TreeElement*>(*current);
                if (temp->m_kind == GUI_TreeElement::kind_e::node) {
                    temp->m_hide = !temp->m_hide;
                    static_cast<GUI_TreeElement*>(*m_items.begin())->m_element_position = GUI_TreeElement::element_position_e::single;
                    change_node();
                    update();
                    m_scrollbar.content_update();
                }
            }
            return;
        }
    }
    if ((m_focus != this) && (m_focus == nullptr)) {
        set_focus(true);
    }
}

void GUI_TreeView::update_info()
{
    Visitor_object_description_getter info;
    m_object->apply_visitor(info);
    m_text.clear();
    while (m_items.begin() != m_items.end()) {
        GUI_Layer::remove((*m_items.begin()));
    }
    add_tree(*info.m_value, 0, nullptr);
    bind_tree(*info.m_value, m_items.begin());
    static_cast<GUI_TreeElement*>(*m_items.begin())->m_element_position = GUI_TreeElement::element_position_e::single;
    change_node();
    update();
    m_scrollbar.content_update();
}

void GUI_TreeView::update()
{
    if (m_content_size.h + m_scroll.y < m_size.h) {
        m_scroll.y = m_size.h - m_content_size.h;
    }
    if (m_scroll.y > 0) {
        m_scroll.y = 0;
    }
    start_render = m_items.begin();
    end_render = m_items.end();
    for (auto i = m_items.begin(); i != m_items.end(); ++i) {
        auto temp = static_cast<GUI_TreeElement*>(*i);
        if (temp->m_position.y + temp->m_size.h + m_scroll.y + 2 > 0) {
            start_render = i;
            for (auto j = i; j != m_items.end(); ++j) {
                temp = static_cast<GUI_TreeElement*>(*j);
                if (temp->m_position.y + temp->m_size.h + 2 + m_scroll.y > m_size.h) {
                    if (temp->m_hide) {
                        end_render = temp->m_next;
                    } else
                        end_render = ++j;
                    return;
                }
                if (temp->m_hide) {
                    j = temp->m_next;
                    --j;
                }
                if (j == m_items.end()) {
                    break;
                }
            }
            return;
        }
        if (temp->m_hide) {
            i = temp->m_next;
            --i;
        }
        if (i == m_items.end()) {
            break;
        }
    }
}

void GUI_TreeView::change_node()
{
    GUI_TreeElement* current = nullptr;
    GUI_TreeElement* previous = nullptr;
    auto y = 2;
    for (auto i = m_items.begin(); i != m_items.end(); ++i) {
        current = static_cast<GUI_TreeElement*>(*i);
        current->m_position.y = y;
        y += current->m_size.h + 2;
        if (current->m_hide) {
            i = current->m_next;
            --i;
        }
        /*if (i == m_items.end())
		{
			break;
		}*/
    }
    m_content_size.h = y;
}

void GUI_TreeView::add_tree(Tree<std::u16string>& value, int level, GUI_TreeElement* owner)
{
    auto result = new GUI_TreeElement(value.m_value, new GUI_TextFormat(8, 17, RGBA_t(1.0, 1.0, 1.0, 1.0)), level);
    result->m_owner = owner;
    add_item_control(result);
    result->m_position.x += 16 * level;
    if (value.m_nodes.empty()) {
        result->m_kind = GUI_TreeElement::kind_e::element;
    } else {
        result->m_kind = GUI_TreeElement::kind_e::node;
        for (auto item = value.m_nodes.begin(); item != value.m_nodes.end(); ++item) {
            add_tree(*item, level + 1, result);
        }
    }
}

void GUI_TreeView::bind_tree(Tree<std::u16string>& value, std::list<GUI_Object*>::iterator& pos)
{
    auto result = static_cast<GUI_TreeElement*>(*pos);
    ++pos;
    if (!value.m_nodes.empty()) {
        for (auto item = value.m_nodes.begin(); item != value.m_nodes.end(); ++item) {
            auto temp = static_cast<GUI_TreeElement*>(*pos);
            if (item == value.m_nodes.begin()) {
                if (value.m_nodes.size() == 1) {
                    temp->m_element_position = GUI_TreeElement::element_position_e::single;
                } else {
                    temp->m_element_position = GUI_TreeElement::element_position_e::begin;
                }
            } else if (item == --value.m_nodes.end()) {
                temp->m_element_position = GUI_TreeElement::element_position_e::end;
            } else {
                temp->m_element_position = GUI_TreeElement::element_position_e::middle;
            }
            bind_tree(*item, pos);
        }
    }
    result->m_next = pos;
}

void GUI_TreeView::render(GraphicalController* graph, int px, int py)
{
   /* glEnable(GL_SCISSOR_TEST);
    if (graph->add_scissor(rectangle_t<float>(float(px), float(py), float(m_size.w), float(m_size.h)))) {
        glEnable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glColor4d(0.0, 0.0, 0.0, 0.5);
        const rectangle_t<int> rect(px, py, m_size.w, m_size.h);
        graph->draw_sprite(rect);
        glEnable(GL_TEXTURE_2D);
        glColor4d(1.0, 1.0, 1.0, 1.0);
        for (auto i = start_render; i != end_render; ++i) {
            auto temp = static_cast<GUI_TreeElement*>(*i);
            const auto element_px = px + temp->m_position.x + m_scroll.x;
            const auto element_py = py + temp->m_position.y + m_scroll.y;
            temp->render(graph, element_px, element_py);
            if (temp->m_hide) {
                i = temp->m_next;
                --i;
            }
            if (i == m_items.end()) {
                break;
            }
        }
        m_scrollbar.render(graph, px + m_scrollbar.m_position.x, py + m_scrollbar.m_position.y);
        graph->remove_scissor();
        if (m_border_visible) {
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            graph->draw_rectangle(rectangle_t<int>(px, py, m_size.w, m_size.h));
        }
    }*/
}

GUI_Description_window::GUI_Description_window(int x, int y, int width, int height, std::u16string name,gui_style_t& style, GameObject*& object)
    : GUI_Window(x, y, width, height, std::move(name),style)
    , m_object(object)
{
    m_textbox = new GUI_TreeView(0, 0, 0, 0, object);
    m_textbox->m_position.x = 2;
    m_textbox->m_position.y = 2;
    //m_items->resize(m_size.w - 4, m_size.h - 25 - 2);
    add(m_textbox);
    m_textbox->resize(m_size.w - 4, m_size.h - m_textbox->m_position.y - 2);
    GUI_Layer::add(m_textbox);
    update_info();
    object->event_update += std::bind(&GUI_Description_window::update_info, this);
}

void GUI_Description_window::update_info()
{
    m_textbox->update_info();
}