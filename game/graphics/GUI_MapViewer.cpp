#include "game/graphics/GUI_MapViewer.h"
#include "game/graphics/GUI_Layer.h"
#include "game/graphics/GUI_PopMenu.h"
#include "game/ActionManager.h"
#include "log.h"


gui_mapviewer_hint::gui_mapviewer_hint(GUI_MapViewer* owner) :m_owner(owner){}
mapviewer_hint_area::mapviewer_hint_area(GUI_MapViewer* owner, GameObject* object, bool consider_object_size) : gui_mapviewer_hint(owner), m_object(object),m_consider_object_size(consider_object_size){}
mapviewer_hint_object_area::mapviewer_hint_object_area(GUI_MapViewer* owner, GameObject* object) : gui_mapviewer_hint(owner), m_object(object){}
mapviewer_hint_line::mapviewer_hint_line(GUI_MapViewer* owner, MapCell* cell) : gui_mapviewer_hint(owner), m_cell(cell){}

void mapviewer_hint_area::render()
{
	int px = 0;
	int py = 0;
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
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
	for (int i = -by; i < 2; i++)
	{
		for (int j = -bx; j <2; j++)
		{
			if (i == -by || j == -bx || i == 1 || j == 1)
			{
				int x = px + (Item->x + j) - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
				int y = py + (Item->y + i) - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
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
				Application::instance().m_graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
				x0 = (xp - yp) * 16 + m_owner->m_shift.x;
				y0 = (xp + yp) * 9 - 9 + m_owner->m_shift.y;
				x1 = (xp - yp) * 16 + 16 + m_owner->m_shift.x;
				y1 = (xp + yp) * 9-18 + m_owner->m_shift.y;
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

void mapviewer_hint_object_area::render()
{
	int px = 0;
	int py = 0;
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01, 0);
	glUseProgramObjectARB(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_select);
	double x0, y0, x1, y1, x2, y2, x3, y3;
	MapCell* Item = m_object->cell();
	int bx;
	int by;
	bx = m_object->m_active_state->m_size.x;
	by = m_object->m_active_state->m_size.y;
	for (int i = -by+1; i < 1; i++)
	{
		for (int j = -bx+1; j <1; j++)
		{
				int x = px + (Item->x + j) - m_owner->m_center.x + m_owner->m_tile_count_x / 2;
				int y = py + (Item->y + i) - m_owner->m_center.y + m_owner->m_tile_count_y / 2;
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
	x0 = x * m_owner->m_tile_size_x;
	y0 = (m_owner->m_tile_count_y - y - 1) * m_owner->m_tile_size_y;
	x1 = x0;
	y1 = (m_owner->m_tile_count_y - y) * m_owner->m_tile_size_y;
	x2 = (x + 1) * m_owner->m_tile_size_x;
	y2 = y1;
	x3 = x2;
	y3 = y0;
	glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_preselect);
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

GUI_MapViewer::GUI_MapViewer(Application* app = nullptr)
{
	m_tile_count_x = 64;
	m_tile_count_y = 64;
	m_just_focused = false;
	m_is_moving = false;
	m_focus=nullptr;
	m_cursored = nullptr;
	m_cursor_x = 1;
	m_cursor_y = 1;
	start_moving += std::bind(&GUI_MapViewer::on_start_moving, this, std::placeholders::_1);
	move += std::bind(&GUI_MapViewer::on_move, this, std::placeholders::_1);
	end_moving += std::bind(&GUI_MapViewer::on_end_moving, this, std::placeholders::_1);
	m_shift.x = -1024 * 0.5 + m_tile_count_x * 32 * 0.5;
	m_shift.y = 1024 * 0.5 - m_tile_count_y * 18 * 0.5;
	LOG(INFO) << m_shift.x;
	LOG(INFO) << m_shift.y;
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

bool GUI_MapViewer::select(int x,int y)
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


//void GUI_MapViewer::render(GraphicalController* Graph, int px, int py)
//{
//	float light[4];
//	glBindFramebuffer(GL_FRAMEBUFFER, Graph->m_FBO);
//	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_BLEND);
//	glUseProgram(0);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glClear(GL_COLOR_BUFFER_BIT);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_02, 0);
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glClear(GL_COLOR_BUFFER_BIT);
//	int x0, y0, x1, y1, x2, y2, x3, y3;
//	bool IsDraw;
//	tile_t tile;
//	int x;
//	int y;
//	int xf;
//	int yf;
//	int dx;
//	glColor4d(1.0, 1.0, 1.0, 1.0);
//	dimension_t object_size;
//	for (int gy = m_tile_count_y - 1; gy > -6; gy--)
//	{
//		for (int gx = 0; gx < m_tile_count_x + 5; gx++)
//		{
//			x = m_center.x + gx - m_tile_count_x / 2;
//			y = m_center.y + gy - m_tile_count_y / 2;
//			xf = x - m_player->m_object->cell()->x;
//			yf = y - m_player->m_object->cell()->y;
//			if ((x<0) || (x>m_map->m_size.w - 1) || (y<0) || (y>m_map->m_size.h - 1))
//			{
//			}
//			else
//			{ 
//				if ((xf >= -m_player->m_fov->m_radius) && (xf <= m_player->m_fov->m_radius) && (yf >= -m_player->m_fov->m_radius) && (yf <= m_player->m_fov->m_radius))
//				{
//					if (m_player->m_fov->m_map[m_player->m_fov->m_middle + yf][m_player->m_fov->m_middle + xf].visible)
//					{
//						light[0] = m_map->m_items[y][x]->m_light.R / 100.0F;
//						light[1] = m_map->m_items[y][x]->m_light.G / 100.0F;
//						light[2] = m_map->m_items[y][x]->m_light.B / 100.0F;
//						light[3] = 0.0;
//						for (std::list<GameObject*>::iterator Current = m_map->m_items[y][x]->m_items.begin(); Current != m_map->m_items[y][x]->m_items.end(); Current++)
//						{
//							IsDraw = false;
//							if ((*Current)->cell()->y == m_map->m_items[y][x]->y)
//							{
//								object_size = (*Current)->m_active_state->m_tile_size;
//								dx = object_size.w + m_map->m_items[y][x]->x - (*Current)->cell()->x;
//								x0 = (px + gx)*m_tile_size_x;
//								y0 = (m_tile_count_y - py - gy - object_size.h)*m_tile_size_y;
//								x1 = x0;
//								y1 = (m_tile_count_y - py - gy)*m_tile_size_y;
//								x2 = (px + gx + 1)*m_tile_size_x;
//								y2 = y1;
//								x3 = x2;
//								y3 = y0;
//								IsDraw = true;
//							}
//							if (IsDraw)
//							{
//								glUseProgram(Graph->m_tile_shader);
//								(*Current)->m_active_state->m_tile_manager->set_tile(tile, (*Current), Application::instance().m_timer->m_tick / 7.0*3.0, dx);
//								GLuint Sprite = tile.unit;
//								glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//								glActiveTexture(GL_TEXTURE0);
//								glBindTexture(GL_TEXTURE_2D, Sprite);
//								Graph->set_uniform_sampler(Graph->m_tile_shader, "Map");
//								Graph->set_uniform_vector(Graph->m_tile_shader, "light", light);
//								glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
//								Graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
//							/*	glDisable(GL_BLEND);
//								glDisable(GL_TEXTURE_2D);
//								glColor4d(1.0, 1.0, 1.0, 1.0);
//								glBegin(GL_LINES);
//								glVertex2d(x0, y0);
//								glVertex2d(x1, y1);
//								glVertex2d(x1, y1);
//								glVertex2d(x2, y2);
//								glVertex2d(x2, y2);
//								glVertex2d(x3, y3);
//								glVertex2d(x3, y3);
//								glVertex2d(x0, y0);
//								glEnd();
//								glEnable(GL_BLEND);
//								glEnable(GL_TEXTURE_2D);*/
//								if ((*Current)->cell()->x == m_map->m_items[y][x]->x)
//								{
//									auto* feat = (*Current)->get_parameter(interaction_e::health);
//									if (feat)
//									{
//										float x0, y0, x1, y1, x2, y2, x3, y3;
//										float h = feat->m_basic_value / 100.0;
//										x0 = (px + gx - object_size.w / 2.0 + 1)*m_tile_size_x - 32;
//										y0 = (m_tile_count_y - py - gy - object_size.h)*m_tile_size_y-8;
//										x1 = x0;
//										y1 = (m_tile_count_y - py - gy - object_size.h)*m_tile_size_y;
//										x2 = (px + gx - object_size.w / 2.0 + 1)*m_tile_size_x + 32;
//										y2 = y1;
//										x3 = x2;
//										y3 = y0;
//										glUseProgram(0);
//										glDisable(GL_TEXTURE_2D);
//										glColor4d(1.0-h, h, 0.0, 0.5);
//										glBegin(GL_LINES);
//										glVertex2d(x0, y0);
//										glVertex2d(x1, y1);
//										glVertex2d(x1, y1);
//										glVertex2d(x2, y2);
//										glVertex2d(x2, y2);
//										glVertex2d(x3, y3);
//										glVertex2d(x3, y3);
//										glVertex2d(x0, y0);
//										glEnd();
//										x0 = (px + gx - object_size.w / 2.0 + 1)*m_tile_size_x - 32;
//										x1 = x0;
//										x2 = x0 + (h) * 64;
//										x3 = x2;
//										Graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
//										glEnable(GL_TEXTURE_2D);
//									}
//								}
//								if ((*Current)->m_active_state->m_layer == 1)
//								{
//									glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_02, 0);
//									Graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
//								}
//								if ((*Current)->m_active_state->m_layer == 2)
//								{
//									glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
//									glUseProgram(Graph->m_mask_shader);
//									glBindTexture(GL_TEXTURE_2D, Graph->m_empty_02);
//									Graph->set_uniform_sampler(Graph->m_mask_shader, "Map");
//									Graph->draw_tile_FBO(x1 / 1024.0, (1024 - y1) / 1024.0, x3 / 1024.0, (1024 - y3) / 1024.0, x0, y0, x1, y1, x2, y2, x3, y3);
//								}
//								if ((*Current)->m_selected)
//								{
//									glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
//									glUseProgram(0);
//									glColor4d(0.0, abs((Application::instance().m_timer->m_tick - 3.5) / 3.5), 0.0, 1.0);
//									glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//									glActiveTexture(GL_TEXTURE0);
//									glBindTexture(GL_TEXTURE_2D, Sprite);
//									Graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
//									glColor4d(1.0, 1.0, 1.0, 1.0);
//								}
//							}
//						}
//					}
//					else
//					{
//						/*object_size = dimension_t(1, 1);
//						dx = object_size.w;
//						x0 = (px + gx)*m_tile_size_x;
//						y0 = (m_tile_count_y - py - gy - object_size.h)*m_tile_size_y;
//						x1 = x0;
//						y1 = (m_tile_count_y - py - gy)*m_tile_size_y;
//						x2 = (px + gx + 1)*m_tile_size_x;
//						y2 = y1;
//						x3 = x2;
//						y3 = y0;
//						glUseProgram(0);
//						glEnable(GL_BLEND);
//						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//						glEnable(GL_TEXTURE_2D);
//						glActiveTexture(GL_TEXTURE0);
//						glBindTexture(GL_TEXTURE_2D, Graph->m_no_visible);
//						glColor4f(1.0, 1.0, 1.0, 1.0);
//						Graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);*/
//					}
//				}
//			}
//		}
//	}
//	if (m_cursored != nullptr)
//	{
//		MapCell* Item = m_cursored;
//		int x = px + Item->x - m_center.x + m_tile_count_x / 2;
//		int y = py + Item->y - m_center.y + m_tile_count_y / 2;
//		x0 = (x - m_cursor_x + 1) * m_tile_size_x;
//		y0 = (m_tile_count_y - y - m_cursor_y) * m_tile_size_y;
//		x1 = (x - m_cursor_x + 1) * m_tile_size_x;
//		y1 = (m_tile_count_y - y) * m_tile_size_y;
//		x2 = (x + 1) * m_tile_size_x;
//		y2 = (m_tile_count_y - y) * m_tile_size_y;
//		x3 = (x + 1) * m_tile_size_x;
//		y3 = (m_tile_count_y - y - m_cursor_y) * m_tile_size_y;
//		glUseProgram(0);
//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//		glEnable(GL_TEXTURE_2D);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, Graph->m_select);
//		glColor4f(1.0, 1.0, 1.0, 0.5);
//		Graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
//		glDisable(GL_BLEND);
//		glDisable(GL_TEXTURE_2D);
//		glBegin(GL_LINES);
//		glColor4f(0.0, 1.0, 0.0, 1.0);
//		glVertex2d(x0, y0);
//		glVertex2d(x1, y1);
//		glVertex2d(x1, y1);
//		glVertex2d(x2, y2);
//		glVertex2d(x2, y2);
//		glVertex2d(x3, y3);
//		glVertex2d(x3, y3);
//		glVertex2d(x0, y0);
//		glEnd();
//		glEnable(GL_BLEND);
//		glEnable(GL_TEXTURE_2D);
//		glColor4f(1.0, 1.0, 1.0, 1.0);
//	}
//	for (std::list<gui_mapviewer_hint*>::iterator current = m_hints.begin(); current != m_hints.end(); current++)
//	{
//		(*current)->render();
//	}
//	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
//	glUseProgram(0);
//	glDisable(GL_BLEND);
//	glBindTexture(GL_TEXTURE_2D, Graph->m_empty_01);
//	glColor4d(1.0, 1.0, 1.0, 1.0);
//	//glGenerateMipmap(GL_TEXTURE_2D);
//	//Graph->draw_sprite(0, 0, 0, 1024/8, 1024/8, 1024/8, 1024/8, 0);
//	//Graph->blur_rect(0, 0, 1024 / 8, 1024 / 8);
//	//glBindTexture(GL_TEXTURE_2D, Graph->m_blur);
//	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 1024 - 1024 / 8, 1024 / 8, 1024 / 8);
//	Graph->draw_sprite(0, 0, 0, 1024, 1024, 1024, 1024, 0);
//	glDisable(GL_TEXTURE_2D);
//}

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
	int x0, y0, x1, y1, x2, y2, x3, y3;
	bool IsDraw;
	tile_t tile;
	int x;
	int y;
	int xf;
	int yf;
	int dx;
	glColor4d(1.0, 1.0, 1.0, 1.0);
	dimension_t object_size;
	game_object_size_t size3d;
	for (int gy = m_tile_count_y - 1; gy > -6; gy--)
	{
		for (int gx = m_tile_count_x - 1; gx > -6 ; gx--)
		{
			x = m_center.x + gx - m_tile_count_x / 2;
			y = m_center.y + gy - m_tile_count_y / 2;
			xf = x - m_player->m_object->cell()->x;
			yf = y - m_player->m_object->cell()->y;
			if ((x<0) || (x>m_map->m_size.w - 1) || (y<0) || (y>m_map->m_size.h - 1))
			{
			}
			else
			{
				if ((xf >= -m_player->m_fov->m_radius) && (xf <= m_player->m_fov->m_radius) && (yf >= -m_player->m_fov->m_radius) && (yf <= m_player->m_fov->m_radius))
				{
					//if (m_player->m_fov->m_map[m_player->m_fov->m_middle + yf][m_player->m_fov->m_middle + xf].visible)
					{
						light[0] = m_map->m_items[y][x]->m_light.R / 100.0F;
						light[1] = m_map->m_items[y][x]->m_light.G / 100.0F;
						light[2] = m_map->m_items[y][x]->m_light.B / 100.0F;
						light[3] = 0.0;
						/*light[0] = 1.0;
						light[1] = 1.0;
						light[2] = 1.0;
						light[3] = 0.0;*/
						for (std::list<GameObject*>::iterator Current = m_map->m_items[y][x]->m_items.begin(); Current != m_map->m_items[y][x]->m_items.end(); Current++)
						{
							IsDraw = false;
							if ((*Current)->cell()->y == m_map->m_items[y][x]->y && (*Current)->cell()->x == m_map->m_items[y][x]->x)
							{
								object_size = (*Current)->m_active_state->m_tile_size;
								size3d = (*Current)->m_active_state->m_size;
								int yp = m_tile_count_x - px - gx;
								int xp = m_tile_count_y - py - gy;
								dx = object_size.w + m_map->m_items[y][x]->x - (*Current)->cell()->x;
								x0 = (xp - yp) * 16 + m_shift.x - (size3d.y-1)*16;
								y0 = (xp + yp) * 9 - (object_size.h) + m_shift.y;
								x1 = x0;
								y1 = (xp + yp) * 9 + m_shift.y;
								x2 = (xp - yp) * 16 + object_size.w + m_shift.x - (size3d.y - 1) * 16;
								y2 = y1;
								x3 = x2;
								y3 = y0;
								IsDraw = true;
							}
							if (IsDraw)
							{
								glUseProgram(Graph->m_tile_shader);
								(*Current)->m_active_state->m_tile_manager->set_tile(tile, (*Current), Application::instance().m_timer->m_tick / 7.0*3.0, dx);
								GLuint Sprite = tile.unit;
								glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
								glActiveTexture(GL_TEXTURE0);
								glBindTexture(GL_TEXTURE_2D, Sprite);
								Graph->set_uniform_sampler(Graph->m_tile_shader, "Map");
								Graph->set_uniform_vector(Graph->m_tile_shader, "light", light);
								glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
								Graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
								/*	glDisable(GL_BLEND);
								glDisable(GL_TEXTURE_2D);
								glColor4d(1.0, 1.0, 1.0, 1.0);
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
								glEnable(GL_TEXTURE_2D);*/
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
									glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_02, 0);
									Graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
								}
								if ((*Current)->m_active_state->m_layer == 2)
								{
									glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
									glUseProgram(Graph->m_mask_shader);
									glBindTexture(GL_TEXTURE_2D, Graph->m_empty_02);
									Graph->set_uniform_sampler(Graph->m_mask_shader, "Map");
									Graph->draw_tile_FBO(x1 / 1024.0, (1024 - y1) / 1024.0, x3 / 1024.0, (1024 - y3) / 1024.0, x0, y0, x1, y1, x2, y2, x3, y3);
								}
						/*		if ((*Current)->m_selected)
								{
									glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
									glUseProgram(0);
									glColor4d(0.0, abs((Application::instance().m_timer->m_tick - 3.5) / 3.5), 0.0, 1.0);
									glBlendFunc(GL_SRC_ALPHA, GL_ONE);
									glActiveTexture(GL_TEXTURE0);
									glBindTexture(GL_TEXTURE_2D, Sprite);
									Graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
									glColor4d(1.0, 1.0, 1.0, 1.0);
								}*/
							}
						}
					}
				//	else
					{
						/*object_size = dimension_t(1, 1);
						dx = object_size.w;
						x0 = (px + gx)*m_tile_size_x;
						y0 = (m_tile_count_y - py - gy - object_size.h)*m_tile_size_y;
						x1 = x0;
						y1 = (m_tile_count_y - py - gy)*m_tile_size_y;
						x2 = (px + gx + 1)*m_tile_size_x;
						y2 = y1;
						x3 = x2;
						y3 = y0;
						glUseProgram(0);
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glEnable(GL_TEXTURE_2D);
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, Graph->m_no_visible);
						glColor4f(1.0, 1.0, 1.0, 1.0);
						Graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);*/
					}
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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Graph->m_select);
		glColor4f(0.0, 1.0, 0.0, 0.25);
		Graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
		x0 = (xp - yp) * 16 + m_shift.x;
		y0 = (xp + yp) * 9 - 9 + m_shift.y;
		x1 = (xp - yp) * 16 + 16 +m_shift.x;
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
	for (std::list<gui_mapviewer_hint*>::iterator current = m_hints.begin(); current != m_hints.end(); current++)
	{
		(*current)->render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	glUseProgram(0);
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, Graph->m_empty_01);
	glColor4d(1.0, 1.0, 1.0, 1.0);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//Graph->draw_sprite(0, 0, 0, 1024/8, 1024/8, 1024/8, 1024/8, 0);
	//Graph->blur_rect(0, 0, 1024 / 8, 1024 / 8);
	//glBindTexture(GL_TEXTURE_2D, Graph->m_blur);
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 1024 - 1024 / 8, 1024 / 8, 1024 / 8);
	Graph->draw_sprite(0, 0, 0, 1024, 1024, 1024, 1024, 0);
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
	case VK_SPACE:
		if (Application::instance().m_message_queue.m_reader)
		{
			Parameter* p = new Parameter(ParameterKind_Cancel);
			Application::instance().m_message_queue.push(p);
		}
		return;
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
				P_object_owner* pr = new P_object_owner(ParameterKind::parameter_kind_cell);
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
		position_t p =local_xy(e.position);
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
	focused=state;
	if(state)
	{
		if(m_focus!=nullptr)
		{
			m_focus->set_focus(false);
		}
		get_focus(this);
	} else {
		if(m_focus!=nullptr)
		{
			m_focus->set_focus(false);
		}
		lose_focus(this);
	}
}

void GUI_MapViewer::on_item_get_focus(GUI_Object* sender)
{
	if(m_focus!=sender)
	{
		set_focus(true);
		m_focus=sender;
	}
}

void GUI_MapViewer::on_lose_focus(GUI_Object* sender)
{
	m_cursored = nullptr;
}

position_t GUI_MapViewer::local_xy(position_t p)
{
	position_t Result = position_t(m_tile_count_x - (m_tile_count_y - (p.x - m_shift.x) / 32 - m_tile_count_x + (p.y - m_shift.y) / 18 + 1), m_tile_count_y - ((p.x - m_shift.x) / 32 + (p.y - m_shift.y) / 18));
	LOG(INFO) << Result.x << "   " << Result.y;
	if (Result.x > m_tile_count_x - 1){ Result.x = m_tile_count_x - 1; }
	if (Result.x <0){ Result.x = 0; }
	if (Result.y > m_tile_count_y - 1){ Result.y = m_tile_count_y - 1; }
	if (Result.y <0){ Result.y = 0; }
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