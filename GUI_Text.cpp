#include "GUI_Text.h"


GUI_Text::GUI_Text(std::string _Text)
{
	Format = new GUI_TextFormat(8, 17, TColor(1.0, 1.0, 1.0, 1.0));
	Set(_Text);
}

GUI_Text::GUI_Text(std::string _Text, GUI_TextFormat* _Format) : Format(_Format)
{
	Set(_Text);
}

GUI_Text::~GUI_Text()
{
}

void GUI_Text::Render(GraphicalController* Graph)
{
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glColor4d(Format->Color.R, Format->Color.G, Format->Color.B, Format->Color.A);
	Graph->STextXY(x, y, Text, Format->SizeX, Format->SizeY);
}


void GUI_Text::RenderAt(GraphicalController* Graph, int px, int py)
{
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glColor4d(Format->Color.R, Format->Color.G, Format->Color.B, Format->Color.A);
	Graph->STextXY(px, py, Text, Format->SizeX, Format->SizeY);
}

void GUI_Text::Set(std::string t)
{
	this->Text = t;
	width = Text.length() *  Format->SizeX;
	height = Format->SizeY;
}