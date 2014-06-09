#include "GUI_Inventory.h"


GUI_InventoryItem::GUI_InventoryItem(int _Width, int _Height, int _Index) : m_index(_Index)
{
	m_size.w = _Width;
	m_size.h = _Height;
}


GUI_InventoryItem::~GUI_InventoryItem()
{
}

void GUI_InventoryItem::render(GraphicalController* Graph, int px, int py)
{
	//glColor4d(1.0, 1.0, 1.0, 1.0);
	//glEnable(GL_BLEND);
	//glEnable(GL_TEXTURE_2D);
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glBindTexture(GL_TEXTURE_2D, Graph->Sprites[18]);
	//Graph->draw_sprite(px, py, px, py + height, px + width, py + height, px + width, py);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2d(px, py);
	glVertex2d(px, py + m_size.h);
	glVertex2d(px, py + m_size.h);
	glVertex2d(px + m_size.w, py + m_size.h);
	glVertex2d(px + m_size.w, py + m_size.h);
	glVertex2d(px + m_size.w, py);
	glVertex2d(px + m_size.w, py);
	glVertex2d(px, py);
	glEnd();
}

GUI_Inventory::GUI_Inventory(Property_Container* _Property)
{
	GUI_InventoryItem* _Item;
	m_size.w = _Property->m_size.w * 48;
	m_size.h = _Property->m_size.h * 48;
	for (int i = 0; i < _Property->m_size.h; i++)
	{
		for (int j = 0; j < _Property->m_size.w; j++)
		{
			_Item = new GUI_InventoryItem(48, 48, i*_Property->m_size.w + j);
			_Item->m_position.x = j * 48;
			_Item->m_position.y = i * 48;
			add(_Item);
		}
	}
}


GUI_Inventory::~GUI_Inventory()
{
}
