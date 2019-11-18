#include "GUI_Window.h"
#include "GUI_Text.h"

GUI_Header::GUI_Header(int x, int y, int width, int height, const std::u16string& text)
    : GUI_Container(x, y, width, height)
{
    const auto m_text = new GUI_Text(text);
    m_text->m_position.x = 18;
    m_text->m_position.y = 6;
    m_size.h = m_text->m_size.h + m_text->m_position.y + 2;
    GUI_Layer::add(m_text);
    auto m_button = new GUI_ItemButton();
    m_button->m_position.x = m_size.w - m_size.h;
    m_button->m_position.y = 0;
    m_button->m_size.w = m_size.h;
    m_button->m_size.h = m_size.h;
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
    m_items.front()->m_position.x = m_size.w - m_size.h;
    m_items.front()->m_position.y = 0;
    m_items.front()->m_size.w = m_size.h;
    m_items.front()->m_size.h = m_size.h;
}

GUI_Window::GUI_Window(int x, int y, int width, int height, const std::u16string& name, gui_style_t& style)
    : GUI_Container(x, y, width, height)
    , m_style(style)
{
    m_header = new GUI_Header(0, 0, m_size.w , 32, name);
    m_header->close += std::bind(&GUI_Window::on_header_close, this);
    GUI_Layer::add(m_header);
    close += std::bind(&GUI_Window::on_close, this, std::placeholders::_1);
    Application::instance().m_window_manager->add_front(this);
}

rectangle_t<int> GUI_Window::client_rect()
{
    return rectangle_t<int>(1, m_header->m_position.y + m_header->m_size.h + 1, m_size.w - 2, m_size.h - (m_header->m_position.y + m_header->m_size.h + 1));
}

void GUI_Window::render(GraphicalController* graph, int px, int py)
{
    if (graph->add_scissor(px, py, m_size.w, m_size.h)) {
        graph->render_background(px, py, m_size.w, m_size.h, m_style);
        for (auto& current : m_items) {
            if (current == m_header) continue;
            current->render(graph, px + current->m_position.x + m_scroll.x, py + current->m_position.y + m_scroll.y);
        }
        graph->render_border(px, py, m_size.w, m_size.h, m_style);
        m_header->render(graph, px + m_header->m_position.x + m_scroll.x, py + m_header->m_position.y + m_scroll.y);
        graph->remove_scissor();
    }
}

GUI_body_window::GUI_body_window(int x, int y, int width, int height, const std::u16string& name, gui_style_t& style, GameObject*& object)
    : GUI_Window(x, y, width, height, name, style)
    , m_object(object)
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
    resize(m_item->m_size.w + 4, m_item->m_position.y + m_item->m_size.h + 2);
}