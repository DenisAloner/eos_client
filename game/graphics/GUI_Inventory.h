#pragma once
#include "GUI_Layer.h"
#include "Property_Container.h"

class Property_Container;

class GUI_InventoryItem :
	public GUI_Object
{
public:

	int m_index;

	GUI_InventoryItem(int _Width, int _Height, int _Index);
	~GUI_InventoryItem();

	virtual void render(GraphicalController* Graph, int px, int py);

};

class GUI_Inventory :
	public GUI_Layer
{
public:

	GUI_Inventory(Property_Container* _Property);
	~GUI_Inventory();
};

