#include "GUI_ItemButton.h"


GUI_ItemButton::GUI_ItemButton()
{
}


GUI_ItemButton::~GUI_ItemButton()
{
}

bool GUI_ItemButton::check_region(MouseEventArgs const& e)
{
	float Radius = m_size.w*0.5;
	float LocalX = (e.position.x - (m_position.x + Radius));
	float LocalY = (e.position.y - (m_position.y + Radius));
	if (LocalX*LocalX+LocalY*LocalY-Radius*Radius<=0)
	{
		return true;
	}
	return false;
}

void GUI_ItemButton::render(GraphicalController* Graph, int px, int py)
{
	glColor4d(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Graph->m_sprites[18]);
	Graph->draw_sprite(px, py, px, py + m_size.h, px + m_size.w, py + m_size.h, px + m_size.w, py);
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
	SetCursor(LoadCursor(NULL, IDC_HAND));
}