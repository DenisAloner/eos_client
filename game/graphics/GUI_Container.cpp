#include "GUI_Container.h"

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
    /*char buf2[32];
	itoa(SetLocalMousePosition(e).Position.x, buf2, 10);
	MessageBox(NULL, buf2, "eos", MB_OK);*/
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

GUI_Scrollbar_vertical::GUI_Scrollbar_vertical(GUI_Scrollable_container* owner)
    : m_owner(owner)
{
}

void GUI_Scrollbar_vertical::render(GraphicalController* graph, int px, int py)
{
    auto& quad = graph->get_gui_quad(px, py, m_size.w, m_size.h);
    quad = graph->get_gui_quad(px, py + m_bar_top, m_size.w, m_bar_height);
}

void GUI_Scrollbar_vertical::content_update()
{
    if (m_owner->m_items.empty()) {
        m_bar_top = 0;
        m_bar_height = m_size.h;
    } else {
        m_bar_top = std::abs(m_owner->m_scroll.y / static_cast<float>(m_owner->m_content_size.h) * m_size.h);
        m_bar_height = m_owner->m_size.h / static_cast<float>(m_owner->m_content_size.h) * m_size.h;
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

GUI_Scrollable_container::GUI_Scrollable_container(int x, int y, int width, int height, bool border_visible)
    : GUI_Container(x, y, width, height)
    , m_scrollbar(this)
{
    m_scrollbar.get_focus += std::bind(&GUI_Scrollable_container::on_item_get_focus, this, std::placeholders::_1);
    start_render = m_items.begin();
    end_render = m_items.end();
    m_scrollbar.m_position = position_t<int>(width - 20, 2);
    m_scrollbar.resize(18, height - 3);
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
    	
        auto& background = graph->atlas_tiles[u"background"];
        for (int y = 0; y < m_size.h; y += 280) {
            for (int x = 0; x < m_size.w; x += 280) {
                auto& tile = graph->get_gui_quad(px + x, py + y, 280, 280, background);
                tile.vertex[0].color[3] = 0.5f;
                tile.vertex[1].color[3] = 0.5f;
                tile.vertex[2].color[3] = 0.5f;
                tile.vertex[3].color[3] = 0.5f;
            }
        }

        for (auto i = start_render; i != end_render; ++i) {
            (*i)->render(graph, px + (*i)->m_position.x + m_scroll.x, py + (*i)->m_position.y + m_scroll.y);
        }
        m_scrollbar.render(graph, px + m_scrollbar.m_position.x, py + m_scrollbar.m_position.y);


		auto& border_tile = graph->atlas_tiles[u"border"];
        if (graph->add_scissor(px + 32, py, m_size.w - 64, m_size.h)) {
            for (int i = px + 32; i < px + m_size.w - 32; i += 64) {
                graph->get_gui_quad(i, py, 64, 12, border_tile);
                graph->get_gui_quad(i, py + m_size.h - 12, 64, 12, border_tile, tile_options_e::FLIP_Y);
            }
            graph->remove_scissor();
        }
        auto& border_vertical_tile = graph->atlas_tiles[u"border_vertical"];
        if (graph->add_scissor(px, py + 32, m_size.w, m_size.h - 64)) {
            for (int i = py + 32; i < py + m_size.h - 32; i += 64) {
                graph->get_gui_quad(px, i, 12, 64, border_vertical_tile);
                graph->get_gui_quad(px + m_size.w - 12, i, 12, 64, border_vertical_tile, tile_options_e::FLIP_X);
            }
            graph->remove_scissor();
        }

        auto& corner_tile = graph->atlas_tiles[u"corner"];
        graph->get_gui_quad(px, py + m_size.h - 32, 32, 32, corner_tile);
        graph->get_gui_quad(px + m_size.w - 32, py + m_size.h - 32, 32, 32, corner_tile, tile_options_e::FLIP_X);
        graph->get_gui_quad(px, py, 32, 32, corner_tile, tile_options_e::FLIP_Y);
        graph->get_gui_quad(px + m_size.w - 32, py, 32, 32, corner_tile, tile_options_e::FLIP_X | tile_options_e::FLIP_Y);
        graph->remove_scissor();
    }
}

void GUI_Scrollable_container::resize(int width, int height)
{
    m_size.w = width;
    m_size.h = height;
    m_scrollbar.m_position = position_t<int>(width - 20, 2);
    m_scrollbar.resize(18, height - 3);
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