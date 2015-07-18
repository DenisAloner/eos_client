#pragma once
#include "GUI_Object.h"
#include "GUI_Text.h"

class GUI_button :
	public GUI_Object
{
public:

	GUI_Text* m_text;

	GUI_button(int x, int y, int width, int height, std::string text);
	~GUI_button();

	virtual void render(GraphicalController* Graph, int px, int py);

};

