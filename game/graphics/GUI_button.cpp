#include "GUI_button.h"


GUI_button::GUI_button(int x, int y, int width, int height, std::string text)
{
	m_position = position_t(x, y);
	m_size = dimension_t(width, height);
	m_text = new GUI_Text(text);
	m_text->m_position = position_t((m_size.w - m_text->m_size.w)*0.5, (m_size.h - m_text->m_size.h)*0.5);
}


GUI_button::~GUI_button()
{
}

void GUI_button::render(GraphicalController* Graph, int px, int py)
{
	glColor4d(1.0, 1.0, 1.0, 1.0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glBegin(GL_LINES);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glVertex2d(px, py);
	glVertex2d(px, py + m_size.h);
	glVertex2d(px, py + m_size.h);
	glVertex2d(px + m_size.w, py + m_size.h);
	glVertex2d(px + m_size.w, py + m_size.h);
	glVertex2d(px + m_size.w, py);
	glVertex2d(px + m_size.w, py);
	glVertex2d(px, py);
	glEnd();
	m_text->render(Graph, px + m_text->m_position.x, py + m_text->m_position.y);
}