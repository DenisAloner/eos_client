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
class Attribute_map;

class GUI_Body;

class GUI_Part_slot :
	public GUI_Object
{
public:

	Object_part* m_item;
	GUI_Body* m_owner;
	std::string m_name;

	GUI_Part_slot(int width, int height, Object_part* item, GUI_Body* owner);

	virtual void render(GraphicalController* Graph, int px, int py);
	virtual void on_mouse_move(MouseEventArgs const& e);
	virtual void on_mouse_down(MouseEventArgs const& e);

};

//class GUI_body_cell :
//	public GUI_Container
//{
//public:
//
//	Object_part* m_items;
//	GUI_Body* m_owner;
//
//	GUI_body_cell(int width, int height, Object_part* item, GUI_Body* owner);
//
//	virtual void add_item_control(GUI_Object* object);
//	virtual void render(GraphicalController* Graph, int px, int py);
//	//virtual void on_mouse_move(MouseEventArgs const& e);
//	//virtual void on_mouse_down(MouseEventArgs const& e);
//	//virtual void set_focus(bool state);
//
//};


class GUI_Body :
	public GUI_Container
{
public:

	bool m_already_active;
	GUI_Body(Attribute_map* feature);
	
	std::list<std::string*> m_owner_name;
	std::size_t m_max_item_name;

	//virtual void Render(GraphicalController* Graph);
	virtual void add_item_control(GUI_Object* object);
	virtual void on_mouse_wheel(MouseEventArgs const& e);
	virtual void set_scroll(int dy);
	virtual void on_mouse_down(MouseEventArgs const& e);
	virtual void set_focus(bool state);
	void get_part_predicat(Object_interaction* object, bool add_mode);
	void update(Attribute_map* feature);

};

//class GUI_Container_visitor :public Visitor
//{
//public:
//
//	GUI_Body* m_owner;
//
//	std::list<std::string*> m_owner_name;
//	void visit(Object_interaction& value) override;
//};


#endif //GUI_INVENTORY_H
