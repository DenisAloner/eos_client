#include "GUI_Layer.h"
#include "GraphicalController.h"

GUI_Layer::GUI_Layer(int x, int y, int width, int height)
{
    m_focus = nullptr;
    m_position.x = x;
    m_position.y = y;
    m_size.w = width;
    m_size.h = height;
}

GUI_Layer::~GUI_Layer(void)
{
}

MouseEventArgs GUI_Layer::set_local_mouse_position(MouseEventArgs const& source)
{
    return MouseEventArgs(position_t<int>(source.position.x - m_position.x, source.position.y - m_position.y), source.key, source.value);
}

void GUI_Layer::on_mouse_click(MouseEventArgs const& e)
{
    const auto local_mouse_event_args = set_local_mouse_position(e);
    for (auto& m_item : m_items) {
        if (m_item->check_region(local_mouse_event_args)) {
            m_item->mouse_click(local_mouse_event_args);
            return;
        }
    }
}

void GUI_Layer::on_mouse_down(MouseEventArgs const& e)
{
    const auto local_mouse_event_args = set_local_mouse_position(e);
    for (auto& m_item : m_items) {
        if (m_item->check_region(local_mouse_event_args)) {
            m_item->mouse_down(local_mouse_event_args);
            return;
        }
    }
}

void GUI_Layer::on_mouse_move(MouseEventArgs const& e)
{
    const auto local_mouse_event_args = set_local_mouse_position(e);
    if (m_focus) {
        m_focus->mouse_move(local_mouse_event_args);
    }
}

void GUI_Layer::on_mouse_start_drag(MouseEventArgs const& e)
{
    const auto local_mouse_event_args = set_local_mouse_position(e);
    if (m_focus) {
        m_focus->mouse_start_drag(local_mouse_event_args);
    }
}

void GUI_Layer::on_mouse_drag(MouseEventArgs const& e)
{
    const auto local_mouse_event_args = set_local_mouse_position(e);
    if (m_focus) {
        m_focus->mouse_drag(local_mouse_event_args);
    }
}

void GUI_Layer::on_mouse_end_drag(MouseEventArgs const& e)
{
    const auto local_mouse_event_args = set_local_mouse_position(e);
    if (m_focus) {
        m_focus->mouse_end_drag(local_mouse_event_args);
    }
}

void GUI_Layer::on_mouse_wheel(MouseEventArgs const& e)
{
    if (m_focus) {
        m_focus->mouse_wheel(e);
    }
}

void GUI_Layer::render(GraphicalController* graph, int px, int py)
{
    /*glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(0);
    for (auto current = m_items.rbegin(); current != m_items.rend(); ++current) {
        (*current)->render(graph, px + (*current)->m_position.x, py + (*current)->m_position.y);
    }
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);*/

    for (auto current = m_items.rbegin(); current != m_items.rend(); ++current) {
        (*current)->render(graph, px + (*current)->m_position.x, py + (*current)->m_position.y);
    }
}

void GUI_Layer::on_key_press(WPARAM w)
{
    if (m_focus) {
        m_focus->key_press(w);
    }
}

void GUI_Layer::set_focus(bool state)
{
    focused = state;
    if (state) {
        if (m_focus != nullptr) {
            m_focus->set_focus(false);
        }
        get_focus(this);
    } else {
        if (m_focus != nullptr) {
            m_focus->set_focus(false);
            m_focus = nullptr;
        }
        lose_focus(this);
    }
}

void GUI_Layer::add(GUI_Object* object)
{
    m_items.push_back(object);
    object->get_focus += std::bind(&GUI_Layer::on_item_get_focus, this, std::placeholders::_1);
    object->destroy += std::bind(&GUI_Layer::remove, this, std::placeholders::_1);
}

void GUI_Layer::add_front(GUI_Object* object)
{
    m_items.push_front(object);
    object->get_focus += std::bind(&GUI_Layer::on_item_get_focus, this, std::placeholders::_1);
    object->destroy += std::bind(&GUI_Layer::remove, this, std::placeholders::_1);
}

void GUI_Layer::remove(GUI_Object* object)
{
    if (m_focus == object) {
        m_focus = nullptr;
    }
    m_items.remove(object);
}

bool GUI_Layer::check_region(MouseEventArgs const& e)
{
    const auto local_mouse_event_args = set_local_mouse_position(e);
    if (this->m_position.x <= e.position.x && this->m_position.x + m_size.w >= e.position.x && this->m_position.y <= e.position.y && this->m_position.y + m_size.h >= e.position.y) {
        for (auto& m_item : m_items) {
            if (m_item->check_region(local_mouse_event_args)) {
                return true;
            }
        }
    }
    return false;
}

void GUI_Layer::on_get_focus(GUI_Object* sender)
{
}

void GUI_Layer::on_item_get_focus(GUI_Object* sender)
{
    if (m_focus != sender) {
        set_focus(true);
        m_focus = sender;
    }
}