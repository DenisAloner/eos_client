#include "GUI_Inventory.h"

GUI_Part_slot::GUI_Part_slot(int width, int height, ObjectPart* item, GUI_Body* owner)
    : m_item(item)
    , m_owner(owner)
{
    m_size.w = width;
    m_size.h = height;
    m_name = u"";
}

void GUI_Part_slot::render(GraphicalController* graph, int px, int py)
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (focused) {
        glColor4d(1.0, 1.0, 1.0, 0.75);
        const rectangle_t<int> rect(px, py, m_size.w, m_size.h);
        graph->draw_sprite(rect);
        glColor4d(0, 0, 0, 1);
    } else {
        glColor4f(1.0, 1.0, 1.0, 1.0);
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    graph->center_text(px + m_size.w / 2 + 32, py + 32, m_name, 8, 17);
    glDisable(GL_BLEND);
    glColor4f(1.0, 1.0, 1.0, 1.0);

    graph->draw_rectangle(rectangle_t<int>(px, py, m_size.w, m_size.h));
    graph->draw_rectangle(rectangle_t<int>(px, py, 64, m_size.h));

    if (m_item->m_item) {
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, m_item->m_item->m_active_state->m_icon->m_value);
        rectangle_t<int> rect(px, py, 64, m_size.h);
        graph->draw_sprite(rect);
    }
}

void GUI_Part_slot::on_mouse_move(MouseEventArgs const& e)
{
    set_focus(true);
}

void GUI_Part_slot::on_mouse_down(MouseEventArgs const& e)
{
    set_focus(true);
    if (Application::instance().m_message_queue.m_reader) {
        const auto p = new Parameter(parameter_type_e::owner);
        (*p)[0].set(m_item);
        Application::instance().m_message_queue.push(p);
    }
}

Visitor_container_hierarchy_getter::Visitor_container_hierarchy_getter(GUI_Body* owner)
    : m_owner(owner)
{
}

void Visitor_container_hierarchy_getter::visit(ObjectPart& value)
{
    m_active.push_front(value.m_name);
    std::u16string name = u"";
    for (auto i = m_active.begin(); i != m_active.end(); ++i) {
        if (name.empty()) {
            name += (*i);
        } else {
            name += u" < " + (*i);
        }
    }
    auto s = Application::instance().m_graph->measure_text_width(name);
    auto* gui_item = new GUI_Part_slot(0, 64, &value, m_owner);
    gui_item->m_name = name;
    m_result.push_back(gui_item);
    Visitor_simple::visit(value);
    m_active.pop_front();
}

void GUI_Body::update(AttributeMap* feature)
{
    while (m_items.begin() != m_items.end()) {
        remove(*m_items.begin());
    }
    m_max_item_name = 0;
    for (auto item = feature->m_items.begin(); item != feature->m_items.end(); ++item) {
        if (item->first == interaction_e::body) {
            Visitor_container_hierarchy_getter getter(this);
            item->second->apply_visitor(getter);
            for (auto i = getter.m_result.begin(); i != getter.m_result.end(); ++i) {
	            const auto s = Application::instance().m_graph->measure_text_width((*i)->m_name);
                if (s > m_max_item_name) {
                    m_max_item_name = s;
                }
                add_item_control(*i);
            }
        }
    }
    m_max_item_name += 80;
    for (auto item = m_items.begin(); item != m_items.end(); ++item) {
        (*item)->resize(m_max_item_name, 64);
    }
    const auto last_element = m_items.back();
    auto h = last_element->m_position.y + last_element->m_size.h + 2;
    if (h > Application::instance().m_gui_controller.m_size.h - 100) {
        h = Application::instance().m_gui_controller.m_size.h - 100;
    }
    resize(m_max_item_name + 2 + 20, h);
}

GUI_Body::GUI_Body(AttributeMap* feature)
    : GUI_Scrollable_container(0, 0, 0, 0)
{
    m_already_active = false;
    update(feature);
}

void GUI_Body::on_mouse_down(MouseEventArgs const& e)
{
    if (m_already_active) {
        GUI_Scrollable_container::on_mouse_down(e);
    } else {
        m_already_active = true;
    }
}

void GUI_Body::set_focus(bool state)
{
    if (!state) {
        m_already_active = false;
    }
    GUI_Scrollable_container::set_focus(state);
}