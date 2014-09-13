#ifndef GUI_ITEM_H
#define	GUI_ITEM_H

#include "GUI_Container.h"
#include "GUI_Text.h"
#include "GUI_ItemButton.h"
#include "ActionManager.h"
#include "GameObject.h"

class GameTask;
class GameObject;

class GUI_Item :
	public GUI_Container
{
public:

	Event<GUI_Object*> close;

	tag_t m_tag;

	GUI_Item(int _x, int _y, int _width, int _height, std::string text, tag_t tag);
	~GUI_Item();

	void on_close_button_click();
	virtual void resize(int _width, int _height);
	//virtual void OnMouseClick(MouseEventArgs const& e);
};

#endif //GUI_ITEM_H
