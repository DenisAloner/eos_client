#include "GUI_Timer.h"


GUI_Timer::GUI_Timer(int _x, int _y, int _width, int _height, int time) :GUI_Container(_x, _y, _width, _height)
{
	ItemControls->add(new GUI_Text(""));
	Update(time);
}


GUI_Timer::~GUI_Timer()
{
}

void GUI_Timer::Update(int time)
{
	GUI_Text* Text = static_cast<GUI_Text*>(ItemControls->Items.front());
	Text->Format->SizeX = 48;
	Text->Format->SizeY = 57;
	Text->Format->Color.R = 1 - time / 15.0;
	Text->Format->Color.G = time / 15.0;
	Text->Format->Color.B = 0.0;
	Text->Set(std::to_string(time));
	Text->x = width*0.5 - Text->width*0.5;
	Text->y = height*0.5 - Text->height*0.5+2;
}