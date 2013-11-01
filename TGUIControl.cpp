#include "TGUIControl.h"
#include "TGraph.h"


TGUIControl::TGUIControl(void)
{
}


TGUIControl::~TGUIControl(void)
{
}

bool TGUIControl::Select(int x,int y)
{
	if(this->x<=x&&this->x+width>=x&&this->y<=y&&this->y+height>=y)
	{
		Click();
		return true;
	}
	return false;
}

void TGUIControl::Render(TGraph* Graph)
{
}
