#include "GUI_Inventory.h"


GUI_InventoryItem::GUI_InventoryItem(int _Width, int _Height, int _Index) : Index(_Index)
{
	width = _Width;
	height = _Height;
}


GUI_InventoryItem::~GUI_InventoryItem()
{
}

void GUI_InventoryItem::RenderAt(GraphicalController* Graph, int px, int py)
{
	//glColor4d(1.0, 1.0, 1.0, 1.0);
	//glEnable(GL_BLEND);
	//glEnable(GL_TEXTURE_2D);
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glBindTexture(GL_TEXTURE_2D, Graph->Sprites[18]);
	//Graph->DrawSprite(px, py, px, py + height, px + width, py + height, px + width, py);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2d(px, py);
	glVertex2d(px, py + height);
	glVertex2d(px, py + height);
	glVertex2d(px + width, py + height);
	glVertex2d(px + width, py + height);
	glVertex2d(px + width, py);
	glVertex2d(px + width, py);
	glVertex2d(px, py);
	glEnd();
}

GUI_Inventory::GUI_Inventory(Property_Container* _Property)
{
	GUI_InventoryItem* _Item;
	width = _Property->Width * 48;
	height = _Property->Height * 48;
	for (int i = 0; i < _Property->Height; i++)
	{
		for (int j = 0; j < _Property->Width; j++)
		{
			_Item = new GUI_InventoryItem(48, 48, i*_Property->Width + j);
			_Item->x = j * 48;
			_Item->y = i * 48;
			add(_Item);
		}
	}
}


GUI_Inventory::~GUI_Inventory()
{
}
