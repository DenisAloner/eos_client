#include "GUI_Inventory.h"
#include "game/utils/log.h"


GUI_InventoryItem::GUI_InventoryItem(int width, int height, Inventory_cell* item, GUI_Inventory* owner) : m_item(item), m_owner(owner)
{
	m_size.w = width;
	m_size.h = height;
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0, 0.9, 0.0, 0.25);
	if (focused)
	{
		Graph->draw_sprite(px, py, px, py + +m_size.h, px + m_size.w, py + +m_size.h, px + m_size.w, py);
	}
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
	if (m_item->m_item)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, m_item->m_item->m_active_state->m_icon);
		Graph->draw_sprite(px, py, px, py + +m_size.h, px + m_size.w, py + +m_size.h, px + m_size.w, py);
	}
}

void GUI_InventoryItem::on_mouse_move(MouseEventArgs const& e)
{
	set_focus(true);
}

void GUI_InventoryItem::on_mouse_down(MouseEventArgs const& e)
{
	set_focus(true);
	if (Application::instance().m_message_queue.m_reader)
	{
		P_inventory_cell* p = new P_inventory_cell();
		p->m_cell = m_item;
		Application::instance().m_message_queue.push(p);
	}
}

GUI_Inventory::GUI_Inventory(Property_Container* property) :GUI_Container(0, 0, property->m_size.w * 64, property->m_size.h * 64), m_item(property)
{
	m_already_active = false;
	GUI_InventoryItem* item;
	for (int i = 0; i < property->m_size.h; i++)
	{
		for (int j = 0; j < property->m_size.w; j++)
		{
			item = new GUI_InventoryItem(64, 64, (*std::next(property->m_items.begin(), i*property->m_size.w + j)),this);
			item->m_position.x = j * 64;
			item->m_position.y = i * 64;
			add(item);
		}
	}
}

void GUI_Inventory::on_mouse_down(MouseEventArgs const& e)
{
	if (m_already_active) {
		GUI_Container::on_mouse_down(e);
	}
	else {
		m_already_active = true;
	}
}

void GUI_Inventory::set_focus(bool state)
{
	if (!state)
	{
		m_already_active = false;
	}
	GUI_Container::set_focus(state);
}

GUI_Inventory::~GUI_Inventory()
{
}
