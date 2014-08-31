#ifndef GUI_INVENTORY_H
#define	GUI_INVENTORY_H

#include "GUI_Container.h"
#include "Property_Container.h"
#include <list>
#include "GameObject.h"

class Property_Container;
class Inventory_cell;
class GameObject;

class GUI_Inventory;

class GUI_InventoryItem :
	public GUI_Object
{
public:

	Inventory_cell* m_item;
	GUI_Inventory* m_owner;

	GUI_InventoryItem(int width, int height, Inventory_cell* item, GUI_Inventory* owner);
	~GUI_InventoryItem();

	virtual void render(GraphicalController* Graph, int px, int py);
	virtual void on_mouse_move(MouseEventArgs const& e);
	virtual void on_mouse_down(MouseEventArgs const& e);

};

class GUI_Inventory :
	public GUI_Container
{
public:

	bool m_already_active;
	Property_Container* m_item;

	GUI_Inventory(Property_Container* property);
	~GUI_Inventory();

	virtual void GUI_Inventory::on_mouse_down(MouseEventArgs const& e);
	virtual void GUI_Inventory::set_focus(bool state);
};

#endif //GUI_INVENTORY_H
