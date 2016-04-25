#include "game/graphics/GUI_MapViewer.h"
#include "game/graphics/GUI_Layer.h"
#include "game/graphics/GUI_PopMenu.h"
#include "game/ActionManager.h"
#include "log.h"
#include <algorithm>

gui_mapviewer_hint::gui_mapviewer_hint(GUI_MapViewer* owner) :m_owner(owner) { m_top = false; }
mapviewer_hint_path::mapviewer_hint_path(GUI_MapViewer* owner, std::vector<MapCell*>* path) : gui_mapviewer_hint(owner), m_path(path) {};
mapviewer_hint_area::mapviewer_hint_area(GUI_MapViewer* owner, GameObject* object, bool consider_object_size) : gui_mapviewer_hint(owner), m_object(object), m_consider_object_size(consider_object_size) {}
mapviewer_hint_object_area::mapviewer_hint_object_area(GUI_MapViewer* owner, GameObject* object) : gui_mapviewer_hint(owner), m_object(object) {}
mapviewer_hint_line::mapviewer_hint_line(GUI_MapViewer* owner, MapCell* cell) : gui_mapviewer_hint(owner), m_cell(cell) {}

void gui_mapviewer_hint::draw_cell(MapCell* cell, int index)
{
	double x0, y0, x1, y1, x2, y2, x3, y3;
	int px = 0;
	int py = 0;
	int x = px + cell->x - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
	int y = py + cell->y - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
	int yp = m_owner->m_tile_count_x - x;
	int xp = m_owner->m_tile_count_y - y;
	x0 = (xp - yp) * 16 + m_owner->m_shift.x;
	y0 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
	x1 = x0;
	y1 = (xp + yp) * 9 + m_owner->m_shift.y;
	x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
	y2 = y1;
	x3 = x2;
	y3 = y0;
	glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
	glColor4f(1.0F, 0.9F, 0.0F, 0.5F);
	Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
	glColor4f(1.0F, 0.9F, 0.0F, 1.0F);
	Application::instance().m_graph->center_text((x0 + x2)*0.5, (y0 + y1) *0.5, std::to_string(index), 8, 17);
}

void mapviewer_hint_path::render()
{
	int px = 0;
	int py = 0;
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	if (m_owner->m_cursored != nullptr)
	{
		if (m_path)
		{
			for (int i = 0; i < m_path->size(); i++)
			{
				draw_cell((*m_path)[(m_path->size() - 1) - i], i);
			}
		}
	}
}

void mapviewer_hint_object_area::render()
{
	int px = 0;
	int py = 0;
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
	double x0, y0, x1, y1, x2, y2, x3, y3;
	MapCell* Item = m_object->cell();
	int bx;
	int by;
	bx = m_object->m_active_state->m_size.x;
	by = m_object->m_active_state->m_size.y;
	for (int i = 0; i < by; i++)
	{
		for (int j = 0; j < bx; j++)
		{
			int x = px + (Item->x + j) - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
			int y = py + (Item->y - i) - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
			int yp = m_owner->m_tile_count_x - x;
			int xp = m_owner->m_tile_count_y - y;
			x0 = (xp - yp) * 16 + m_owner->m_shift.x;
			y0 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
			x1 = x0;
			y1 = (xp + yp) * 9 + m_owner->m_shift.y;
			x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
			y2 = y1;
			x3 = x2;
			y3 = y0;
			glColor4f(1.0F, 0.9F, 0.0F, 0.25F);
			Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
			x0 = (xp - yp) * 16 + m_owner->m_shift.x;
			y0 = (xp + yp) * 9 - 9 + m_owner->m_shift.y;
			x1 = (xp - yp) * 16 + 16 + m_owner->m_shift.x;
			y1 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
			x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
			y2 = y0;
			x3 = x1;
			y3 = (xp + yp) * 9 + m_owner->m_shift.y;
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_LINES);
			glColor4f(1.0, 1.0, 0.0, 1.0);
			glVertex2d(x0, y0);
			glVertex2d(x1, y1);
			glVertex2d(x1, y1);
			glVertex2d(x2, y2);
			glVertex2d(x2, y2);
			glVertex2d(x3, y3);
			glVertex2d(x3, y3);
			glVertex2d(x0, y0);
			glEnd();
			glEnable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);
		}
	}
}

void mapviewer_hint_line::draw_cell(MapCell* a)
{
	double x0, y0, x1, y1, x2, y2, x3, y3;
	int px = 0;
	int py = 0;
	int x = px + a->x - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
	int y = py + a->y - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
	int yp = m_owner->m_tile_count_x - x;
	int xp = m_owner->m_tile_count_y - y;
	x0 = (xp - yp) * 16 + m_owner->m_shift.x;
	y0 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
	x1 = x0;
	y1 = (xp + yp) * 9 + m_owner->m_shift.y;
	x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
	y2 = y1;
	x3 = x2;
	y3 = y0;
	glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
	glColor4f(1.0F, 0.9F, 0.0F, 0.5F);
	Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
	glColor4f(1.0F, 0.9F, 0.0F, 1.0F);
	Application::instance().m_graph->center_text((x0 + x2)*0.5, (y0 + y1) *0.5, std::to_string(m_step_count), 8, 17);
	m_step_count += 1;
}

void mapviewer_hint_line::render()
{
	int px = 0;
	int py = 0;
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	if (m_owner->m_cursored != nullptr)
	{
		m_step_count = 0;
		m_owner->m_map->bresenham_line(m_cell, m_owner->m_cursored, std::bind(&mapviewer_hint_line::draw_cell, this, std::placeholders::_1));
	}
}

mapviewer_hint_weapon_range::mapviewer_hint_weapon_range(GUI_MapViewer* owner, GameObject* object, int range) : gui_mapviewer_hint(owner), m_object(object), m_range(range) {}

void mapviewer_hint_area::render()
{
	int px = 0;
	int py = 0;
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
	double x0, y0, x1, y1, x2, y2, x3, y3;
	MapCell* Item = m_object->cell();
	int bx;
	int by;
	if (m_consider_object_size)
	{
		bx = m_object->m_active_state->m_size.x;
		by = m_object->m_active_state->m_size.y;
	}
	else
	{
		bx = 1;
		by = 1;
	}
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
	for (int i = -1; i < by + 1; i++)
	{
		for (int j = -1; j < bx + 1; j++)
		{
			if (i == by || j == bx || i == -1 || j == -1)
			{
				int x = px + (Item->x + j) - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
				int y = py + (Item->y - i) - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
				int yp = m_owner->m_tile_count_x - x;
				int xp = m_owner->m_tile_count_y - y;
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x1 = x0;
				y1 = (xp + yp) * 9 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y1;
				x3 = x2;
				y3 = y0;
				glColor4f(1.0F, 0.9F, 0.0F, 0.25F);
				Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 9 + m_owner->m_shift.y;
				x1 = (xp - yp) * 16 + 16 + m_owner->m_shift.x;
				y1 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y0;
				x3 = x1;
				y3 = (xp + yp) * 9 + m_owner->m_shift.y;
				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
				glBegin(GL_LINES);
				glColor4f(1.0, 1.0, 0.0, 1.0);
				glVertex2d(x0, y0);
				glVertex2d(x1, y1);
				glVertex2d(x1, y1);
				glVertex2d(x2, y2);
				glVertex2d(x2, y2);
				glVertex2d(x3, y3);
				glVertex2d(x3, y3);
				glVertex2d(x0, y0);
				glEnd();
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
			}
		}
	}
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
	for (int i = -1; i < by + 1; i++)
	{
		for (int j = -1; j < bx + 1; j++)
		{
			if (i == by || j == bx || i == -1 || j == -1)
			{
				int x = px + (Item->x + j) - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
				int y = py + (Item->y - i) - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
				int yp = m_owner->m_tile_count_x - x;
				int xp = m_owner->m_tile_count_y - y;
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x1 = x0;
				y1 = (xp + yp) * 9 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y1;
				x3 = x2;
				y3 = y0;
				glColor4f(1.0F, 0.9F, 0.0F, 0.25F);
				Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 9 + m_owner->m_shift.y;
				x1 = (xp - yp) * 16 + 16 + m_owner->m_shift.x;
				y1 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y0;
				x3 = x1;
				y3 = (xp + yp) * 9 + m_owner->m_shift.y;
				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
				glBegin(GL_LINES);
				glColor4f(1.0, 1.0, 0.0, 1.0);
				glVertex2d(x0, y0);
				glVertex2d(x1, y1);
				glVertex2d(x1, y1);
				glVertex2d(x2, y2);
				glVertex2d(x2, y2);
				glVertex2d(x3, y3);
				glVertex2d(x3, y3);
				glVertex2d(x0, y0);
				glEnd();
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
			}
		}
	}
}

void mapviewer_hint_weapon_range::render()
{
	int px = 0;
	int py = 0;
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
	double x0, y0, x1, y1, x2, y2, x3, y3;
	MapCell* item = m_object->cell();
	int xc;
	int yc;
	xc = item->x + m_object->m_active_state->m_size.x / 2;
	yc = item->y - m_object->m_active_state->m_size.y / 2;
	int ax = 2*m_range + m_object->m_active_state->m_size.x;
	int ay = 2*m_range + m_object->m_active_state->m_size.y;
	int xr = ax / 2;
	int yr = ay / 2;
	for (int i = 0; i < ay; i++)
	{
		for (int j = 0; j < ax; j++)
		{
			//if (i*i + j*j <= m_range*m_range)
			int xp = xc + j - xr;
			int yp = yc - i + yr;
			if (!m_object->is_own(xp,yp))
			{
				int x = px + xp - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
				int y = py + yp - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
				/*x0 = x * m_owner->m_tile_size_x;
				y0 = (m_owner->m_tile_count_y - y - 1) * m_owner->m_tile_size_y;
				x1 = x0;
				y1 = (m_owner->m_tile_count_y - y) * m_owner->m_tile_size_y;
				x2 = (x + 1) * m_owner->m_tile_size_x;
				y2 = y1;
				x3 = x2;
				y3 = y0;*/
				int yp = m_owner->m_tile_count_x - x;
				int xp = m_owner->m_tile_count_y - y;
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x1 = x0;
				y1 = (xp + yp) * 9 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y1;
				x3 = x2;
				y3 = y0;
				glColor4f(1.0F, 0.9F, 0.0F, 0.25F);
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
				Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
				Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
				glColor4f(1.0F, 0.9F, 0.0F, 0.5F);
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 9 + m_owner->m_shift.y;
				x1 = (xp - yp) * 16 + 16 + m_owner->m_shift.x;
				y1 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y0;
				x3 = x1;
				y3 = (xp + yp) * 9 + m_owner->m_shift.y;
				//glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
				glBegin(GL_LINES);
				glVertex2d(x0, y0);
				glVertex2d(x1, y1);
				glVertex2d(x1, y1);
				glVertex2d(x2, y2);
				glVertex2d(x2, y2);
				glVertex2d(x3, y3);
				glVertex2d(x3, y3);
				glVertex2d(x0, y0);
				glEnd();
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
				glBegin(GL_LINES);
				glVertex2d(x0, y0);
				glVertex2d(x1, y1);
				glVertex2d(x1, y1);
				glVertex2d(x2, y2);
				glVertex2d(x2, y2);
				glVertex2d(x3, y3);
				glVertex2d(x3, y3);
				glVertex2d(x0, y0);
				glEnd();
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
			}
		}
	}
}

mapviewer_hint_shoot::mapviewer_hint_shoot(GUI_MapViewer* owner, GameObject* object, int range) : gui_mapviewer_hint(owner), m_object(object),m_range(range) { m_top = true; }

void mapviewer_hint_shoot::draw_cell(MapCell* a)
{
	if (!m_object->is_own(a))
	{
		double x0, y0, x1, y1, x2, y2, x3, y3;
		int px = 0;
		int py = 0;
		int x = px + a->x - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
		int y = py + a->y - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
		int yp = m_owner->m_tile_count_x - x;
		int xp = m_owner->m_tile_count_y - y;
		x0 = (xp - yp) * 16 + m_owner->m_shift.x;
		y0 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
		x1 = x0;
		y1 = (xp + yp) * 9 + m_owner->m_shift.y;
		x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
		y2 = y1;
		x3 = x2;
		y3 = y0;
		bool pass_able = true;
		glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
		if (m_step_count > m_range)
		{
			pass_able = false;
		}
		else
		{
			for (auto object = a->m_items.begin(); object != a->m_items.end(); object++)
			{
				if (!(*object)->get_tag(object_tag_e::pass_able))
				{
					pass_able = false;
					break;
				}
			}
		}
		if (pass_able)
		{
			glColor4f(0.0F, 1.0F, 0.0F, 0.5F);
		}
		else
		{
			glColor4f(1.0F, 0.0F, 0.0F, 0.5F);
		}
		Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
		glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
		Application::instance().m_graph->center_text((x0 + x2)*0.5, (y0 + y1) *0.5, std::to_string(m_step_count), 8, 17);
		if (pass_able)
		{
			glColor4f(0.0F, 1.0F, 0.0F, 1.0F);
		}
		else
		{
			glColor4f(1.0F, 0.0F, 0.0F, 1.0F);
		}
		x0 = (xp - yp) * 16 + m_owner->m_shift.x;
		y0 = (xp + yp) * 9 - 9 + m_owner->m_shift.y;
		x1 = (xp - yp) * 16 + 16 + m_owner->m_shift.x;
		y1 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
		x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
		y2 = y0;
		x3 = x1;
		y3 = (xp + yp) * 9 + m_owner->m_shift.y;
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
		glVertex2d(x0, y0);
		glVertex2d(x1, y1);
		glVertex2d(x1, y1);
		glVertex2d(x2, y2);
		glVertex2d(x2, y2);
		glVertex2d(x3, y3);
		glVertex2d(x3, y3);
		glVertex2d(x0, y0);
		glEnd();
		glEnable(GL_TEXTURE_2D);
		m_step_count += 1;
	}
}

void mapviewer_hint_shoot::render()
{
	int px = 0;
	int py = 0;
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	if (m_owner->m_cursored != nullptr)
	{
		m_step_count = 1;
		MapCell* cell = m_object->get_center(m_owner->m_cursored);
		//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
		m_owner->m_map->bresenham_line(cell, m_owner->m_cursored, std::bind(&mapviewer_hint_shoot::draw_cell, this, std::placeholders::_1));
		m_step_count = 1;
	/*	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
		m_owner->m_map->bresenham_line(cell, m_owner->m_cursored, std::bind(&mapviewer_hint_shoot::draw_cell, this, std::placeholders::_1));*/
	}
}

mapviewer_object_move::mapviewer_object_move(GUI_MapViewer* owner, GameObject* object) : gui_mapviewer_hint(owner), m_object(object) { init(); }

void mapviewer_object_move::init()
{
	if (m_owner->m_cursored)
	{
		m_cell = Game_algorithm::step_in_direction(m_object, Game_algorithm::turn_to_cell(m_object, m_owner->m_cursored));
	}
	else
	{
		m_cell = nullptr;
	}
}

void mapviewer_object_move::render()
{
	int px = 0;
	int py = 0;
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
	double x0, y0, x1, y1, x2, y2, x3, y3;
	MapCell* Item = m_object->cell();
	int bx;
	int by;
	bx = 1;
	by = 1;
	/*glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
	for (int i = -1; i < by + 1; i++)
	{
		for (int j = -1; j < bx + 1; j++)
		{
			if (i == by || j == bx || i == -1 || j == -1)
			{
				int x = px + (Item->x + j) - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
				int y = py + (Item->y - i) - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
				int yp = m_owner->m_tile_count_x - x;
				int xp = m_owner->m_tile_count_y - y;
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x1 = x0;
				y1 = (xp + yp) * 9 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y1;
				x3 = x2;
				y3 = y0;
				glColor4f(1.0F, 0.9F, 0.0F, 0.25F);
				Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 9 + m_owner->m_shift.y;
				x1 = (xp - yp) * 16 + 16 + m_owner->m_shift.x;
				y1 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y0;
				x3 = x1;
				y3 = (xp + yp) * 9 + m_owner->m_shift.y;
				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
				glBegin(GL_LINES);
				glColor4f(1.0, 1.0, 0.0, 1.0);
				glVertex2d(x0, y0);
				glVertex2d(x1, y1);
				glVertex2d(x1, y1);
				glVertex2d(x2, y2);
				glVertex2d(x2, y2);
				glVertex2d(x3, y3);
				glVertex2d(x3, y3);
				glVertex2d(x0, y0);
				glEnd();
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
			}
		}
	}
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
	for (int i = -1; i < by + 1; i++)
	{
		for (int j = -1; j < bx + 1; j++)
		{
			if (i == by || j == bx || i == -1 || j == -1)
			{
				int x = px + (Item->x + j) - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
				int y = py + (Item->y - i) - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
				int yp = m_owner->m_tile_count_x - x;
				int xp = m_owner->m_tile_count_y - y;
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x1 = x0;
				y1 = (xp + yp) * 9 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y1;
				x3 = x2;
				y3 = y0;
				glColor4f(1.0F, 0.9F, 0.0F, 0.25F);
				Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 9 + m_owner->m_shift.y;
				x1 = (xp - yp) * 16 + 16 + m_owner->m_shift.x;
				y1 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y0;
				x3 = x1;
				y3 = (xp + yp) * 9 + m_owner->m_shift.y;
				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
				glBegin(GL_LINES);
				glColor4f(0.0F, 1.0F, 0.0F, 0.75F);
				glVertex2d(x0, y0);
				glVertex2d(x1, y1);
				glVertex2d(x1, y1);
				glVertex2d(x2, y2);
				glVertex2d(x2, y2);
				glVertex2d(x3, y3);
				glVertex2d(x3, y3);
				glVertex2d(x0, y0);
				glEnd();
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
			}
		}
	}*/
	if (m_cell)
	{
		glUseProgramObjectARB(0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
		double x0, y0, x1, y1, x2, y2, x3, y3;
		
		int bx;
		int by;
		bx = m_object->m_active_state->m_size.x;
		by = m_object->m_active_state->m_size.y;
		bool valid = true;//((abs(m_object->cell()->x - m_owner->m_cursored->x) < 2) && (abs(m_object->cell()->y - m_owner->m_cursored->y) < 2));
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
		for (int i = 0; i < by; i++)
		{
			for (int j = 0; j < bx; j++)
			{
				int x = px + (m_cell->x + j) - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
				int y = py + (m_cell->y - i) - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
				int yp = m_owner->m_tile_count_x - x;
				int xp = m_owner->m_tile_count_y - y;
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x1 = x0;
				y1 = (xp + yp) * 9 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y1;
				x3 = x2;
				y3 = y0;
				if (valid)
				{
					glColor4f(0.0F, 1.0F, 0.0F, 0.25F);
				}
				else
				{
					glColor4f(1.0F, 0.0F, 0.0F, 0.25F);
				}
				Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 9 + m_owner->m_shift.y;
				x1 = (xp - yp) * 16 + 16 + m_owner->m_shift.x;
				y1 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y0;
				x3 = x1;
				y3 = (xp + yp) * 9 + m_owner->m_shift.y;
				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
				if (valid)
				{
					glColor4f(0.0, 1.0, 0.0, 1.0);
				}
				else
				{
					glColor4f(1.0, 0.0, 0.0, 1.0);
				}
				glBegin(GL_LINES);
				glVertex2d(x0, y0);
				glVertex2d(x1, y1);
				glVertex2d(x1, y1);
				glVertex2d(x2, y2);
				glVertex2d(x2, y2);
				glVertex2d(x3, y3);
				glVertex2d(x3, y3);
				glVertex2d(x0, y0);
				glEnd();
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
			}
		}
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
		for (int i = 0; i < by; i++)
		{
			for (int j = 0; j < bx; j++)
			{
				int x = px + (m_cell->x + j) - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
				int y = py + (m_cell->y - i) - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
				int yp = m_owner->m_tile_count_x - x;
				int xp = m_owner->m_tile_count_y - y;
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x1 = x0;
				y1 = (xp + yp) * 9 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y1;
				x3 = x2;
				y3 = y0;
				if (valid)
				{
					glColor4f(0.0F, 1.0F, 0.0F, 0.25F);
				}
				else
				{
					glColor4f(1.0F, 0.0F, 0.0F, 0.25F);
				}
				Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 9 + m_owner->m_shift.y;
				x1 = (xp - yp) * 16 + 16 + m_owner->m_shift.x;
				y1 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x2 = (xp - yp) * 16 + 32 + m_owner->m_shift.x;
				y2 = y0;
				x3 = x1;
				y3 = (xp + yp) * 9 + m_owner->m_shift.y;
				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
				if (valid)
				{
					glColor4f(0.0, 1.0, 0.0, 1.0);
				}
				else
				{
					glColor4f(1.0, 0.0, 0.0, 1.0);
				}
				glBegin(GL_LINES);
				glVertex2d(x0, y0);
				glVertex2d(x1, y1);
				glVertex2d(x1, y1);
				glVertex2d(x2, y2);
				glVertex2d(x2, y2);
				glVertex2d(x3, y3);
				glVertex2d(x3, y3);
				glVertex2d(x0, y0);
				glEnd();
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
			}
		}
		/*{
			dimension_t object_size;
			game_object_size_t size3d;
			object_size = m_object->m_active_state->m_tile_size;
			size3d = m_object->m_active_state->m_size;
			int x = px + (m_object->cell()->x-1) - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
			int y = py + (m_object->cell()->y+1) - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
			int yp = m_owner->m_tile_count_x - x;
			int xp = m_owner->m_tile_count_y - y;
			x0 = (xp - yp) * 16 + m_owner->m_shift.x;
			y0 = (xp + yp) * 9 - (54) + m_owner->m_shift.y + (2) * 9;
			x1 = x0;
			y1 = (xp + yp) * 9 + m_owner->m_shift.y + (2) * 9;
			x2 = (xp - yp) * 16 + 96 + m_owner->m_shift.x;
			y2 = y1;
			x3 = x2;
			y3 = y0;
			tile_t tile;
			int dx = 0;
			m_object->m_active_state->m_tile_manager->set_tile(tile, m_object, Application::instance().m_timer->m_tick / 7.0*3.0, dx);
			GLuint Sprite = tile.unit;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_dir);
			glColor4f(1.0F, 1.0F, 0.0F, 1.00F);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
			Application::instance().m_graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
			Application::instance().m_graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
		}*/
	//	if (valid)
	//	{
	//		glColor4f(0.0F, 1.0F, 0.0F, 0.75F);
	//	}
	//	else
	//	{
	//		glColor4f(1.0F, 0.0F, 0.0F, 0.75F);
	//	}
	//dimension_t object_size;
	//game_object_size_t size3d;
	//object_size = m_object->m_active_state->m_tile_size;
	//size3d = m_object->m_active_state->m_size;
	//int x = px + Item->x - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
	//int y = py + Item->y - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
	//int yp = m_owner->m_tile_count_x - x;
	//int xp = m_owner->m_tile_count_y - y;
	//x0 = (xp - yp) * 16 + m_owner->m_shift.x;
	//y0 = (xp + yp) * 9 - (object_size.h) + m_owner->m_shift.y + (size3d.x - 1) * 9;
	//x1 = x0;
	//y1 = (xp + yp) * 9 + m_owner->m_shift.y + (size3d.x - 1) * 9;
	//x2 = (xp - yp) * 16 + object_size.w + m_owner->m_shift.x;
	//y2 = y1;
	//x3 = x2;
	//y3 = y0;
	//tile_t tile;
	////int dx = 0;
	//m_object->m_active_state->m_tile_manager->set_tile(tile, m_object, Application::instance().m_timer->m_tick / 7.0*3.0, 0);
	//GLuint Sprite = tile.unit;
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, Sprite);
	//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
	//Application::instance().m_graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
	//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
	//Application::instance().m_graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
	}
}

void mapviewer_object_move::render_on_cell(MapCell* c)
{
	if (m_cell)
	{
		if (m_cell == c)
		{
			double x0, y0, x1, y1, x2, y2, x3, y3;
			glColor4f(0.0F, 1.0F, 0.0F, 0.75F);
			dimension_t object_size;
			game_object_size_t size3d;
			object_size = m_object->m_active_state->m_tile_size;
			size3d = m_object->m_active_state->m_size;
			int x = c->x - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
			int y = c->y - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
			int yp = m_owner->m_tile_count_x - x;
			int xp = m_owner->m_tile_count_y - y;
			x0 = (xp - yp) * 16 + m_owner->m_shift.x;
			y0 = (xp + yp) * 9 - (object_size.h) + m_owner->m_shift.y + (size3d.x - 1) * 9;
			x1 = x0;
			y1 = (xp + yp) * 9 + m_owner->m_shift.y + (size3d.x - 1) * 9;
			x2 = (xp - yp) * 16 + object_size.w + m_owner->m_shift.x;
			y2 = y1;
			x3 = x2;
			y3 = y0;
			tile_t tile;
			m_object->m_active_state->m_tile_manager->set_tile(tile, m_object, Application::instance().m_timer->m_tick / 7.0*3.0, Game_algorithm::turn_to_cell(m_object, m_owner->m_cursored));
			GLuint Sprite = tile.unit;
			glUseProgramObjectARB(0);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Sprite);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
			Application::instance().m_graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
			Application::instance().m_graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
		}
	}
}

mapviewer_object_rotate::mapviewer_object_rotate(GUI_MapViewer* owner, GameObject* object) : gui_mapviewer_hint(owner), m_object(object)
{ 
	m_fov = new FOV_help();
	init();
	
}

void mapviewer_object_rotate::init()
{
	if (m_owner->m_cursored)
	{
		m_direction =  Game_algorithm::turn_to_cell(m_object, m_owner->m_cursored);
	}
	else
	{
		m_direction = m_object->m_direction;
	}
	m_fov->calculate_FOV(m_object, m_object->cell()->m_map, m_direction);
}

void mapviewer_object_rotate::render()
{
	if (m_owner->m_cursored)
	{
		bool top_bound;
		bool down_bound;
		bool left_bound;
		bool right_bound;
		double xt, yt;
		int yp;
		int xp;
		MapCell* c;
		glUseProgramObjectARB(0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
		glLineWidth(4);
		glColor4f(0.0F, 1.0F, 0.0F, 0.25F);
		for (int y = m_fov->m_map_center.y - m_fov->m_view.d, yf = m_fov->m_view_center.y - m_fov->m_view.d; y < m_fov->m_map_center.y + m_fov->m_view.u + 1; y++, yf++)
		{
			for (int x = m_fov->m_map_center.x - m_fov->m_view.l, xf = m_fov->m_view_center.x - m_fov->m_view.l; x < m_fov->m_map_center.x + m_fov->m_view.r + 1; x++, xf++)
			{
				if (m_fov->m_map[yf][xf].visible)
				{  
					c = m_owner->m_map->m_items[y][x];
					yp = m_owner->m_tile_count_x / 2 - c->x + m_owner->m_center.x;
					xp = m_owner->m_tile_count_y / 2 - c->y + m_owner->m_center.y;
					xt = (xp - yp) * 16 + m_owner->m_shift.x;
					yt = (xp + yp) * 9 + m_owner->m_shift.y;
					Application::instance().m_graph->draw_sprite(xt, yt - 18, xt, yt, xt + 32, yt, xt + 32, yt - 18);
					glDisable(GL_BLEND);
					glDisable(GL_TEXTURE_2D);
					top_bound = false;
					if (yf == m_fov->m_view_center.y + m_fov->m_view.u)
					{
						top_bound = true;
					}
					else
					{
						if (!m_fov->m_map[yf + 1][xf].visible) { top_bound = true; }
					}

					right_bound = false;
					if (xf == m_fov->m_view_center.x + m_fov->m_view.r)
					{
						right_bound = true;
					}
					else
					{
						if (!m_fov->m_map[yf][xf + 1].visible) { right_bound = true; }
					}

					down_bound = false;
					if (yf == m_fov->m_view_center.y - m_fov->m_view.d)
					{
						down_bound = true;
					}
					else
					{
						if (!m_fov->m_map[yf - 1][xf].visible) { down_bound = true; }
					}

					left_bound = false;
					if (xf == m_fov->m_view_center.x - m_fov->m_view.l)
					{
						left_bound = true;
					}
					else
					{
						if (!m_fov->m_map[yf][xf - 1].visible) { left_bound = true; }
					}
					glBegin(GL_LINES);
					if (top_bound)
					{
						glVertex2d(xt, yt - 9);
						glVertex2d(xt + 16, yt - 18);
					}
					if (right_bound)
					{
						glVertex2d(xt + 16, yt - 18);
						glVertex2d(xt + 32, yt-9);
					}
					if (down_bound)
					{
						glVertex2d(xt + 32, yt-9);
						glVertex2d(xt + 16, yt);
					}
					if (left_bound)
					{
						glVertex2d(xt + 16, yt);
						glVertex2d(xt, yt - 9);
					}
					glEnd();

					glEnable(GL_BLEND);
					glEnable(GL_TEXTURE_2D);
				}
			}
		}
		glLineWidth(1);
	}
}

void mapviewer_object_rotate::render_on_cell(MapCell* c)
{
	if (m_owner->m_cursored)
	{
		if (c == m_object->cell())
		{
			double x0, y0, x1, y1, x2, y2, x3, y3;
			glColor4f(0.0F, 1.0F, 0.0F, 0.75F);
			dimension_t object_size;
			game_object_size_t size3d;
			object_size = m_object->m_active_state->m_tile_size;
			size3d = m_object->m_active_state->m_size;
			int x = c->x - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
			int y = c->y - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
			int yp = m_owner->m_tile_count_x - x;
			int xp = m_owner->m_tile_count_y - y;
			x0 = (xp - yp) * 16 + m_owner->m_shift.x;
			y0 = (xp + yp) * 9 - (object_size.h) + m_owner->m_shift.y + (size3d.x - 1) * 9;
			x1 = x0;
			y1 = (xp + yp) * 9 + m_owner->m_shift.y + (size3d.x - 1) * 9;
			x2 = (xp - yp) * 16 + object_size.w + m_owner->m_shift.x;
			y2 = y1;
			x3 = x2;
			y3 = y0;
			tile_t tile;
			m_object->m_active_state->m_tile_manager->set_tile(tile, m_object, Application::instance().m_timer->m_tick / 7.0*3.0, m_direction);
			GLuint Sprite = tile.unit;
			glUseProgramObjectARB(0);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Sprite);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
			Application::instance().m_graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
			Application::instance().m_graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
		}
	}
}

GUI_MapViewer::GUI_MapViewer(Application* app)
{
	m_tile_count_x = 90;
	m_tile_count_y = 90;
	m_just_focused = false;
	m_is_moving = false;
	m_focus = nullptr;
	m_cursored = nullptr;
	m_cursor_x = 1;
	m_cursor_y = 1;
	start_moving += std::bind(&GUI_MapViewer::on_start_moving, this, std::placeholders::_1);
	move += std::bind(&GUI_MapViewer::on_move, this, std::placeholders::_1);
	end_moving += std::bind(&GUI_MapViewer::on_end_moving, this, std::placeholders::_1);
	m_size = app->m_size;
	m_shift.x = (m_size.w / 2) - 16;// (m_tile_count_x)* 32 * 0.5;
	m_shift.y = (m_size.h / 2) - (m_tile_count_y)* 9 - 9;// 1024 * 0.5 - (m_tile_count_y + 6) * 18 * 0.5;
}

GUI_MapViewer::~GUI_MapViewer(void)
{
}

void GUI_MapViewer::calculate()
{
	m_tile_size_x = 16;
	m_tile_size_y = 9;
}

void GUI_MapViewer::update()
{
	calculate();
	/*int x;
	int y;
	for (int i = 0; i<m_tile_count_y; i++)
	{
		for (int j = 0; j<m_tile_count_x; j++)
		{
			x = m_center.x + j - m_tile_count_x / 2;
			y = m_center.y + i - m_tile_count_y / 2;
			if((x<0)||(x>m_map->m_size.w-1)||(y<0)||(y>m_map->m_size.h-1))
			{
				m_items[i][j]->m_map_element=nullptr;
			} else {
				m_items[i][j]->m_map_element=m_map->m_items[y][x];
			}
		}
	}*/
}

bool GUI_MapViewer::select(int x, int y)
{
	//int CellX=x/32;
	//int CellY=63-y/16;
	////char buf2[32];
	////itoa(CellX, buf2, 10);
	////MessageBox(0, buf2, "X", MB_OK);
	////itoa(CellY, buf2, 10);
	////MessageBox(0, buf2, "Y", MB_OK);
	//if(Items[CellY][CellX]!=nullptr)
	//{
	//	Items[CellY][CellX]->IsSelected=!Items[CellY][CellX]->IsSelected;
	//}
	return false;
}

void GUI_MapViewer::render(GraphicalController* Graph, int px, int py)
{
	float light[4];
	glBindFramebuffer(GL_FRAMEBUFFER, Graph->m_FBO);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glUseProgram(0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_02, 0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_03, 0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	int x0, y0, x1, y1, x2, y2, x3, y3;
	bool IsDraw;
	tile_t tile;
	int x;
	int y;
	int xf;
	int yf;

	int xv;
	int yv;

	int dx;
	glColor4d(1.0, 1.0, 1.0, 1.0);
	dimension_t object_size;
	game_object_size_t size3d;

	int j_limit;
	int gx;
	int gy;
	bool passable;
	bool is_hide;
	bool is_in_fov;
	for (auto current = m_hints.begin(); current != m_hints.end(); current++)
	{
		(*current)->init();
	}

	bool m_observer;
	
	AI_enemy* ai = static_cast<AI_enemy*>(m_player->m_object->m_active_state->m_ai);
	Vision_list* vl = static_cast<Vision_list*>(m_player->m_object->m_active_state->get_list(interaction_e::vision));
	m_observer = !vl;
	int max_fov_radius;
	view_t fov;
	position_t posc;
	if (!m_observer)
	{
		max_fov_radius = vl->m_max_radius;
		fov = ai->m_fov->m_view;
		posc=position_t((m_player->m_object->m_active_state->m_size.x - 1) >> 1, (m_player->m_object->m_active_state->m_size.y - 1) >> 1);
	}
	for (int r = 0; r < 2; r++)
	{
		for (int i = 0; i < (m_tile_count_x + m_tile_count_y) - 1; i++)
		{
			if (i < m_tile_count_y)
			{
				j_limit = i + 1;
				gx = m_tile_count_x;
				gy = m_tile_count_y - i - 2;
			}
			else
			{
				j_limit = (m_tile_count_x + m_tile_count_y) - i;
				gx = (m_tile_count_x + m_tile_count_y) - i;
				gy = -1;
			}
			for (int j = 0; j < j_limit; j++)
			{
				gx -= 1;
				gy += 1;
				x = m_center.x + gx - m_tile_count_x / 2;
				y = m_center.y + gy - m_tile_count_y / 2;
				xf = x - m_player->m_object->cell()->x - posc.x;
				yf = y - m_player->m_object->cell()->y + posc.y;
				if ((x > -1) && (x<m_map->m_size.w) && (y>-1) && (y < m_map->m_size.h))
				{
					if (m_observer)
					{
						is_in_fov = true;
					}
					else
					{
						if ((xf >= -fov.l) && (xf <= fov.r) && (yf >= -fov.d) && (yf <= fov.u))
						{
							is_in_fov = true;
						}
						else
						{
							is_in_fov = false;
						}
					}

					light[0] = (m_map->m_items[y][x]->m_light.R / 100.0F);
					light[1] = (m_map->m_items[y][x]->m_light.G / 100.0F);
					light[2] = (m_map->m_items[y][x]->m_light.B / 100.0F);
					light[3] = 0.0;
					/*light[0] = 1.0;
					light[1] = 1.0;
					light[2] = 1.0;
					light[3] = 0.0;*/
					for (std::list<GameObject*>::iterator Current = m_map->m_items[y][x]->m_items.begin(); Current != m_map->m_items[y][x]->m_items.end(); Current++)
					{
						is_hide = false;
						IsDraw = false;
						passable = (*Current)->get_stat(object_tag_e::pass_able);
						if ((r == 0 && passable) || (r == 1 && !passable))
						{
							if ((*Current)->cell()->y == m_map->m_items[y][x]->y && (*Current)->cell()->x == m_map->m_items[y][x]->x)
							{
								//IsDraw = true;
								size3d = (*Current)->m_active_state->m_size;
								if (m_observer)
								{
									IsDraw = true;
								}
								else
								{
									if (is_in_fov)
									{
										if (ai->m_fov->m_map[fov.d + yf][fov.l + xf].visible)
										{

											IsDraw = true;
										}
									}
								}

								if (!IsDraw)
								{
									if (size3d.x > 1 || size3d.y > 1)
									{
										for (int m = 0; m < size3d.y; m++)
										{
											for (int n = 0; n < size3d.x; n++)
											{
												if ((xf + n >= -fov.l) && (xf + n <= fov.r) && (yf - m >= -fov.d) && (yf - m <= fov.u))
												{
													if (ai->m_fov->m_map[fov.d + yf - m][fov.l + xf + n].visible)
													{
														IsDraw = true;
													}
												}
											}
										}
									}
									else if (m_map->m_items[y][x]->m_notable)
									{
										if (((*Current)->m_name == "floor") || ((*Current)->m_name == "wall"))
										{
											IsDraw = true;
											is_hide = true;
										}
									}
								}
							}
						}
						if (IsDraw)
						{
							object_size = (*Current)->m_active_state->m_tile_size;
							int yp = m_tile_count_x - px - gx;
							int xp = m_tile_count_y - py - gy;
							dx = object_size.w + m_map->m_items[y][x]->x - (*Current)->cell()->x;
							x0 = (xp - yp) * 16 + m_shift.x;
							y0 = (xp + yp) * 9 - (object_size.h) + m_shift.y + (size3d.x - 1) * 9;
							x1 = x0;
							y1 = (xp + yp) * 9 + m_shift.y + (size3d.x - 1) * 9;
							x2 = (xp - yp) * 16 + object_size.w + m_shift.x;
							y2 = y1;
							x3 = x2;
							y3 = y0;
							// считаем среднюю освещенность для многотайловых объектов
							if (size3d.x > 1 || size3d.y > 1)
							{
								light[0] = 0;
								light[1] = 0;
								light[2] = 0;
								for (int m = 0; m < size3d.y; m++)
								{
									for (int n = 0; n < size3d.x; n++)
									{
										light[0] += m_map->m_items[y - m][x + n]->m_light.R;
										light[1] += m_map->m_items[y - m][x + n]->m_light.G;
										light[2] += m_map->m_items[y - m][x + n]->m_light.B;
									}
								}
								light[0] = light[0] / (size3d.x*size3d.y*100.0F);
								light[1] = light[1] / (size3d.x*size3d.y*100.0F);
								light[2] = light[2] / (size3d.x*size3d.y*100.0F);
								/*		light[0] = 1.0;
										light[1] = 1.0;
										light[2] = 1.0;
										light[3] = 0.0;*/
							}

							// Подсветка тайлов юнитов
							if ((*Current)->m_name == "bat" || (*Current)->m_name == "skeleton" || (*Current)->m_name == "bear" || (*Current)->m_name == "rat" || (*Current)->m_name == "iso_unit")
							{
								auto a = new mapviewer_hint_object_area(this, (*Current));
								a->render();
							}
							if (is_hide)
							{
								glUseProgram(Graph->m_tile_shader_hide);
							}
							else
							{
								glUseProgram(Graph->m_tile_shader);
							}
							if ((*Current)->m_active_state->m_layer == 2)
							{
								glUseProgram(Graph->m_tile_shader_alpha);
							}
							(*Current)->m_active_state->m_tile_manager->set_tile(tile, (*Current), Application::instance().m_timer->m_tick / 7.0*3.0, dx);
							GLuint Sprite = tile.unit;
							glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, Sprite);
							Graph->set_uniform_sampler(Graph->m_tile_shader, "Map", 0);
							Graph->set_uniform_vector(Graph->m_tile_shader, "light", light);
							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
							Graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);

							if ((*Current)->cell()->x == m_map->m_items[y][x]->x)
							{
								/*auto* feat = (*Current)->get_parameter(interaction_e::health);
								if (feat)
								{
								float x0, y0, x1, y1, x2, y2, x3, y3;
								float h = feat->m_basic_value / 100.0;
								x0 = (px + gx - object_size.w / 2.0 + 1)*m_tile_size_x - 32;
								y0 = (m_tile_count_y - py - gy - object_size.h)*m_tile_size_y - 8;
								x1 = x0;
								y1 = (m_tile_count_y - py - gy - object_size.h)*m_tile_size_y;
								x2 = (px + gx - object_size.w / 2.0 + 1)*m_tile_size_x + 32;
								y2 = y1;
								x3 = x2;
								y3 = y0;
								glUseProgram(0);
								glDisable(GL_TEXTURE_2D);
								glColor4d(1.0 - h, h, 0.0, 0.5);
								glBegin(GL_LINES);
								glVertex2d(x0, y0);
								glVertex2d(x1, y1);
								glVertex2d(x1, y1);
								glVertex2d(x2, y2);
								glVertex2d(x2, y2);
								glVertex2d(x3, y3);
								glVertex2d(x3, y3);
								glVertex2d(x0, y0);
								glEnd();
								x0 = (px + gx - object_size.w / 2.0 + 1)*m_tile_size_x - 32;
								x1 = x0;
								x2 = x0 + (h)* 64;
								x3 = x2;
								Graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
								glEnable(GL_TEXTURE_2D);
								}*/
							}

							if ((*Current)->m_active_state->m_layer == 1)
							{
								if (is_hide)
								{
									glUseProgram(Graph->m_tile_shader_hide);
								}
								else
								{
									glUseProgram(Graph->m_tile_shader);
								}
								glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_02, 0);
								Graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);

								/*glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
								glUseProgram(Graph->m_mask_shader2);
								glActiveTexture(GL_TEXTURE0);
								glBindTexture(GL_TEXTURE_2D, Graph->m_empty_03);
								Graph->set_uniform_sampler(Graph->m_mask_shader2, "Map", 0);
								glActiveTexture(GL_TEXTURE1);
								glBindTexture(GL_TEXTURE_2D, Sprite);
								Graph->set_uniform_sampler(Graph->m_mask_shader2, "Unit", 1);
								glActiveTexture(GL_TEXTURE0);
								Graph->draw_tile_FBO(x1 / 1024.0, (1024 - y1) / 1024.0, x3 / 1024.0, (1024 - y3) / 1024.0, x0, y0, x1, y1, x2, y2, x3, y3);*/
							}

							if ((*Current)->m_active_state->m_layer == 2)
							{
								/*glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
								glUseProgram(Graph->m_mask_shader2);
								glActiveTexture(GL_TEXTURE0);
								glBindTexture(GL_TEXTURE_2D, Graph->m_empty_03);
								Graph->set_uniform_sampler(Graph->m_mask_shader2, "Map1", 0);
								glActiveTexture(GL_TEXTURE1);
								glBindTexture(GL_TEXTURE_2D, Sprite);
								Graph->set_uniform_sampler(Graph->m_mask_shader2, "Unit", 1);
								glActiveTexture(GL_TEXTURE2);
								glBindTexture(GL_TEXTURE_2D, Graph->m_empty_02);
								Graph->set_uniform_sampler(Graph->m_mask_shader2, "Map2", 2);
								glActiveTexture(GL_TEXTURE0);
								Graph->draw_tile_FBO(x1 /(float)m_size.w, (m_size.h - y1) / (float)m_size.h, x3 / (float)m_size.w, (m_size.h - y3) /(float) m_size.h, x0, y0, x1, y1, x2, y2, x3, y3);

								glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
								glUseProgram(Graph->m_mask_shader);
								glActiveTexture(GL_TEXTURE0);
								glBindTexture(GL_TEXTURE_2D, Graph->m_empty_02);
								Graph->set_uniform_sampler(Graph->m_mask_shader, "Map", 0);
								glActiveTexture(GL_TEXTURE1);
								glBindTexture(GL_TEXTURE_2D, Sprite);
								Graph->set_uniform_sampler(Graph->m_mask_shader, "Unit", 1);
								glActiveTexture(GL_TEXTURE0);
								Graph->draw_tile_FBO(x1 / (float)m_size.w, (m_size.h - y1) / (float)m_size.h, x3 / (float)m_size.w, (m_size.h - y3) / (float)m_size.h, x0, y0, x1, y1, x2, y2, x3, y3);*/
							}

						}
					}
					for (auto current = m_hints.begin(); current != m_hints.end(); current++)
					{
						(*current)->render_on_cell(m_map->m_items[y][x]);
					}
				}
			}
		}
		if (r == 0)
		{
			for (std::list<gui_mapviewer_hint*>::iterator current = m_hints.begin(); current != m_hints.end(); current++)
			{
				if (!(*current)->m_top)
				{
					(*current)->render();
				}
			}
		}
		else
		{
			for (std::list<gui_mapviewer_hint*>::iterator current = m_hints.begin(); current != m_hints.end(); current++)
			{
				if ((*current)->m_top)
				{
					(*current)->render();
				}
			}
		}
	}
	if (m_cursored != nullptr)
	{
		MapCell* Item = m_cursored;
		int x = px + Item->x - m_center.x + m_tile_count_x / 2;
		int y = py + Item->y - m_center.y + m_tile_count_y / 2;
		//x0 = (x - m_cursor_x + 1) * m_tile_size_x;
		//y0 = (m_tile_count_y - y - m_cursor_y) * m_tile_size_y;
		//x1 = (x - m_cursor_x + 1) * m_tile_size_x;
		//y1 = (m_tile_count_y - y) * m_tile_size_y;
		//x2 = (x + 1) * m_tile_size_x;
		//y2 = (m_tile_count_y - y) * m_tile_size_y;
		//x3 = (x + 1) * m_tile_size_x;
		//y3 = (m_tile_count_y - y - m_cursor_y) * m_tile_size_y;
		int yp = m_tile_count_x - x;
		int xp = m_tile_count_y - y;
		x0 = (xp - yp) * 16 + m_shift.x;
		y0 = (xp + yp) * 9 - 18 + m_shift.y;
		x1 = x0;
		y1 = (xp + yp) * 9 + m_shift.y;
		x2 = (xp - yp) * 16 + 32 + m_shift.x;
		y2 = y1;
		x3 = x2;
		y3 = y0;
		glUseProgram(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Graph->m_select);
		glColor4f(0.0, 1.0, 0.0, 0.25);
		Graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
		x0 = (xp - yp) * 16 + m_shift.x;
		y0 = (xp + yp) * 9 - 9 + m_shift.y;
		x1 = (xp - yp) * 16 + 16 + m_shift.x;
		y1 = (xp + yp) * 9 - 18 + m_shift.y;
		x2 = (xp - yp) * 16 + 32 + m_shift.x;
		y2 = y0;
		x3 = x1;
		y3 = (xp + yp) * 9 + m_shift.y;
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
		glColor4f(0.0, 1.0, 0.0, 1.0);
		glVertex2d(x0, y0);
		glVertex2d(x1, y1);
		glVertex2d(x1, y1);
		glVertex2d(x2, y2);
		glVertex2d(x2, y2);
		glVertex2d(x3, y3);
		glVertex2d(x3, y3);
		glVertex2d(x0, y0);
		glEnd();
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		/*	glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_LINES);
			glColor4f(0.0, 1.0, 0.0, 1.0);
			glVertex2d(x0, y0);
			glVertex2d(x1, y1);
			glVertex2d(x1, y1);
			glVertex2d(x2, y2);
			glVertex2d(x2, y2);
			glVertex2d(x3, y3);
			glVertex2d(x3, y3);
			glVertex2d(x0, y0);
			glEnd();*/
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0, 1.0, 1.0, 1.0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	glUseProgram(0);
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, Graph->m_empty_01);
	glColor4d(1.0, 1.0, 1.0, 1.0);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//Graph->draw_sprite(0, 0, 0, 1024/4, 1024/4, 1024/4, 1024/4, 0);
	//Graph->blur_rect(0, 0, 1024 / 4, 1024 / 4);
	//glBindTexture(GL_TEXTURE_2D, Graph->m_blur);
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 1024 - 1024 / 4, 1024 /4, 1024 /4);
	Graph->draw_sprite(0, 0, 0, m_size.h, m_size.w, m_size.h, m_size.w, 0);
	glDisable(GL_TEXTURE_2D);
}

void GUI_MapViewer::on_key_press(WPARAM w)
{
	Parameter_Position* P;
	switch (w)
	{
	case VK_ESCAPE:
		PostQuitMessage(0);
		return;
		//case VK_UP:
		//	P = new Parameter_Position();
		//	P->m_object=m_player;
		//	P->m_place=m_map->m_items[m_player->cell()->y+1][m_player->cell()->x];
		//	P->m_map=m_map;
		//	Application::instance().m_action_manager->m_items.push_back(new GameTask(Application::instance().m_actions[action_e::move], P));
		//	P->~Parameter_Position();
		//	//app->TurnEnd=true;
		//	return;
		//case VK_DOWN:
		//	P = new Parameter_Position();
		//	P->m_object=m_player;
		//	P->m_place=m_map->m_items[m_player->cell()->y-1][m_player->cell()->x];
		//	P->m_map=m_map;
		//	Application::instance().m_action_manager->m_items.push_back(new GameTask(Application::instance().m_actions[action_e::move], P));
		//	P->~Parameter_Position();
		//	//app->TurnEnd=true;
		//	return;
		//case VK_LEFT:
		//	P = new Parameter_Position();
		//	P->m_object=m_player;
		//	P->m_place=m_map->m_items[m_player->cell()->y][m_player->cell()->x-1];
		//	P->m_map=m_map;
		//	Application::instance().m_action_manager->m_items.push_back(new GameTask(Application::instance().m_actions[action_e::move], P));
		//	P->~Parameter_Position();
		//	//app->TurnEnd=true;
		//	return;
		//case VK_RIGHT:
		//	P = new Parameter_Position();
		//	P->m_object=m_player;
		//	P->m_place=m_map->m_items[m_player->cell()->y][m_player->cell()->x+1];
		//	P->m_map=m_map;
		//	Application::instance().m_action_manager->m_items.push_back(new GameTask(Application::instance().m_actions[action_e::move], P));
		//	P->~Parameter_Position();
		//	//app->TurnEnd=true;
		//	return;
	case 0x59:
	{
		if (Application::instance().m_message_queue.m_reader)
		{
			Parameter* p = new Parameter(parameter_accept);
			Application::instance().m_message_queue.push(p);
		}
		return;
	}
	case VK_SPACE:
	{
		if (Application::instance().m_message_queue.m_reader)
		{
			Parameter* p = new Parameter(ParameterKind_Cancel);
			Application::instance().m_message_queue.push(p);
		}
		return;
	}
	}
}

void GUI_MapViewer::on_mouse_click(MouseEventArgs const& e)
{
	if (e.key == mk_left)
	{
		position_t p = local_xy(position_t(e.position.x, e.position.y));
		int x;
		int y;
		x = m_center.x + p.x - m_tile_count_x / 2;
		y = m_center.y + p.y - m_tile_count_y / 2;
		if (!m_just_focused)
		{
			if (!((x<0) || (x>m_map->m_size.w - 1) || (y<0) || (y>m_map->m_size.h - 1)))
			{
				P_object_owner* pr = new P_object_owner();
				pr->m_cell = m_map->m_items[y][x];
				if (Application::instance().m_message_queue.m_reader)
				{
					Application::instance().m_message_queue.push(pr);
				}
			}
		}
		m_just_focused = false;
	}
	else {
		Select_object_popmenu *PopMenu;
		PopMenu = new Select_object_popmenu();
		PopMenu->m_position.x = e.position.x;
		PopMenu->m_position.y = e.position.y;
		position_t p = local_xy(position_t(e.position.x, e.position.y));
		int x;
		int y;
		x = m_center.x + p.x - m_tile_count_x / 2;
		y = m_center.y + p.y - m_tile_count_y / 2;
		if (!((x<0) || (x>m_map->m_size.w - 1) || (y<0) || (y>m_map->m_size.h - 1)))
		{
			//UnderCursor(CursorEventArgs(e.x, e.y));
			for (std::list<GameObject*>::iterator Current = m_map->m_items[y][x]->m_items.begin(); Current != m_map->m_items[y][x]->m_items.end(); Current++)
			{
				PopMenu->add((*Current)->m_name, (*Current));
			}
		}
		if (PopMenu->m_items.size() == 0)
		{
			PopMenu->add("Нет действий", nullptr);
		}
		PopMenu->destroy += std::bind(&GUI_Layer::remove, m_GUI, std::placeholders::_1);
		m_GUI->add_front(PopMenu);
		PopMenu->m_items.front()->set_focus(true);
	}
}

void GUI_MapViewer::on_mouse_down(MouseEventArgs const& e)
{
	if (e.key == mk_left || e.key == mk_right)
	{
		position_t p = local_xy(e.position);
		int x;
		int y;
		x = m_center.x + p.x - m_tile_count_x / 2;
		y = m_center.y + p.y - m_tile_count_y / 2;

		if (!((x<0) || (x>m_map->m_size.w - 1) || (y<0) || (y>m_map->m_size.h - 1)))
		{
			set_focus(true);
			//on_mouse_click(e);
		}

	}
}

void GUI_MapViewer::on_mouse_wheel(MouseEventArgs const& e)
{
	if (e.value > 0)
	{
		m_tile_count_x += 2;
		m_tile_count_y += 4;
		if (m_tile_count_x > 64) m_tile_count_x = 64;
		if (m_tile_count_y > 128) m_tile_count_y = 128;
	}
	else
	{
		m_tile_count_x -= 2;
		m_tile_count_y -= 4;
		if (m_tile_count_x < 16) m_tile_count_x = 16;
		if (m_tile_count_y < 32) m_tile_count_y = 32;
	}
	calculate();
	position_t p = local_xy(position_t(e.position.x, e.position.y));
	const int x = m_center.x + p.x - m_tile_count_x / 2;
	const int y = m_center.y + p.y - m_tile_count_y / 2;
	if (!(x < 0 || x > m_map->m_size.w - 1 || y < 0 || y > m_map->m_size.h - 1))
	{
		m_cursored = m_map->m_items[y][x];
	}
}

void GUI_MapViewer::set_focus(bool state)
{
	if (!focused&&state)
	{
		m_just_focused = true;
	}
	if (!state)
	{
		m_just_focused = false;
	}
	focused = state;
	if (state)
	{
		if (m_focus != nullptr)
		{
			m_focus->set_focus(false);
		}
		get_focus(this);
	}
	else {
		if (m_focus != nullptr)
		{
			m_focus->set_focus(false);
		}
		lose_focus(this);
	}
}

void GUI_MapViewer::on_item_get_focus(GUI_Object* sender)
{
	if (m_focus != sender)
	{
		set_focus(true);
		m_focus = sender;
	}
}

void GUI_MapViewer::on_lose_focus(GUI_Object* sender)
{
	m_cursored = nullptr;
}

position_t GUI_MapViewer::local_xy(position_t p)
{
	position_t Result = position_t(m_tile_count_x - (m_tile_count_y - (p.x - m_shift.x) / 32 - m_tile_count_x + (p.y - m_shift.y) / 18 + 1), m_tile_count_y - ((p.x - m_shift.x) / 32 + (p.y - m_shift.y) / 18));
	if (Result.x > m_tile_count_x - 1) { Result.x = m_tile_count_x - 1; }
	if (Result.x < 0) { Result.x = 0; }
	if (Result.y > m_tile_count_y - 1) { Result.y = m_tile_count_y - 1; }
	if (Result.y < 0) { Result.y = 0; }
	return Result;
}
void GUI_MapViewer::on_mouse_move(MouseEventArgs const& e)
{
	position_t p = local_xy(e.position);
	const int x = m_center.x + p.x - m_tile_count_x / 2;
	const int y = m_center.y + p.y - m_tile_count_y / 2;
	if (!(x < 0 || x > m_map->m_size.w - 1 || y < 0 || y > m_map->m_size.h - 1))
	{
		m_cursored = m_map->m_items[y][x];
	}
	if (e.key == mk_left)
	{
		if (!m_is_moving) start_moving(e);
		move(e);
	}
	else
	{
		if (m_is_moving) end_moving(e);
		MouseEventArgs LocalMouseEventArgs = set_local_mouse_control(e);
		if (m_focus)
		{
			m_focus->mouse_move(LocalMouseEventArgs);
		}
	}
}

void GUI_MapViewer::on_start_moving(MouseEventArgs const& e)
{
	m_is_moving = true;
	position_t p = local_xy(position_t(e.position.x, e.position.y));
	m_initial_position.x = m_center.x - p.x;
	m_initial_position.y = m_center.y - p.y;
	//MessageBox(NULL, "Start", "", MB_OK);
}

void GUI_MapViewer::on_move(MouseEventArgs const& e)
{
	position_t p = local_xy(position_t(e.position.x, e.position.y));
	m_center.x = m_initial_position.x + p.x;
	m_center.y = m_initial_position.y + p.y;
}

void GUI_MapViewer::on_end_moving(MouseEventArgs const& e)
{
	m_is_moving = false;
	//MessageBox(NULL, "End", "", MB_OK);

}

MouseEventArgs GUI_MapViewer::set_local_mouse_control(MouseEventArgs const& source)
{
	return MouseEventArgs(position_t(source.position.x - m_position.x, source.position.y - m_position.y), source.key, source.value);
}