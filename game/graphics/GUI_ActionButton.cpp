#include <thread>
#include "game/graphics/GUI_ActionButton.h"


GUI_ActionButton::GUI_ActionButton()
{
	m_size.w = 40;
	m_size.h = 40;
}


GUI_ActionButton::~GUI_ActionButton()
{
}

void GUI_ActionButton::render(GraphicalController* Graph, int px, int py)
{
	glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
	glColor4d(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Graph->m_actions[m_action->m_kind]);
	Graph->draw_sprite(px, py, px, py + m_size.h, px + m_size.w, py +m_size.h, px + m_size.w, py);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glColor4f(1.0, 1.0, 1.0, 1);
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

void GUI_ActionButton::on_mouse_click(MouseEventArgs const& e)
{
	set_focus(true);
	std::thread t(&Action::interaction_handler,m_action);
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