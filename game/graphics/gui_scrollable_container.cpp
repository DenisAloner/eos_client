#include "gui_scrollable_container.h"
#include "Application.h"

GUI_Scrollable_container::GUI_Scrollable_container(int x, int y, int width, int height, bool border_visible)
    : GUI_Container(x, y, width, height)
    , m_scrollbar(this)
{
    m_scrollbar.get_focus += std::bind(&GUI_Scrollable_container::on_item_get_focus, this, std::placeholders::_1);
    start_render = m_items.begin();
    end_render = m_items.end();

	auto& style = Application::instance().m_graph->gui_styles[u"default"];
    auto& scroll_y_head_tile = *style.scroll_y_head_tile;
    auto& border_y_tile = *style.border_y_tile;
	
    m_scrollbar.m_position = position_t<int>(width - scroll_y_head_tile.texture.w - border_y_tile.texture.w, 0);
    m_scrollbar.resize(scroll_y_head_tile.texture.w, height);
}

void GUI_Scrollable_container::add_item_control(GUI_Object* object)
{
    if (!m_items.empty()) {
        const auto last_element = m_items.back();
        object->m_position.x = 2;
        object->m_position.y = last_element->m_position.y + last_element->m_size.h + 2;
        if (object->m_position.y + object->m_size.h + m_scroll.y + 2 > m_size.h) {
            m_scroll.y = (m_size.h - object->m_position.y - object->m_size.h - 2);
        }
        m_content_size.h = object->m_position.y + object->m_size.h + 2;
    } else {
        object->m_position.x = 2;
        object->m_position.y = 2;
        m_scroll.y = 0;
        m_content_size.h = 2 + object->m_size.h + 2;
    }
    GUI_Layer::add(object);
    update();
    m_scrollbar.content_update();
}

void GUI_Scrollable_container::set_scroll(int dy)
{
    if (!m_items.empty()) {
        if (dy < 0) {
            if (m_content_size.h + m_scroll.y + dy < m_size.h) {
                m_scroll.y = m_size.h - m_content_size.h;
                if (m_scroll.y > 0) {
                    m_scroll.y = 0;
                }
                update();
                m_scrollbar.content_update();
                return;
            }
        } else {
            const auto item = m_items.front();
            if (item->m_position.y + m_scroll.y + dy > 0) {
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

void GUI_Scrollable_container::set_scroll2(int dy)
{
    if (!m_items.empty()) {
        if (dy < 0) {
            if (m_content_size.h + dy < m_size.h) {
                m_scroll.y = m_size.h - m_content_size.h;
                if (m_scroll.y > 0) {
                    m_scroll.y = 0;
                }
                update();
                return;
            }
        } else {
            const auto item = m_items.front();
            if (item->m_position.y + dy > 0) {
                m_scroll.y = 0;
                update();
                return;
            }
        }
        m_scroll.y = dy;
        update();
    }
}

void GUI_Scrollable_container::update()
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
        if ((*i)->m_position.y + (*i)->m_size.h + m_scroll.y + 2 > 0) {
            start_render = i;
            for (auto j = i; j != m_items.end(); ++j) {
                if ((*j)->m_position.y + (*j)->m_size.h + m_scroll.y + 2 > m_size.h) {
                    end_render = ++j;
                    return;
                }
            }
            end_render = m_items.end();
            return;
        }
    }
}

void GUI_Scrollable_container::render(GraphicalController* graph, int px, int py)
{
    if (graph->add_scissor(px, py, m_size.w, m_size.h)) {
        graph->render_background(px, py, m_size.w, m_size.h, graph->gui_styles[u"default"]);
        for (auto i = start_render; i != end_render; ++i) {
            (*i)->render(graph, px + (*i)->m_position.x + m_scroll.x, py + (*i)->m_position.y + m_scroll.y);
        }
        m_scrollbar.render(graph, px + m_scrollbar.m_position.x, py + m_scrollbar.m_position.y);
        graph->render_border(px, py, m_size.w, m_size.h, graph->gui_styles[u"default"]);
        graph->remove_scissor();
    }
}

void GUI_Scrollable_container::resize(int width, int height)
{
    m_size.w = width;
    m_size.h = height;
    auto& style = Application::instance().m_graph->gui_styles[u"default"];
    auto& scroll_y_head_tile = *style.scroll_y_head_tile;
    auto& border_y_tile = *style.border_y_tile;
    m_scrollbar.m_position = position_t<int>(width - scroll_y_head_tile.texture.w - border_y_tile.texture.w, 0);
    m_scrollbar.resize(scroll_y_head_tile.texture.w, height);
}

void GUI_Scrollable_container::on_mouse_wheel(MouseEventArgs const& e)
{
    //#warning FIXME Что за магическое 30?
    set_scroll(e.value / 5);
}

void GUI_Scrollable_container::on_mouse_click(MouseEventArgs const& e)
{
    auto local_mouse_event_args = MouseEventArgs(position_t<int>(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value);
    if (m_scrollbar.check_region(local_mouse_event_args)) {
        m_scrollbar.mouse_click(local_mouse_event_args);
        return;
    }
    local_mouse_event_args = set_local_mouse_position(e);
    for (auto& m_item : m_items) {
        if (m_item->check_region(local_mouse_event_args)) {
            m_item->mouse_click(local_mouse_event_args);
            return;
        }
    }
    if ((m_focus != this) && (m_focus == nullptr)) {
        set_focus(true);
    }
}

void GUI_Scrollable_container::on_mouse_down(MouseEventArgs const& e)
{
    auto local_mouse_event_args = MouseEventArgs(position_t<int>(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value);
    if (m_scrollbar.check_region(local_mouse_event_args)) {
        m_scrollbar.mouse_down(local_mouse_event_args);
        return;
    }
    local_mouse_event_args = set_local_mouse_position(e);
    for (auto& m_item : m_items) {
        if (m_item->check_region(local_mouse_event_args)) {
            m_item->mouse_down(local_mouse_event_args);
            return;
        }
    }
    if ((m_focus != this) && (m_focus == nullptr)) {
        set_focus(true);
    }
}

void GUI_Scrollable_container::on_mouse_start_drag(MouseEventArgs const& e)
{
    if (m_focus) {
        if (m_focus == &m_scrollbar) {
            const auto local_mouse_event_args = MouseEventArgs(position_t<int>(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value);
            m_focus->mouse_start_drag(local_mouse_event_args);
        } else {
            const auto local_mouse_event_args = set_local_mouse_position(e);
            m_focus->mouse_start_drag(local_mouse_event_args);
        }
    }
}

void GUI_Scrollable_container::on_mouse_drag(MouseEventArgs const& e)
{
    if (m_focus) {
        if (m_focus == &m_scrollbar) {
            const auto local_mouse_event_args = MouseEventArgs(position_t<int>(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value);
            m_focus->mouse_drag(local_mouse_event_args);
        } else {
            const auto local_mouse_event_args = set_local_mouse_position(e);
            m_focus->mouse_drag(local_mouse_event_args);
        }
    }
}

void GUI_Scrollable_container::on_mouse_move(MouseEventArgs const& e)
{
    auto local_mouse_event_args = MouseEventArgs(position_t<int>(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value);
    if (m_scrollbar.check_region(local_mouse_event_args)) {
        m_scrollbar.mouse_move(local_mouse_event_args);
        return;
    }
    local_mouse_event_args = set_local_mouse_position(e);
    for (auto& m_item : m_items) {
        if (m_item->check_region(local_mouse_event_args)) {
            m_item->mouse_move(local_mouse_event_args);
            return;
        }
    }
}