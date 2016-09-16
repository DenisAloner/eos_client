#include <thread>
#include "game/graphics/GUI_ActionButton.h"


GUI_ActionButton::GUI_ActionButton(int width, int height)
{
	m_size.w = width;
	m_size.h = height;
}


GUI_ActionButton::~GUI_ActionButton()
{
}

void GUI_ActionButton::render(GraphicalController* Graph, int px, int py)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int y_shift = (m_size.h - ((Application::instance().m_graph->m_face->size->metrics.ascender - Application::instance().m_graph->m_face->size->metrics.descender) >> 6)) / 2;

	if (focused)
	{
		glColor4d(1.0, 1.0, 1.0, 0.75);
		Graph->draw_sprite(px, py, px, py + m_size.h, px + m_size.w, py + m_size.h, px + m_size.w, py);
		glColor4d(0, 0, 0, 1);
	}
	else 
	{
		glColor4d(1, 1, 1, 1);
	}
	glEnable(GL_TEXTURE_2D);
	Graph->output_text(px + m_size.h + 4, py + y_shift, m_value.action->get_description(m_value.parameter), 8, 17);
	glDisable(GL_BLEND);
	glColor4d(1, 1, 1, 1);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, m_value.action->m_icon);
	Graph->draw_sprite(px, py, px, py + m_size.h, px + m_size.h, py + m_size.h, px + m_size.h, py);

	if (focused)
	{
		glDisable(GL_TEXTURE_2D);
		glColor4d(0.75, 0.75, 0.75, 1.0);
		glBegin(GL_LINES);
		glVertex2d(px, py);
		glVertex2d(px, py + m_size.h);
		glVertex2d(px, py + m_size.h);
		glVertex2d(px + m_size.w, py + m_size.h);
		glVertex2d(px + m_size.w, py + m_size.h);
		glVertex2d(px + m_size.w, py);
		glVertex2d(px + m_size.w, py);
		glVertex2d(px, py);
		glEnd();
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
	//char buf2[32];
	//itoa(e.Position.x, buf2, 10);
	//MessageBox(NULL, buf2, "eos", MB_OK);
	if (this->m_position.x <= e.position.x&&this->m_position.x + m_size.w >= e.position.x&&this->m_position.y <= e.position.y&&this->m_position.y + m_size.h >= e.position.y)
	{
		return true;
	}
	return false;
}