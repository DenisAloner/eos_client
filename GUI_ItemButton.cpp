#include "GUI_ItemButton.h"


GUI_ItemButton::GUI_ItemButton()
{
}


GUI_ItemButton::~GUI_ItemButton()
{
}

bool GUI_ItemButton::CheckRegion(MouseEventArgs const& e)
{
	float Radius = width*0.5;
	float LocalX = (e.Position.x - (x + Radius));
	float LocalY = (e.Position.y - (y + Radius));
	if (LocalX*LocalX+LocalY*LocalY-Radius*Radius<=0)
	{
		return true;
	}
	return false;
}

void GUI_ItemButton::RenderAt(GraphicalController* Graph, int px, int py)
{
	glColor4d(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Graph->Sprites[18]);
	Graph->DrawSprite(px, py, px, py + height, px + width, py + height, px + width, py);
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

void GUI_ItemButton::OnMouseMove(MouseEventArgs const& e)
{
	SetCursor(LoadCursor(NULL, IDC_HAND));
}