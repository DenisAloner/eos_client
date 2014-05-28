#include "GUI_ActionButton.h"


GUI_ActionButton::GUI_ActionButton()
{
	width = 40;
	height = 40;
}


GUI_ActionButton::~GUI_ActionButton()
{
}

void GUI_ActionButton::RenderAt(GraphicalController* Graph, int px, int py)
{
	glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
	glColor4d(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Graph->Actions[Action->kind]);
	Graph->DrawSprite(px, py, px, py + height, px + width, py + height, px + width, py);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glColor4f(1.0, 1.0, 1.0, 1);
	glVertex2d(px, py);
	glVertex2d(px, py + height);
	glVertex2d(px, py + height);
	glVertex2d(px + width, py + height);
	glVertex2d(px + width, py + height);
	glVertex2d(px + width, py);
	glVertex2d(px + width, py);
	glVertex2d(px, py);
	glEnd();
}

void GUI_ActionButton::OnMouseClick(MouseEventArgs const& e)
{
	SetFocus(true);
	std::thread t(&TAction::InteractionHandler,Action);
	t.detach();
}

bool GUI_ActionButton::CheckRegion(MouseEventArgs const& e)
{
	//char buf2[32];
	//itoa(e.Position.x, buf2, 10);
	//MessageBox(NULL, buf2, "eos", MB_OK);
	if (this->x <= e.Position.x&&this->x + width >= e.Position.x&&this->y <= e.Position.y&&this->y + height >= e.Position.y)
	{
		return true;
	}
	return false;
}