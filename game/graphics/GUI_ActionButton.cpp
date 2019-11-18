#include "game/graphics/GUI_ActionButton.h"

GUI_ActionButton::GUI_ActionButton(const int width, const int height)
{
    m_size.w = width;
    m_size.h = height;
}

GUI_ActionButton::~GUI_ActionButton()
{
}

void GUI_ActionButton::render(GraphicalController* graph, const int px, const int py)
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    const int y_shift = (m_size.h - Application::instance().m_graph->max_symbol_height_for_current_font()) / 2;
    auto rect = rectangle_t<int>(px, py, m_size.w, m_size.h);
    if (focused) {
        glColor4d(1.0, 1.0, 1.0, 0.75);
        graph->draw_sprite(rect);
        glColor4d(0, 0, 0, 1);
    } else {
        glColor4d(1, 1, 1, 1);
    }
    glEnable(GL_TEXTURE_2D);
    auto desc = m_value.action->get_description(m_value.parameter);
    graph->output_text(px + m_size.h + 4, py + y_shift, desc, 8, 17);
    glDisable(GL_BLEND);
    glColor4d(1, 1, 1, 1);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, m_value.action->m_icon);
    rect = rectangle_t<int>(px, py, m_size.h, m_size.h);
    graph->draw_sprite(rect);
    if (focused) {
        glDisable(GL_TEXTURE_2D);
        glColor4d(0.75, 0.75, 0.75, 1.0);
        graph->draw_rectangle(rect);
    }
}

void GUI_ActionButton::on_mouse_move(MouseEventArgs const& e)
{
    set_focus(true);
}

void GUI_ActionButton::on_mouse_click(MouseEventArgs const& e)
{
    set_focus(true);
    std::thread t(&Action::interaction_handler, m_value.action, m_value.parameter);
    t.detach();
}

bool GUI_ActionButton::check_region(MouseEventArgs const& e)
{
    return this->m_position.x <= e.position.x && this->m_position.x + m_size.w >= e.position.x && this->m_position.y <= e.position.y && this->m_position.y + m_size.h >= e.position.y;
}