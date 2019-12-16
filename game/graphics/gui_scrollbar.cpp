#include "gui_scrollbar.h"
#include "GraphicalController.h"

GUI_Scrollbar_vertical::GUI_Scrollbar_vertical(GUI_Scrollable_container* owner)
    : m_owner(owner)
{
}

void GUI_Scrollbar_vertical::render(GraphicalController* graph, int px, int py)
{
    // auto& quad = graph->get_gui_quad(px, py, m_size.w, m_size.h);
    auto& style = graph->gui_styles[u"default"];

    auto& head_tile = *style.scroll_y_head_tile;
    auto& body_tile = *style.scroll_y_body_tile;

    if (graph->add_scissor(px, py + m_bar_top + head_tile.texture.h, m_size.w, m_bar_height - head_tile.texture.h * 2)) {
        for (auto i = py + m_bar_top + head_tile.texture.h; i < py + m_bar_top + m_bar_height - head_tile.texture.h; i += body_tile.texture.h) {
            graph->get_gui_quad(px, i, body_tile.texture.w, body_tile.texture.h, body_tile);
        }
        graph->remove_scissor();
    }

    graph->get_gui_quad(px, py + m_bar_top, m_size.w, head_tile.texture.h, head_tile);
    graph->get_gui_quad(px, py + m_bar_top + m_bar_height - head_tile.texture.h, m_size.w, head_tile.texture.h, head_tile, tile_options_e::FLIP_Y);
}

void GUI_Scrollbar_vertical::content_update()
{
    if (m_owner->m_items.empty()) {
        m_bar_top = 0;
        m_bar_height = m_size.h;
    } else {

		m_bar_height = float(m_owner->m_size.h) / float(m_owner->m_content_size.h) * float(m_size.h);
        if (m_bar_height < 64) {
                    m_bar_height = 64;
            m_bar_top = m_size.h - 64;
        } else {
            m_bar_top = std::abs(float(m_owner->m_scroll.y) / float(m_owner->m_content_size.h) * float(m_size.h));
        }
    }
}

MouseEventArgs GUI_Scrollbar_vertical::set_local_mouse_control(MouseEventArgs const& source)
{
    return MouseEventArgs(position_t<int>(source.position.x - m_position.x, source.position.y - m_position.y), source.key, source.value);
}

void GUI_Scrollbar_vertical::set_scroll_top(int value)
{
    m_bar_top = value;
    if (m_bar_top < 0) {
        m_bar_top = 0;
    }
    if (m_bar_top + m_bar_height > m_size.h) {
        m_bar_top = m_size.h - m_bar_height;
    }
    //GUI_Object& LastElement = *m_owner->m_items.back();
    //int total = LastElement.m_position.y + LastElement.m_size.h + 2;
    m_owner->set_scroll2(-static_cast<float>(m_bar_top) / static_cast<float>(m_size.h) * m_owner->m_content_size.h);
}

void GUI_Scrollbar_vertical::on_mouse_click(MouseEventArgs const& e)
{
    const auto local_mouse_event_args = set_local_mouse_control(e);
    if (local_mouse_event_args.position.y < m_bar_top) {
        set_scroll_top(local_mouse_event_args.position.y);
    } else if (local_mouse_event_args.position.y > m_bar_top + m_bar_height) {
        set_scroll_top(local_mouse_event_args.position.y - m_bar_height);
    }
    set_focus(true);
}

void GUI_Scrollbar_vertical::on_mouse_start_drag(MouseEventArgs const& e)
{
    const auto local_mouse_event_args = set_local_mouse_control(e);
    m_pos = local_mouse_event_args.position.y;
}

void GUI_Scrollbar_vertical::on_mouse_drag(MouseEventArgs const& e)
{
    const auto local_mouse_event_args = set_local_mouse_control(e);
    set_scroll_top(m_bar_top - m_pos + local_mouse_event_args.position.y);
    m_pos = local_mouse_event_args.position.y;
}

bool GUI_Scrollbar_vertical::check_region(MouseEventArgs const& e)
{
    if (this->m_position.x <= e.position.x && this->m_position.x + m_size.w >= e.position.x && this->m_position.y <= e.position.y && this->m_position.y + m_size.h >= e.position.y) {
        return true;
    }
    return false;
}