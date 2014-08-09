#pragma once
#include "GUI_Container.h"
#include "GUI_Text.h"
class GUI_Timer :
	public GUI_Container
{
public:

	GUI_Timer(int _x, int _y, int _width, int _height,int time);
	~GUI_Timer();

	void Update(int time);
};

