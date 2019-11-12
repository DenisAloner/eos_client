#include "GUI_Container.h"
#include "Application.h"

GUI_Container::GUI_Container(int x, int y, int width, int height, bool border_visible)
    : GUI_Layer(x, y, width, height)
{
    m_border_visible = border_visible;
}

GUI_Container::~GUI_Container()
{
}

bool GUI_Container::check_region(MouseEventArgs const& e)
{
    if (this->m_position.x <= e.position.x && this->m_position.x + m_size.w >= e.position.x && this->m_position.y <= e.position.y && this->m_position.y + m_size.h >= e.position.y) {
        return true;
    }
    return false;
}

void GUI_Container::on_mouse_click(MouseEventArgs const& e)
{
    GUI_Layer::on_mouse_click(e);
    if ((m_focus != this) && (m_focus == nullptr)) {
        set_focus(true);
    }
}

void GUI_Container::on_mouse_down(MouseEventArgs const& e)
{
    GUI_Layer::on_mouse_down(e);
    if ((m_focus != this) && (m_focus == nullptr)) {
        set_focus(true);
    }
}

void GUI_Container::on_mouse_move(MouseEventArgs const& e)
{
    const auto local_mouse_event_args = set_local_mouse_position(e);
    for (auto& m_item : m_items) {
        if (m_item->check_region(local_mouse_event_args)) {
            m_item->mouse_move(local_mouse_event_args);
            return;
        }
    }
}

void GUI_Container::render(GraphicalController* graph, int px, int py)
{
    if (graph->add_scissor(px, py, m_size.w, m_size.h)) {
        for (auto current : m_items) {
            (current)->render(graph, px + current->m_position.x + m_scroll.x, py + current->m_position.y + m_scroll.y);
        }
        graph->remove_scissor();
    }
}

void GUI_Container::on_mouse_wheel(MouseEventArgs const& e)
{
    //#warning FIXME Создать дефайн или лучше статическую константу для значения 30, обозвать понятным образом
    set_scroll(e.value / 30);
}

void GUI_Container::set_scroll(int dy)
{
    if (!m_items.empty()) {
        GUI_Object* item;
        if (dy < 0) {
            item = m_items.back();
            if (item->m_position.y + item->m_size.h + m_scroll.y + dy < m_size.h) {
                if (m_scroll.y != 0) {
                    m_scroll.y = m_size.h - (item->m_position.y + item->m_size.h);
                }
                return;
            }
        } else {
            item = m_items.front();
            if (item->m_position.y + m_scroll.y + dy > 0) {
                m_scroll.y = 0;
                return;
            }
        }
        m_scroll.y += dy;
    }
}

MouseEventArgs GUI_Container::set_local_mouse_position(MouseEventArgs const& source)
{
    return MouseEventArgs(position_t<int>(source.position.x - m_position.x - m_scroll.x, source.position.y - m_position.y - m_scroll.y), source.key, source.value);
}

void GUI_Container::resize(int width, int height)
{
    m_size.w = width;
    m_size.h = height;
}