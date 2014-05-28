#pragma once
#include "GUI_Container.h"
#include "GUI_Text.h"
#include "GUI_ItemButton.h"
#include "TActionManager.h"

class GameTask;

class GUI_Item :
	public GUI_Container
{
public:

	Event<GUI_Object*> Close;

	GameTask* Tag;

	GUI_Item(int _x, int _y, int _width, int _height, std::string text, GameTask* tag);
	~GUI_Item();

	void OnCloseButtonClick();
	virtual void Resize(int _width, int _height);
	//virtual void OnMouseClick(MouseEventArgs const& e);
};

