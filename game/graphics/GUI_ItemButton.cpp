#include "GUI_ItemButton.h"

GUI_ItemButton::GUI_ItemButton()
{
}

GUI_ItemButton::~GUI_ItemButton()
{
}

bool GUI_ItemButton::check_region(MouseEventArgs const& e)
{
    const auto radius = m_size.w * 0.5F;
    const auto local_x = (e.position.x - (m_position.x + radius));
    const auto local_y = (e.position.y - (m_position.y + radius));
    if (local_x * local_x + local_y * local_y - radius * radius <= 0) {
        return true;
    }
    return false;
}

void GUI_ItemButton::render(GraphicalController* graph, int px, int py)
{
    glColor4d(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, graph->m_close);
    const rectangle_t<int> rect(px, py, m_size.w, m_size.h);
    graph->draw_sprite(rect);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    /*glBegin(GL_LINES);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glVertex2d(px, py);
	glVertex2d(px, py + height);
	glVertex2d(px, py + height);
	glVertex2d(px + width, py + height);
	glVertex2d(px + width, py + height);
	glVertex2d(px + width, py);
	glVertex2d(px + width, py);
	glVertex2d(px, py);
	glEnd();*/
}

void GUI_ItemButton::on_mouse_move(MouseEventArgs const& e)
{
    //SetCursor(LoadCursor(NULL, IDC_HAND));
}