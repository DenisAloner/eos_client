#include "game/graphics/GUI_MapViewer.h"
#include "game/graphics/GUI_Layer.h"
#include "game/graphics/GUI_PopMenu.h"
#include "game/ActionManager.h"
#include "game/utils/Logger.h"
#include <algorithm>

gui_mapviewer_hint::gui_mapviewer_hint() { m_top = false; }
mapviewer_hint_path::mapviewer_hint_path(std::vector<MapCell*>* path,GameObject* object) :  m_object(object),m_path(path) {};
mapviewer_hint_area::mapviewer_hint_area(GameObject* object, bool consider_object_size) :  m_consider_object_size(consider_object_size), m_object(object) {}
mapviewer_hint_object_area::mapviewer_hint_object_area(GameObject* object) : m_object(object) {}
mapviewer_hint_line::mapviewer_hint_line(MapCell* cell, GameObject* object) : m_cell(cell),m_object(object) {}

void gui_mapviewer_hint::draw_cell(GUI_MapViewer* owner,MapCell* cell, int index)
{
	const auto px = 0;
	const auto py = 0;
	const auto x = px + cell->x - owner->m_center.x + owner->m_tile_count_x / 2;
	const auto y = py + cell->y - owner->m_center.y + owner->m_tile_count_y / 2;
	const auto yp = owner->m_tile_count_x - x;
	const auto xp = owner->m_tile_count_y - y;

	
	const GraphicalController::rectangle_t r((xp - yp) * tile_size_x_half + owner->m_shift.x, (xp + yp - cell->z * 2) * tile_size_y_half + owner->m_shift.y, tile_size_x, -tile_size_y);
	//glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
	//glColor4f(1.0F, 0.9F, 0.0F, 0.5F);
	//Application::instance().m_graph->draw_sprite(r);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0F, 0.9F, 0.0F, 1.0F);
	Application::instance().m_graph->center_text((r.a.x + r.b.x)*0.5, (r.a.y + r.b.y) *0.5, cp1251_to_utf16(std::to_string(index)), 8, 17);

	glDisable(GL_TEXTURE_2D);
	Application::instance().m_graph->stroke_cube(xp, yp, cell->z, int(owner->m_shift.x), int(owner->m_shift.y));
}

void mapviewer_hint_path::render(GUI_MapViewer* owner)
{
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (owner->m_cursored != nullptr){
		if (m_path){
			for (std::size_t i = 0; i < m_path->size(); i++){
				draw_cell(owner,(*m_path)[m_path->size() - 1 - i], i);
			}
		}
	}
}

void mapviewer_hint_object_area::render(GUI_MapViewer* owner)
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
	MapCell* Item = m_object->cell();
	int bx;
	int by;
	bx = m_object->m_active_state->m_size.dx;
	by = m_object->m_active_state->m_size.dy;
	for (int i = 0; i < by; i++)
	{
		for (int j = 0; j < bx; j++)
		{
			int x = px + (Item->x + j) - owner->m_center.x + owner->m_tile_count_x / 2;
			int y = py + (Item->y - i) - owner->m_center.y + owner->m_tile_count_y / 2;
			int yp = owner->m_tile_count_x - x;
			int xp = owner->m_tile_count_y - y;
			GraphicalController::rectangle_t r((xp - yp) * tile_size_x_half + owner->m_shift.x, (xp + yp) * tile_size_y_half + owner->m_shift.y, tile_size_x, -tile_size_y);
			glColor4f(1.0F, 0.9F, 0.0F, 0.25F);
			Application::instance().m_graph->draw_sprite(r);
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glColor4f(1.0, 1.0, 0.0, 1.0);
			Application::instance().m_graph->stroke_cell(xp, yp, owner->m_shift.x, owner->m_shift.y);
			glEnable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);
		}
	}
}

void mapviewer_hint_line::draw_cell(GUI_MapViewer* owner,MapCell* a)
{
	const auto px = 0;
	const auto py = 0;
	const auto x = px + a->x - owner->m_center.x + owner->m_tile_count_x / 2;
	const auto y = py + a->y - owner->m_center.y + owner->m_tile_count_y / 2;
	const auto yp = owner->m_tile_count_x - x;
	const auto xp = owner->m_tile_count_y - y;
	const GraphicalController::rectangle_t r((xp - yp) * tile_size_x_half + owner->m_shift.x, (xp + yp) * tile_size_y_half + owner->m_shift.y, tile_size_x, -tile_size_y);
	glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
	glColor4f(1.0F, 0.9F, 0.0F, 0.5F);
	Application::instance().m_graph->draw_sprite(r);
	glColor4f(1.0F, 0.9F, 0.0F, 1.0F);
	Application::instance().m_graph->center_text((r.a.x + r.b.x)*0.5, (r.a.y + r.b.y) *0.5, cp1251_to_utf16(std::to_string(m_step_count)), 8, 17);
	m_step_count += 1;
}

void mapviewer_hint_line::render(GUI_MapViewer* owner) {
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	if (owner->m_cursored != nullptr) {
		m_step_count = 0;
		Path::instance().map_costing(m_cell->m_map, m_object, owner->m_cursored, 40);
		const auto path = Path::instance().get_path_to_cell();
		if (path) {
			for (std::size_t i = 0; i < path->size(); ++i) {
				draw_cell(owner, (*path)[(path->size() - 1) - i]);
			}
			Path::instance().m_heap.m_items.clear();
		}
	}
}

mapviewer_hint_weapon_range::mapviewer_hint_weapon_range( GameObject* object, int range) :m_object(object), m_range(range) {}

void mapviewer_hint_area::render(GUI_MapViewer* owner)
{
	int px = 0;
	int py = 0;
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
	MapCell* Item = m_object->cell();
	int bx;
	int by;
	if (m_consider_object_size)
	{
		bx = m_object->m_active_state->m_size.dx;
		by = m_object->m_active_state->m_size.dy;
	}
	else
	{
		bx = 1;
		by = 1;
	}
	GraphicalController::rectangle_t rect;
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
	for (int i = -1; i < by + 1; i++)
	{
		for (int j = -1; j < bx + 1; j++)
		{
			if (i == by || j == bx || i == -1 || j == -1)
			{
				int x = px + (Item->x + j) - owner->m_center.x + owner->m_tile_count_x / 2;
				int y = py + (Item->y - i) - owner->m_center.y + owner->m_tile_count_y / 2;
				int yp = owner->m_tile_count_x - x;
				int xp = owner->m_tile_count_y - y;
				rect.set((xp - yp) * tile_size_x_half + owner->m_shift.x, (xp + yp) * tile_size_y_half + owner->m_shift.y, tile_size_x, -tile_size_y);
				glColor4f(1.0F, 0.9F, 0.0F, 0.25F);
				Application::instance().m_graph->draw_sprite(rect);
				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
				glColor4f(1.0, 1.0, 0.0, 1.0);
				Application::instance().m_graph->stroke_cell(xp, yp, owner->m_shift.x, owner->m_shift.y);
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
				int x = px + (Item->x + j) - owner->m_center.x + owner->m_tile_count_x / 2;
				int y = py + (Item->y - i) - owner->m_center.y + owner->m_tile_count_y / 2;
				int yp = owner->m_tile_count_x - x;
				int xp = owner->m_tile_count_y - y;
				rect.set((xp - yp) * tile_size_x_half + owner->m_shift.x, (xp + yp) * tile_size_y_half + owner->m_shift.y, tile_size_x, -tile_size_y);
				glColor4f(1.0F, 0.9F, 0.0F, 0.25F);
				Application::instance().m_graph->draw_sprite(rect);
				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
				glColor4f(1.0, 1.0, 0.0, 1.0);
				Application::instance().m_graph->stroke_cell(xp, yp, owner->m_shift.x, owner->m_shift.y);
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
			}
		}
	}
}

void mapviewer_hint_weapon_range::render(GUI_MapViewer* owner)
{
	int px = 0;
	int py = 0;
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
	MapCell* item = m_object->cell();
	int xc;
	int yc;
	xc = item->x + m_object->m_active_state->m_size.dx / 2;
	yc = item->y - m_object->m_active_state->m_size.dy / 2;
	int ax = 2*m_range + m_object->m_active_state->m_size.dx;
	int ay = 2*m_range + m_object->m_active_state->m_size.dy;
	int xr = ax / 2;
	int yr = ay / 2;
	float xrf = ax / 2.0F - 0.5F;
	float yrf = ay / 2.0F - 0.5F;
	float rf = m_range + m_object->m_active_state->m_size.dx / 2.0F;
	GraphicalController::rectangle_t r;
	for (int i = 0; i < ay; i++)
	{
		for (int j = 0; j < ax; j++)
		{
			
			//if (i*i + j*j <= m_range*m_range)
			int xp = xc + j - xr;
			int yp = yc - i + yr;
			if ((!m_object->is_own(0,xp,yp))&&(rf*rf >= ((j - xrf)*(j - xrf) + (i - yrf)*(i - yrf))))
			{
				int x = px + xp - owner->m_center.x + owner->m_tile_count_x / 2;
				int y = py + yp - owner->m_center.y + owner->m_tile_count_y / 2;
				/*x0 = x * m_owner->m_tile_size_x;
				y0 = (m_owner->m_tile_count_y - y - 1) * m_owner->m_tile_size_y;
				x1 = x0;
				y1 = (m_owner->m_tile_count_y - y) * m_owner->m_tile_size_y;
				x2 = (x + 1) * m_owner->m_tile_size_x;
				y2 = y1;
				x3 = x2;
				y3 = y0;*/
				int yp = owner->m_tile_count_x - x;
				int xp = owner->m_tile_count_y - y;
				r.set((xp - yp) * tile_size_x_half + owner->m_shift.x, (xp + yp) * tile_size_y_half + owner->m_shift.y, tile_size_x, -tile_size_y);
				glColor4f(1.0F, 0.9F, 0.0F, 0.25F);
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
				Application::instance().m_graph->draw_sprite(r);
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
				Application::instance().m_graph->draw_sprite(r);
				glColor4f(1.0F, 0.9F, 0.0F, 0.5F);
				//glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
				Application::instance().m_graph->stroke_cell(xp, yp, owner->m_shift.x, owner->m_shift.y);
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
				Application::instance().m_graph->stroke_cell(xp, yp, owner->m_shift.x,owner->m_shift.y);
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
			}
		}
	}
}

mapviewer_hint_shoot::mapviewer_hint_shoot( GameObject* object, int range) :  m_object(object),m_range(range) { m_top = true; }

void mapviewer_hint_shoot::draw_cell(GUI_MapViewer* owner,MapCell* a)
{
	if (!m_object->is_own(a))
	{
		int px = 0;
		int py = 0;
		int x = px + a->x - owner->m_center.x + owner->m_tile_count_x / 2;
		int y = py + a->y - owner->m_center.y + owner->m_tile_count_y / 2;
		int yp = owner->m_tile_count_x - x;
		int xp = owner->m_tile_count_y - y;
		GraphicalController::rectangle_t r((xp - yp) * tile_size_x_half + owner->m_shift.x, (xp + yp) * tile_size_y_half + owner->m_shift.y, tile_size_x, -tile_size_y);
		bool pass_able = true;
		float xrf = a->x - m_object->cell()->x - m_object->m_active_state->m_size.dx / 2.0F + 0.5F;
		float yrf = a->y - m_object->cell()->y + m_object->m_active_state->m_size.dy / 2.0F - 0.5F;
		float rf = m_range + m_object->m_active_state->m_size.dx / 2.0F;
		glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
		if (rf*rf < (xrf*xrf + yrf*yrf))
		{
			pass_able = false;
		}
		else
		{
			for (auto object = a->m_items.begin(); object != a->m_items.end(); ++object)
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
		Application::instance().m_graph->draw_sprite(r);
		glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
		Application::instance().m_graph->center_text((r.a.x + r.b.x)*0.5, (r.a.y + r.b.y) *0.5, cp1251_to_utf16(std::to_string(m_step_count)), 8, 17);
		if (pass_able)
		{
			glColor4f(0.0F, 1.0F, 0.0F, 1.0F);
		}
		else
		{
			glColor4f(1.0F, 0.0F, 0.0F, 1.0F);
		}
		glDisable(GL_TEXTURE_2D);
		Application::instance().m_graph->stroke_cell(xp, yp, owner->m_shift.x, owner->m_shift.y);
		glEnable(GL_TEXTURE_2D);
		m_step_count += 1;
	}
}

void mapviewer_hint_shoot::render(GUI_MapViewer* owner)
{
	int px = 0;
	int py = 0;
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	if (owner->m_cursored != nullptr)
	{
		m_step_count = 1;
		MapCell* cell = m_object->get_center(owner->m_cursored);
		//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
		owner->m_map->bresenham_line(cell, owner->m_cursored, std::bind(&mapviewer_hint_shoot::draw_cell, this,owner, std::placeholders::_1));
		m_step_count = 1;
	/*	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
		m_owner->m_map->bresenham_line(cell, m_owner->m_cursored, std::bind(&mapviewer_hint_shoot::draw_cell, this, std::placeholders::_1));*/
	}
}

mapviewer_object_move::mapviewer_object_move( GameObject* object) : m_object(object) {}

void mapviewer_object_move::init(GUI_MapViewer* owner)
{
	if (owner->m_cursored)
	{
		m_cell = Game_algorithm::step_in_direction(m_object, Game_algorithm::turn_to_cell(m_object, owner->m_cursored));
	}
	else
	{
		m_cell = nullptr;
	}
}

void mapviewer_object_move::render(GUI_MapViewer* owner)
{
	int px = 0;
	int py = 0;
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
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
				x0 = (xp - yp) * tile_size_x_half + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x1 = x0;
				y1 = (xp + yp) * 9 + m_owner->m_shift.y;
				x2 = (xp - yp) * tile_size_x_half + 32 + m_owner->m_shift.x;
				y2 = y1;
				x3 = x2;
				y3 = y0;
				glColor4f(1.0F, 0.9F, 0.0F, 0.25F);
				Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
				x0 = (xp - yp) * tile_size_x_half + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 9 + m_owner->m_shift.y;
				x1 = (xp - yp) * tile_size_x_half + tile_size_x_half + m_owner->m_shift.x;
				y1 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x2 = (xp - yp) * tile_size_x_half + 32 + m_owner->m_shift.x;
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
				x0 = (xp - yp) * tile_size_x_half + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x1 = x0;
				y1 = (xp + yp) * 9 + m_owner->m_shift.y;
				x2 = (xp - yp) * tile_size_x_half + 32 + m_owner->m_shift.x;
				y2 = y1;
				x3 = x2;
				y3 = y0;
				glColor4f(1.0F, 0.9F, 0.0F, 0.25F);
				Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
				x0 = (xp - yp) * tile_size_x_half + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 9 + m_owner->m_shift.y;
				x1 = (xp - yp) * tile_size_x_half + tile_size_x_half + m_owner->m_shift.x;
				y1 = (xp + yp) * 9 - 18 + m_owner->m_shift.y;
				x2 = (xp - yp) * tile_size_x_half + 32 + m_owner->m_shift.x;
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
		
		int bx;
		int by;
		bx = m_object->m_active_state->m_size.dx;
		by = m_object->m_active_state->m_size.dy;
		bool valid = true;//((abs(m_object->cell()->x - m_owner->m_cursored->x) < 2) && (abs(m_object->cell()->y - m_owner->m_cursored->y) < 2));
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
		GraphicalController::rectangle_t r;
		for (int i = 0; i < by; i++)
		{
			for (int j = 0; j < bx; j++)
			{
				int x = px + (m_cell->x + j) - owner->m_center.x + owner->m_tile_count_x / 2;
				int y = py + (m_cell->y - i) - owner->m_center.y + owner->m_tile_count_y / 2;
				int yp = owner->m_tile_count_x - x;
				int xp = owner->m_tile_count_y - y;
				r.set((xp - yp) * tile_size_x_half + owner->m_shift.x, (xp + yp) * tile_size_y_half + owner->m_shift.y, tile_size_x, -tile_size_y);
				if (valid)
				{
					glColor4f(0.0F, 1.0F, 0.0F, 0.25F);
				}
				else
				{
					glColor4f(1.0F, 0.0F, 0.0F, 0.25F);
				}
				Application::instance().m_graph->draw_sprite(r);
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
				Application::instance().m_graph->stroke_cell(xp, yp, owner->m_shift.x, owner->m_shift.y);
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
			}
		}
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
		for (int i = 0; i < by; i++)
		{
			for (int j = 0; j < bx; j++)
			{
				int x = px + (m_cell->x + j) - owner->m_center.x + owner->m_tile_count_x / 2;
				int y = py + (m_cell->y - i) - owner->m_center.y + owner->m_tile_count_y / 2;
				int yp = owner->m_tile_count_x - x;
				int xp = owner->m_tile_count_y - y;
				r.set((xp - yp) * tile_size_x_half + owner->m_shift.x, (xp + yp) * tile_size_y_half + owner->m_shift.y, tile_size_x, -tile_size_y);
				if (valid)
				{
					glColor4f(0.0F, 1.0F, 0.0F, 0.25F);
				}
				else
				{
					glColor4f(1.0F, 0.0F, 0.0F, 0.25F);
				}
				Application::instance().m_graph->draw_sprite(r);
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
				Application::instance().m_graph->stroke_cell(xp, yp, owner->m_shift.x, owner->m_shift.y);
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
			x0 = (xp - yp) * tile_size_x_half + m_owner->m_shift.x;
			y0 = (xp + yp) * 9 - (54) + m_owner->m_shift.y + (2) * 9;
			x1 = x0;
			y1 = (xp + yp) * 9 + m_owner->m_shift.y + (2) * 9;
			x2 = (xp - yp) * tile_size_x_half + 96 + m_owner->m_shift.x;
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
	//x0 = (xp - yp) * tile_size_x_half + m_owner->m_shift.x;
	//y0 = (xp + yp) * 9 - (object_size.h) + m_owner->m_shift.y + (size3d.x - 1) * 9;
	//x1 = x0;
	//y1 = (xp + yp) * 9 + m_owner->m_shift.y + (size3d.x - 1) * 9;
	//x2 = (xp - yp) * tile_size_x_half + object_size.w + m_owner->m_shift.x;
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

void mapviewer_object_move::render_on_cell(GUI_MapViewer* owner,MapCell* c)
{
	if (m_cell)
	{
		if (m_cell == c)
		{
			glColor4f(0.0F, 1.0F, 0.0F, 0.75F);
			dimension_t object_size;
			dimension3_t size3d;
			object_size = m_object->m_active_state->m_tile_size;
			size3d = m_object->m_active_state->m_size;
			int x = c->x - owner->m_center.x + owner->m_tile_count_x / 2;
			int y = c->y - owner->m_center.y + owner->m_tile_count_y / 2;
			int yp = owner->m_tile_count_x - x;
			int xp = owner->m_tile_count_y - y;
			GraphicalController::rectangle_t r((xp - yp) * tile_size_x_half + owner->m_shift.x, (xp + yp+ size3d.dx - 1) * tile_size_y_half + owner->m_shift.y, object_size.w, -object_size.h);
			tile_t tile;
			m_object->m_active_state->m_tile_manager->set_tile(tile, m_object, Application::instance().m_timer->get_tick(), Game_algorithm::turn_to_cell(m_object, owner->m_cursored));
			GLuint Sprite = tile.unit;
			glUseProgramObjectARB(0);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Sprite);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
			Application::instance().m_graph->draw_tile(tile, r);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
			Application::instance().m_graph->draw_tile(tile, r);
		}
	}
}

mapviewer_object_rotate::mapviewer_object_rotate(GameObject* object) :  m_object(object)
{ 
	m_fov = new FOV_help();
}

void mapviewer_object_rotate::init(GUI_MapViewer* owner)
{
	if (owner->m_cursored)
	{
		m_direction =  Game_algorithm::turn_to_cell(m_object, owner->m_cursored);
	}
	else
	{
		m_direction = m_object->m_direction;
	}
	m_fov->calculate_FOV(m_object, m_object->cell()->m_map, m_direction);
}

void mapviewer_object_rotate::render(GUI_MapViewer* owner)
{
	if (owner->m_cursored)
	{
		bool top_bound;
		bool down_bound;
		bool left_bound;
		bool right_bound;
		double xt, yt;
		int yp;
		int xp;
		MapCell* c;
		glUseProgram(0);
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
					c = &(owner->m_map->get(m_object->cell()->z,y,x));
					yp = owner->m_tile_count_x / 2 - c->x + owner->m_center.x;
					xp = owner->m_tile_count_y / 2 - c->y + owner->m_center.y;
					xt = (xp - yp) * tile_size_x_half + owner->m_shift.x;
					yt = (xp + yp) * tile_size_y_half + owner->m_shift.y - m_object->cell()->z * 22;
					GraphicalController::rectangle_t rect(xt, yt, tile_size_x, -tile_size_y);
					Application::instance().m_graph->draw_sprite(rect);
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
						glVertex2d(xt, yt - tile_size_y_half);
						glVertex2d(xt + tile_size_x_half, yt - tile_size_y);
					}
					if (right_bound)
					{
						glVertex2d(xt + tile_size_x_half, yt - tile_size_y);
						glVertex2d(xt + tile_size_x, yt-tile_size_y_half);
					}
					if (down_bound)
					{
						glVertex2d(xt + tile_size_x, yt-tile_size_y_half);
						glVertex2d(xt + tile_size_x_half, yt);
					}
					if (left_bound)
					{
						glVertex2d(xt + tile_size_x_half, yt);
						glVertex2d(xt, yt - tile_size_y_half);
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

void mapviewer_object_rotate::render_on_cell(GUI_MapViewer* owner,MapCell* c)
{
	if (owner->m_cursored)
	{
		if (c == m_object->cell())
		{
			glColor4f(0.0F, 1.0F, 0.0F, 0.75F);
			dimension_t object_size;
			dimension3_t size3d;
			object_size = m_object->m_active_state->m_tile_size;
			size3d = m_object->m_active_state->m_size;
			int x = c->x - owner->m_center.x + owner->m_tile_count_x / 2;
			int y = c->y - owner->m_center.y + owner->m_tile_count_y / 2;
			int yp = owner->m_tile_count_x - x;
			int xp = owner->m_tile_count_y - y;
			GraphicalController::rectangle_t r((xp - yp) * tile_size_x_half + owner->m_shift.x, (xp + yp + size3d.dx - 1) * tile_size_y_half + owner->m_shift.y, object_size.w, -object_size.h);
			tile_t tile;
			m_object->m_active_state->m_tile_manager->set_tile(tile, m_object, Application::instance().m_timer->get_tick(), m_direction);
			GLuint Sprite = tile.unit;
			Logger::instance().info(std::to_string(tile.coordinates[0]) + " " + std::to_string(tile.coordinates[1]) + "  " + std::to_string(tile.coordinates[2]) + "  " + std::to_string(tile.coordinates[3]));
			glUseProgramObjectARB(0);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Sprite);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
			Application::instance().m_graph->draw_tile(tile, r);
			//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_03, 0);
			//Application::instance().m_graph->draw_tile(tile, r);
		}
	}
}

GUI_MapViewer::GUI_MapViewer(Application* app)
{
	m_z_level = 0;
	m_max_count = 0;
	m_tile_count_x = 90;
	m_tile_count_y = 90;
	m_rotate = 0;
	m_just_focused = false;
	m_focus = nullptr;
	m_cursored = nullptr;
	m_cursor_x = 1;
	m_cursor_y = 1;
	m_size = app->m_size;
	m_shift.x = float(m_size.w) / 2 - tile_size_x_half;// (m_tile_count_x)* 32 * 0.5;
	m_shift.y = float(m_size.h) / 2 - m_tile_count_y* tile_size_y_half - tile_size_y_half;// 1024 * 0.5 - (m_tile_count_y + 6) * 18 * 0.5;

	m_vao = 0;
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vertex_buffer);
	//glGenBuffers(1, &m_textcoor_buffer);
	//glGenBuffers(1, &m_light_buffer);

	m_quads.resize(18000);

	/*m_shift.x = 0;
	m_shift.y = 0;*/

}

GUI_MapViewer::~GUI_MapViewer(void)
{
}

void GUI_MapViewer::calculate()
{
	m_tile_size_x = tile_size_x_half;
	m_tile_size_y = tile_size_y_half;
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

struct rgb
{
	GLfloat value[3];
};

void GUI_MapViewer::render(GraphicalController* graph, int px, int py)
{
	graph->check_gl_error("test");
	float light[4];
	glBindFramebuffer(GL_FRAMEBUFFER, graph->m_FBO);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glUseProgram(0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, graph->m_empty_01, 0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, graph->m_empty_02, 0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, graph->m_empty_03, 0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	bool is_draw;
	tile_t tile{};
	int x;
	int y;
	int xf;
	int yf;
	int xx;
	int yy;

	int dx;
	glColor4d(1.0, 1.0, 1.0, 1.0);
	dimension_t object_size;
	dimension3_t size3d;

	int j_limit;
	int gx;
	int gy;
	bool passable;
	bool is_hide;
	bool is_in_fov;

	Application::instance().m_UI_mutex.lock();
	for (auto& m_hint : Application::instance().m_gui_controller.m_hints)
	{
		m_hint->init(this);
	}
	Application::instance().m_UI_mutex.unlock();

	bool m_observer;
	auto ai = dynamic_cast<AI_enemy*>(m_player->m_object->m_active_state->m_ai);
	auto vl = dynamic_cast<Vision_list*>(m_player->m_object->m_active_state->get_list(interaction_e::vision));
	m_observer = !vl;
	int max_fov_radius;
	view_t fov;
	position_t posc;
	if (!m_observer)
	{
		max_fov_radius = vl->m_max_radius;
		fov = ai->m_fov->m_view;
		posc = position_t((m_player->m_object->m_active_state->m_size.dx - 1) >> 1, (m_player->m_object->m_active_state->m_size.dy - 1) >> 1);
	}
	GraphicalController::rectangle_t rect;

	auto count = 0;
	for (int i = 0; i < (m_tile_count_x + m_tile_count_y) - 1; i++)
	{
		if (i < m_tile_count_y)
		{
			j_limit = i + 1;
			gx = m_tile_count_x;
			gy = m_tile_count_y - i - 1;
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

			/*float sin_a = -1;
			float cos_a = 0;
			x = m_center.x + (gx - m_tile_count_x / 2) * cos_a - (gy - m_tile_count_y / 2) *sin_a;
			y = m_center.y + (gy - m_tile_count_y / 2) * cos_a + (gx - m_tile_count_x / 2) *sin_a;*/

			switch (m_rotate)
			{
			case 0:
			{
				x = m_center.x + (gx - m_tile_count_x / 2);
				y = m_center.y + (gy - m_tile_count_y / 2);
				break;
			}
			case 1:
			{
				x = m_center.x - (gy - m_tile_count_y / 2);
				y = m_center.y + (gx - m_tile_count_x / 2);
				break;
			}
			case 2:
			{
				x = m_center.x - (gx - m_tile_count_x / 2);
				y = m_center.y - (gy - m_tile_count_y / 2);
				break;
			}
			case 3:
			{
				x = m_center.x + (gy - m_tile_count_y / 2);
				y = m_center.y - (gx - m_tile_count_x / 2);
				break;
			}
			}

			xf = x - m_player->m_object->cell()->x - posc.x;
			yf = y - m_player->m_object->cell()->y + posc.y;
			if ((x > -1) && (x < m_map->m_size.dx) && (y > -1) && (y < m_map->m_size.dy))
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

				for (int z = 0; z < m_map->m_size.dz; ++z)
				{
					auto& cell = m_map->get(z, y, x);
					for (auto current = cell.m_items.begin(); current != cell.m_items.end(); ++current)
					{
						light[0] = cell.m_light.r * 0.01F;
						light[1] = cell.m_light.g * 0.01F;
						light[2] = cell.m_light.b * 0.01F;
						light[3] = 0.0;
						/*	light[0] = 1.0;
							light[1] = 1.0;
							light[2] = 1.0;
							light[3] = 0.0;*/
						is_hide = false;
						is_draw = false;
						passable = (*current)->get_stat(object_tag_e::pass_able);
						/*if ((r == 0 && passable) || (r == 1 && !passable))
						{*/
						switch (m_rotate)
						{
						case 0:
						{
							xx = x;
							yy = y;
							break;
						}
						case 1:
						{
							xx = x;
							yy = y + (*current)->m_active_state->m_size.dy - 1;
							break;
						}
						case 2:
						{
							xx = x - (*current)->m_active_state->m_size.dx + 1;
							yy = y + (*current)->m_active_state->m_size.dy - 1;
							break;
						}
						case 3:
						{
							xx = x - (*current)->m_active_state->m_size.dx + 1;
							yy = y;
							break;
						}
						}
						//if ((*Current)->cell()->z == z && (*Current)->cell()->y == yy && (*Current)->cell()->x  == xx)
						{
							size3d = (*current)->m_active_state->m_size;


							is_draw = true;
							/*	if (m_observer)
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
									if (size3d.dx > 1 || size3d.dy > 1)
									{
										for (int m = 0; m < size3d.dy; m++)
										{
											for (int n = 0; n < size3d.dx; n++)
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
									else if (m_map->get(0, y, x).m_notable)
									{
										if (((*Current)->m_name == u"floor") || ((*Current)->m_name == u"wall"))
										{
											IsDraw = true;
											is_hide = true;
										}
									}
								}*/
						}
						/*}*/
						if (is_draw)
						{
							object_size = (*current)->m_active_state->m_tile_size;
							auto yp = m_tile_count_x - px - gx;
							auto xp = m_tile_count_y - py - gy;
							rect.set((xp - yp) * tile_size_x_half + m_shift.x, (xp + yp) * tile_size_y_half + m_shift.y - z * tile_size_y, object_size.w, -object_size.h);
							// считаем среднюю освещенность для многотайловых объектов
							//if (size3d.dx > 1 || size3d.dy > 1)
							//{
							//	light[0] = 0;
							//	light[1] = 0;
							//	light[2] = 0;
							//	for (int m = 0; m < size3d.dy; m++)
							//	{
							//		for (int n = 0; n < size3d.dx; n++)
							//		{
							//			light[0] += m_map->get(0, y - m, x + n).m_light.R;
							//			light[1] += m_map->get(0, y - m, x + n).m_light.G;
							//			light[2] += m_map->get(0, y - m, x + n).m_light.B;
							//		}
							//	}
							//	light[0] = light[0] / (size3d.dx*size3d.dy*100.0F);
							//	light[1] = light[1] / (size3d.dx*size3d.dy*100.0F);
							//	light[2] = light[2] / (size3d.dx*size3d.dy*100.0F);
							//	/*		light[0] = 1.0;
							//			light[1] = 1.0;
							//			light[2] = 1.0;
							//			light[3] = 0.0;*/
							//}

							/*if (is_hide)
							{
								glUseProgram(Graph->m_tile_shader_hide);
							}
							else
							{
								glUseProgram(Graph->m_tile_shader);
							}*/
							int pos;
							switch (m_rotate)
							{
							case 0:
							{
								pos = abs((*current)->cell()->z - z) * (size3d.dx * size3d.dy) + abs((*current)->cell()->y - yy) * size3d.dx + abs((*current)->cell()->x - xx);
								break;
							}
							case 1:
							{
								pos = abs((*current)->cell()->z - z) * (size3d.dx * size3d.dy) + abs((*current)->cell()->x - xx) * size3d.dx + abs((*current)->cell()->y - yy);
								break;
							}
							case 2:
							{
								pos = abs((*current)->cell()->z - z) * (size3d.dx * size3d.dy) + abs((*current)->cell()->y - yy) * size3d.dx + abs((*current)->cell()->x - xx);
								break;
							}
							case 3:
							{
								pos = abs((*current)->cell()->z - z) * (size3d.dx * size3d.dy) + abs((*current)->cell()->x - xx) * size3d.dx + abs((*current)->cell()->y - yy);
								break;
							}
							}

							/*if ((*Current)->m_name == u"darkeye")
							{
								LOG(INFO) << std::to_string(pos);
							}*/
							(*current)->m_active_state->m_tile_manager->set_tile(m_quads[count], (*current), pos, Game_algorithm::get_direction((*current)->m_direction, m_rotate));
							//GLuint Sprite = tile.unit;
							glEnable(GL_BLEND);
							glEnable(GL_TEXTURE_2D);
							glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							/*		glActiveTexture(GL_TEXTURE0);
									glBindTexture(GL_TEXTURE_2D, Sprite);*/
									//Graph->set_uniform_sampler(Graph->m_tile_shader, "Map", 0);
									//Graph->set_uniform_vector(Graph->m_tile_shader, "light", light);
									/*glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
									glUseProgramObjectARB(0);*/

							auto& vb = m_quads[count];

							vb.vertex[0].position[0] = rect.a.x;
							vb.vertex[0].position[1] = rect.b.y;
							vb.vertex[1].position[0] = rect.a.x;
							vb.vertex[1].position[1] = rect.a.y;
							vb.vertex[2].position[0] = rect.b.x;
							vb.vertex[2].position[1] = rect.a.y;
							vb.vertex[3].position[0] = rect.b.x;
							vb.vertex[3].position[1] = rect.b.y;

							//auto& lb = m_light[count];
							vb.vertex[0].light[0] = light[0];
							vb.vertex[0].light[1] = light[1];
							vb.vertex[0].light[2] = light[2];

							vb.vertex[1].light[0] = light[0];
							vb.vertex[1].light[1] = light[1];
							vb.vertex[1].light[2] = light[2];

							vb.vertex[2].light[0] = light[0];
							vb.vertex[2].light[1] = light[1];
							vb.vertex[2].light[2] = light[2];

							vb.vertex[3].light[0] = light[0];
							vb.vertex[3].light[1] = light[1];
							vb.vertex[3].light[2] = light[2];




							/*	auto& tb = m_textcoords[count];
								tb.value[0] = 0.0;
								tb.value[1] = 1.0;
								tb.value[2] = 0.0;
								tb.value[3] = 0.0;
								tb.value[4] = 1.0;
								tb.value[5] = 0.0;
								tb.value[6] = 1.0;
								tb.value[7] = 1.0;*/


								//LOG(INFO) << std::to_string(m_textcoords[count].value[1]);

							count += 1;



							/*tb[0].value[0] = 1.0;
							tb[0].value[1] = 0;
							tb[0].value[2] = 0;
							tb[1].value[0] = 1.0;
							tb[1].value[1] = 0;
							tb[1].value[2] = 0;
							tb[2].value[0] = 1.0;
							tb[2].value[1] = 0;
							tb[2].value[2] = 0;
							tb[3].value[0] = 0;
							tb[3].value[1] = 0;
							tb[3].value[2] = 0;*/


							//glBindBuffer(GL_ARRAY_BUFFER, m_textcoor_buffer);
							//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, tb, GL_DYNAMIC_DRAW);
							//glColorPointer(3, GL_FLOAT, 0, nullptr);




							//Graph->draw_tile(tile, rect);

							if ((*current)->cell()->x == m_map->get(0, y, x).x)
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

							//if ((*Current)->m_active_state->m_layer == 1)
							//{
							//	/*if (is_hide)
							//	{
							//		glUseProgram(Graph->m_tile_shader_hide);
							//	}
							//	else
							//	{*/
							//	glUseProgram(Graph->m_tile_shader);
							//	/*}*/
							//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_02, 0);
							//	Graph->draw_tile(tile, rect);

							//	/*glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
							//	glUseProgram(Graph->m_mask_shader2);
							//	glActiveTexture(GL_TEXTURE0);
							//	glBindTexture(GL_TEXTURE_2D, Graph->m_empty_03);
							//	Graph->set_uniform_sampler(Graph->m_mask_shader2, "Map", 0);
							//	glActiveTexture(GL_TEXTURE1);
							//	glBindTexture(GL_TEXTURE_2D, Sprite);
							//	Graph->set_uniform_sampler(Graph->m_mask_shader2, "Unit", 1);
							//	glActiveTexture(GL_TEXTURE0);
							//	Graph->draw_tile_FBO(x1 / 1024.0, (1024 - y1) / 1024.0, x3 / 1024.0, (1024 - y3) / 1024.0, x0, y0, x1, y1, x2, y2, x3, y3);*/
							//}

							//if ((*Current)->m_active_state->m_layer == 2)
							//{
							//	/*glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
							//	glUseProgram(Graph->m_mask_shader2);
							//	glActiveTexture(GL_TEXTURE0);
							//	glBindTexture(GL_TEXTURE_2D, Graph->m_empty_03);
							//	Graph->set_uniform_sampler(Graph->m_mask_shader2, "Map1", 0);
							//	glActiveTexture(GL_TEXTURE1);
							//	glBindTexture(GL_TEXTURE_2D, Sprite);
							//	Graph->set_uniform_sampler(Graph->m_mask_shader2, "Unit", 1);
							//	glActiveTexture(GL_TEXTURE2);
							//	glBindTexture(GL_TEXTURE_2D, Graph->m_empty_02);
							//	Graph->set_uniform_sampler(Graph->m_mask_shader2, "Map2", 2);
							//	glActiveTexture(GL_TEXTURE0);
							//	Graph->draw_tile_FBO(x1 /(float)m_size.w, (m_size.h - y1) / (float)m_size.h, x3 / (float)m_size.w, (m_size.h - y3) /(float) m_size.h, x0, y0, x1, y1, x2, y2, x3, y3);*/

							//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
							//	glUseProgram(Graph->m_mask_shader);
							//	glActiveTexture(GL_TEXTURE0);
							//	glBindTexture(GL_TEXTURE_2D, Graph->m_empty_02);
							//	Graph->set_uniform_sampler(Graph->m_mask_shader, "Map", 0);
							//	glActiveTexture(GL_TEXTURE1);
							//	glBindTexture(GL_TEXTURE_2D, Sprite);
							//	Graph->set_uniform_sampler(Graph->m_mask_shader, "Unit", 1);
							//	glActiveTexture(GL_TEXTURE0);
							//	Graph->draw_tile_FBO(rect.a.x / static_cast<float>(m_size.w), (m_size.h - rect.a.y) / static_cast<float>(m_size.h), rect.b.x / static_cast<float>(m_size.w), (m_size.h - rect.b.y) / static_cast<float>(m_size.h), rect);
							//}

						}
					}
					if (m_z_level== z) {
						if (m_cursored != nullptr && m_cursored->x == cell.x && m_cursored->y == cell.y) {
							auto item = m_cursored;
						/*	int x;
							int y;

							switch (m_rotate) {
							case 0: {
								x = px + item->x - m_center.x + m_tile_count_x / 2;
								y = py + item->y - m_center.y + m_tile_count_y / 2;
								break;
							}
							case 1: {
								x = -m_center.y + item->y + m_tile_count_x / 2;
								y = m_center.x - item->x + m_tile_count_y / 2;
								break;
							}
							case 2: {
								x = m_center.x - item->x + m_tile_count_x / 2;
								y = m_center.y - item->y + m_tile_count_y / 2;
								break;
							}
							case 3: {
								x = m_center.y - item->y + m_tile_count_x / 2;
								y = -m_center.x + item->x + m_tile_count_y / 2;
								break;
							}
							}*/
							
							auto yp = m_tile_count_x - px - gx;
							auto xp = m_tile_count_y - py - gy;
							rect.set((xp - yp)* tile_size_x_half + m_shift.x, (xp + yp)* tile_size_y_half + m_shift.y - z * tile_size_y, tile_size_x, -2 * tile_size_y);
							auto& vb = m_quads[count];

							vb.vertex[0].position[0] = rect.a.x;
							vb.vertex[0].position[1] = rect.b.y;
							vb.vertex[1].position[0] = rect.a.x;
							vb.vertex[1].position[1] = rect.a.y;
							vb.vertex[2].position[0] = rect.b.x;
							vb.vertex[2].position[1] = rect.a.y;
							vb.vertex[3].position[0] = rect.b.x;
							vb.vertex[3].position[1] = rect.b.y;

						
							vb.vertex[0].light[0] = 1;
							vb.vertex[0].light[1] = 1;
							vb.vertex[0].light[2] = 1;

							vb.vertex[1].light[0] = 1;
							vb.vertex[1].light[1] = 1;
							vb.vertex[1].light[2] = 1;

							vb.vertex[2].light[0] = 1;
							vb.vertex[2].light[1] = 1;
							vb.vertex[2].light[2] = 1;

							vb.vertex[3].light[0] = 1;
							vb.vertex[3].light[1] = 1;
							vb.vertex[3].light[2] = 1;

							const auto x1 = 0 / 950.0;
							const auto y1 = 220 / 352.0;
							const auto x2 = (0 + 38) / 950.0;
							const auto y2 = (220 + 44) / 352.0;
							vb.vertex[0].texture_coordinates[0] = x1;
							vb.vertex[0].texture_coordinates[1] = y2;
							vb.vertex[0].texture_coordinates[2] = static_cast<double>(1);

							vb.vertex[1].texture_coordinates[0] = x1;
							vb.vertex[1].texture_coordinates[1] = y1;
							vb.vertex[1].texture_coordinates[2] = static_cast<double>(1);

							vb.vertex[2].texture_coordinates[0] = x2;
							vb.vertex[2].texture_coordinates[1] = y1;
							vb.vertex[2].texture_coordinates[2] = static_cast<double>(1);

							vb.vertex[3].texture_coordinates[0] = x2;
							vb.vertex[3].texture_coordinates[1] = y2;
							vb.vertex[3].texture_coordinates[2] = static_cast<double>(1);

							count += 1;
						}
					}
				}
				//Application::instance().m_UI_mutex.lock();
				//for (auto& m_hint : Application::instance().m_gui_controller.m_hints)
				//{
				//	m_hint->render_on_cell(this, &m_map->get(0, y, x));
				//}
				//Application::instance().m_UI_mutex.unlock();
			}
		}
	}

	if (count > m_max_count)
	{
		m_max_count = count;
		Logger::instance().info("m_max_count : " + std::to_string(m_max_count));
	}
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	glUseProgram(graph->m_atlas_shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, graph->m_atlas);
	graph->set_uniform_sampler(graph->m_atlas_shader, "atlas", 0);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_t) * count, &m_quads[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(vertex_t), position_offset);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, sizeof(vertex_t), texture_coordinates_offset);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), light_offset);
	glEnableVertexAttribArray(2);
	glDrawArrays(GL_QUADS, 0, 4 * count);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
	/*Application::instance().m_UI_mutex.lock();
	if (r == 0)
	{
		for (auto current = Application::instance().m_gui_controller.m_hints.begin(); current != Application::instance().m_gui_controller.m_hints.end(); ++current)
		{
			if (!(*current)->m_top)
			{
				(*current)->render(this);
			}
		}
	}
	else
	{
		for (auto current = Application::instance().m_gui_controller.m_hints.begin(); current != Application::instance().m_gui_controller.m_hints.end(); ++current)
		{
			if ((*current)->m_top)
			{
				(*current)->render(this);
			}
		}
	}
	Application::instance().m_UI_mutex.unlock();*/

	Application::instance().m_UI_mutex.lock();


	for (auto& m_hint : Application::instance().m_gui_controller.m_hints)
	{
		//if (m_hint->m_top)
		{
			m_hint->render(this);
		}
	}

Application::instance().m_UI_mutex.unlock();

	if (m_cursored != nullptr){
		auto item = m_cursored;

		/*int x = px + Item->x - m_center.x + m_tile_count_x / 2;
		int y = py + Item->y - m_center.y + m_tile_count_y / 2;*/

		int x;
		int y;

		switch (m_rotate) {
		case 0: {
			x = px + item->x - m_center.x + m_tile_count_x / 2;
			y = py + item->y - m_center.y + m_tile_count_y / 2;
			break;
		}
		case 1: {
			x = -m_center.y + item->y + m_tile_count_x / 2;
			y = m_center.x - item->x + m_tile_count_y / 2;
			break;
		}
		case 2: {
			x = m_center.x - item->x + m_tile_count_x / 2;
			y = m_center.y - item->y + m_tile_count_y / 2;
			break;
		}
		case 3: {
			x = m_center.y - item->y + m_tile_count_x / 2;
			y = -m_center.x + item->x + m_tile_count_y / 2;
			break;
		}
		}


		//x0 = (x - m_cursor_x + 1) * m_tile_size_x;
		//y0 = (m_tile_count_y - y - m_cursor_y) * m_tile_size_y;
		//x1 = (x - m_cursor_x + 1) * m_tile_size_x;
		//y1 = (m_tile_count_y - y) * m_tile_size_y;
		//x2 = (x + 1) * m_tile_size_x;
		//y2 = (m_tile_count_y - y) * m_tile_size_y;
		//x3 = (x + 1) * m_tile_size_x;
		//y3 = (m_tile_count_y - y - m_cursor_y) * m_tile_size_y;
		auto yp = m_tile_count_x - x;
		auto xp = m_tile_count_y - y;
		rect.set((xp - yp)* tile_size_x_half + m_shift.x, (xp + yp - m_z_level * 2)* tile_size_y_half + m_shift.y, tile_size_x, -tile_size_y);
		glUseProgram(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, graph->m_empty_01, 0);
		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor4f(0.0, 1.0, 0.0, 0.75);
		Application::instance().m_graph->selection_cell(xp, yp, m_z_level, int(m_shift.x), int(m_shift.y));
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0, 1.0, 1.0, 1.0);
	}

	//glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	//glUseProgram(0);
	//glDisable(GL_BLEND);
	//glBindTexture(GL_TEXTURE_2D, Graph->m_empty_01);
	//glColor4d(1.0, 1.0, 1.0, 1.0);
	////glGenerateMipmap(GL_TEXTURE_2D);
	////Graph->draw_sprite(0, 0, 0, 1024/4, 1024/4, 1024/4, 1024/4, 0);
	////Graph->blur_rect(0, 0, 1024 / 4, 1024 / 4);
	////glBindTexture(GL_TEXTURE_2D, Graph->m_blur);
	////glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 1024 - 1024 / 4, 1024 /4, 1024 /4);
	//rect.set(0, 0, m_size.w, m_size.h);
	//Application::instance().m_graph->draw_sprite(rect);
	//glDisable(GL_TEXTURE_2D);
}

void GUI_MapViewer::on_key_press(WPARAM w)
{
	//Parameter_Position* P;
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
			Parameter* p = new Parameter(parameter_type_e::accept);
			Application::instance().m_message_queue.push(p);
		}
		return;
	}
	case VK_SPACE:
	{
		if (Application::instance().m_message_queue.m_reader)
		{
			Parameter* p = new Parameter(parameter_type_e::cancel);
			Application::instance().m_message_queue.push(p);
		}
		return;
	}
	}
}

void GUI_MapViewer::on_mouse_click(MouseEventArgs const& e) {
	if (e.key == mk_left) {
		const auto p = local_xy(position_t(e.position.x, e.position.y));
		auto x = m_center.x + p.x - m_tile_count_x / 2;
		auto y = m_center.y + p.y - m_tile_count_y / 2;
		switch (m_rotate) {
		case 0:
		{
			x = m_center.x + (p.x - m_tile_count_x / 2);
			y = m_center.y + (p.y - m_tile_count_y / 2);
			break;
		}
		case 1:
		{
			x = m_center.x - (p.y - m_tile_count_y / 2);
			y = m_center.y + (p.x - m_tile_count_x / 2);
			break;
		}
		case 2:
		{
			x = m_center.x - (p.x - m_tile_count_x / 2);
			y = m_center.y - (p.y - m_tile_count_y / 2);
			break;
		}
		case 3:
		{
			x = m_center.x + (p.y - m_tile_count_y / 2);
			y = m_center.y - (p.x - m_tile_count_x / 2);
			break;
		}
		}

		if (!m_just_focused) {
			if (!(x < 0 || x > m_map->m_size.dx - 1 || y < 0 || y > m_map->m_size.dy - 1)) {
				const auto parameter = new Parameter(parameter_type_e::owner);
				(*parameter)[0].set(&m_map->get(m_z_level, y, x));
				if (Application::instance().m_message_queue.m_reader)
				{
					Application::instance().m_message_queue.push(parameter);
				}
			}
		}
		m_just_focused = false;
	}
	else {
		auto pop_menu = new Select_object_popmenu();
		pop_menu->m_position.x = e.position.x;
		pop_menu->m_position.y = e.position.y;
		const auto p = local_xy(position_t(e.position.x, e.position.y));

		const auto x = m_center.x + p.x - m_tile_count_x / 2;
		const auto y = m_center.y + p.y - m_tile_count_y / 2;
		if (!((x < 0) || (x > m_map->m_size.dx - 1) || (y < 0) || (y > m_map->m_size.dy - 1)))
		{
			//UnderCursor(CursorEventArgs(e.x, e.y));
			for (auto& m_item : m_map->get(m_z_level, y, x).m_items)
			{
				pop_menu->add_item(m_item->m_name, m_item);
			}
		}
		if (pop_menu->m_items.empty())
		{
			pop_menu->add_item(u"Нет действий", nullptr);
		}
		pop_menu->destroy += std::bind(&GUI_Layer::remove, m_gui, std::placeholders::_1);
		m_gui->add_front(pop_menu);
		pop_menu->m_items.front()->set_focus(true);
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

		switch (m_rotate)
		{
		case 0:
		{
			x = m_center.x + (p.x - m_tile_count_x / 2);
			y = m_center.y + (p.y - m_tile_count_y / 2);
			break;
		}
		case 1:
		{
			x = m_center.x - (p.y - m_tile_count_y / 2);
			y = m_center.y + (p.x - m_tile_count_x / 2);
			break;
		}
		case 2:
		{
			x = m_center.x - (p.x - m_tile_count_x / 2);
			y = m_center.y - (p.y - m_tile_count_y / 2);
			break;
		}
		case 3:
		{
			x = m_center.x + (p.y - m_tile_count_y / 2);
			y = m_center.y - (p.x - m_tile_count_x / 2);
			break;
		}
		}


		//if (!((x<0) || (x>m_map->m_size.w - 1) || (y<0) || (y>m_map->m_size.h - 1)))
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
		if (m_tile_count_x < tile_size_x_half) m_tile_count_x = tile_size_x_half;
		if (m_tile_count_y < 32) m_tile_count_y = 32;
	}
	calculate();
	position_t p = local_xy(position_t(e.position.x, e.position.y));
	const int x = m_center.x + p.x - m_tile_count_x / 2;
	const int y = m_center.y + p.y - m_tile_count_y / 2;
	if (!(x < 0 || x > m_map->m_size.dx - 1 || y < 0 || y > m_map->m_size.dy - 1))
	{
		m_cursored = &m_map->get(0,y, x);
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

position_t GUI_MapViewer::local_xy(const position_t p)
{
	const auto x = (p.x - m_shift.x - tile_size_x_half) / tile_size_x;
	const auto y = (p.y - m_shift.y) / tile_size_y;
	auto result = position_t(m_tile_count_x - (m_tile_count_y - x - m_tile_count_x + y), m_tile_count_y - (x + y));
	
	if (result.x > m_tile_count_x - 1) { result.x = m_tile_count_x - 1; }
	if (result.x < 0) { result.x = 0; }
	if (result.y > m_tile_count_y - 1) { result.y = m_tile_count_y - 1; }
	if (result.y < 0) { result.y = 0; }
	return result;
}

void GUI_MapViewer::on_mouse_move(MouseEventArgs const& e)
{
	const auto p = local_xy(e.position);
	/*int x = m_center.x + p.x - m_tile_count_x / 2;
	int y = m_center.y + p.y - m_tile_count_y / 2;*/

	int x;
	int y;

	switch (m_rotate)
	{
	case 0:
	{
		x = m_center.x + (p.x - m_tile_count_x / 2);
		y = m_center.y + (p.y - m_tile_count_y / 2);
		break;
	}
	case 1:
	{
		x = m_center.x - (p.y - m_tile_count_y / 2);
		y = m_center.y + (p.x - m_tile_count_x / 2);
		break;
	}
	case 2:
	{
		x = m_center.x - (p.x - m_tile_count_x / 2);
		y = m_center.y - (p.y - m_tile_count_y / 2);
		break;
	}
	case 3:
	{
		x = m_center.x + (p.y - m_tile_count_y / 2);
		y = m_center.y - (p.x - m_tile_count_x / 2);
		break;
	}
	}

	if (!(x < 0 || x > m_map->m_size.dx - 1 || y < 0 || y > m_map->m_size.dy - 1))
	{
		m_cursored = &m_map->get(m_z_level, y, x);
	}

	const auto local_mouse_event_args = set_local_mouse_control(e);
	if (m_focus)
	{
		m_focus->mouse_move(local_mouse_event_args);
	}
}

void GUI_MapViewer::on_mouse_start_drag(MouseEventArgs const& e)
{
	const auto p = local_xy(position_t(e.position.x, e.position.y));
	switch (m_rotate)
	{
	case 0:
	{
		m_initial_position.x = m_center.x - p.x;
		m_initial_position.y = m_center.y - p.y;
		break;
	}
	case 1:
	{
		m_initial_position.x = m_center.x + p.y;
		m_initial_position.y = m_center.y - p.x;
		break;
	}
	case 2:
	{
		m_initial_position.x = m_center.x + p.x;
		m_initial_position.y = m_center.y + p.y;
		break;
	}
	case 3:
	{
		m_initial_position.x = m_center.x - p.y;
		m_initial_position.y = m_center.y + p.x;
		break;
	}
	}
}

void GUI_MapViewer::on_mouse_drag(MouseEventArgs const& e)
{
	const auto p = local_xy(position_t(e.position.x, e.position.y));
	switch (m_rotate)
	{
	case 0:
	{
		m_center.x = m_initial_position.x + p.x;
		m_center.y = m_initial_position.y + p.y;
		break;
	}
	case 1:
	{
		m_center.x = m_initial_position.x - p.y;
		m_center.y = m_initial_position.y + p.x;
		break;
	}
	case 2:
	{
		m_center.x = m_initial_position.x - p.x;
		m_center.y = m_initial_position.y - p.y;
		break;
	}
	case 3:
	{
		m_center.x = m_initial_position.x + p.y;
		m_center.y = m_initial_position.y - p.x;
		break;
	}
	}
}

void GUI_MapViewer::on_mouse_end_drag(MouseEventArgs const& e)
{
}

MouseEventArgs GUI_MapViewer::set_local_mouse_control(MouseEventArgs const& source)
{
	return MouseEventArgs(position_t(source.position.x - m_position.x, source.position.y - m_position.y), source.key, source.value);
}