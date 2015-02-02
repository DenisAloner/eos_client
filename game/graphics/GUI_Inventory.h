#ifndef GUI_INVENTORY_H
#define	GUI_INVENTORY_H

#include "GUI_Container.h"
#include <list>
#include "GameObject.h"

class Property_Container;
class Parts_list;
class Inventory_cell;
class GameObject;
class Object_part;
class Interaction_feature;

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

	virtual void on_mouse_down(MouseEventArgs const& e);
	virtual void set_focus(bool state);
};

class GUI_Body;

class GUI_body_cell :
	public GUI_Object
{
public:

	Object_part* m_item;
	GUI_Body* m_owner;

	GUI_body_cell(int width, int height, Object_part* item, GUI_Body* owner);

	virtual void render(GraphicalController* Graph, int px, int py);
	virtual void on_mouse_move(MouseEventArgs const& e);
	virtual void on_mouse_down(MouseEventArgs const& e);

};

class GUI_Body :
	public GUI_Container
{
public:

	bool m_already_active;
	GUI_Body(Interaction_feature* feature);

	//virtual void Render(GraphicalController* Graph);
	virtual void add_item_control(GUI_Object* object);
	virtual void on_mouse_wheel(MouseEventArgs const& e);
	virtual void set_scroll(int dy);
	virtual void on_mouse_down(MouseEventArgs const& e);
	virtual void set_focus(bool state);
	void get_part_predicat(Object_interaction* object);
	void update(Interaction_feature* feature);

};

#endif //GUI_INVENTORY_H
