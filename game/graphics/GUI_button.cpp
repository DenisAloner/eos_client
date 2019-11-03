#include "GUI_button.h"
#include "Application.h"

GUI_button::GUI_button(int x, int y, int width, int height, std::u16string text)
{
    m_position = position_t<int>(x, y);
    m_size = dimension_t<int>(width, height);
    m_text = new GUI_Text(text);
    m_text->m_position = Application::instance().m_graph->center_align_to_point(m_size.w * 0.5, (m_size.h - m_text->m_size.h) * 0.5, m_text->m_text);
}

GUI_button::~GUI_button()
{
}

void GUI_button::render(GraphicalController* graph, int px, int py)
{
    glEnable(GL_SCISSOR_TEST);
    if (graph->add_scissor(rectangle_t<float>(float(px), float(py), float(m_size.w), float(m_size.h)))) {
        glEnable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glColor4d(0.0, 0.0, 0.0, 0.5);
        const rectangle_t<int> rect(px, py, m_size.w, m_size.h);
        graph->draw_sprite(rect);
        glEnable(GL_TEXTURE_2D);
        glColor4d(1.0, 1.0, 1.0, 1.0);
        m_text->render(graph, px + m_text->m_position.x, py + m_text->m_position.y);
        graph->remove_scissor();
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        graph->draw_rectangle(rectangle_t<int>(px, py, m_size.w, m_size.h));
    }
}

GUI_Mainmenu_button::GUI_Mainmenu_button(int x, int y, int width, int height, std::u16string text, parameter_type_e kind)
    : GUI_button(x, y, width, height, text)
    , m_parameter(new Parameter(kind))
{
}

void GUI_Mainmenu_button::on_mouse_click(MouseEventArgs const& e)
{
    GUI_Object::on_mouse_click(e);
    if (Application::instance().m_message_queue.m_reader) {
        Application::instance().m_message_queue.push(m_parameter);
    }
}
