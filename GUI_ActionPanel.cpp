#include "GUI_ActionPanel.h"


GUI_ActionPanel::GUI_ActionPanel(int _x, int _y, int _width, int _height) :GUI_Container(_x, _y, _width, _height)
{
}


GUI_ActionPanel::~GUI_ActionPanel()
{
}

void GUI_ActionPanel::OnMouseClick(MouseEventArgs const& e)
{
	GUI_Layer::OnMouseClick(e);
	if ((focus != this) && (focus == nullptr))
	{
		SetFocus(true);
	}
}

void GUI_ActionPanel::AddItemControl(GUI_Object* object)
{
	if (!ItemControls->Items.empty())
	{
		GUI_Object* Back = ItemControls->Items.back();
		object->y = 4;
		object->x = Back->x + Back->width+4;
		if (object->x + object->width>width)
		{
			Scroll.x -= object->width;
		}
	}
	else
	{
		object->x = 4;
		object->y = 4;
	}
	ItemControls->add(object);
}