#include "game/graphics/GUI_MapViewer.h"
#include "game/graphics/GUI_Layer.h"
#include "game/graphics/GUI_PopMenu.h"
#include "game/ActionManager.h"


GUI_MapViewer::GUI_MapViewer(Application* app = nullptr)
{
	m_tile_count_x = 32;
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
}

GUI_MapViewer::~GUI_MapViewer(void)
{
}

void GUI_MapViewer::calculate()
{
	m_tile_size_x = static_cast<double>(m_size.w) / static_cast<double>(m_tile_count_x);
	m_tile_size_y = static_cast<double>(m_size.h) / static_cast<double>(m_tile_count_y);
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
	double x0, y0, x1, y1, x2, y2, x3, y3;
	bool IsDraw;
	tile_t tile;
	int x;
	int y;
	int dx;
	glColor4d(1.0, 1.0, 1.0, 1.0);
	dimension_t object_size;
	for (int gy = m_tile_count_y - 1; gy > -6; gy--)
	{
		for (int gx = 0; gx < m_tile_count_x + 5; gx++)
		{
			x = m_center.x + gx - m_tile_count_x / 2;
			y = m_center.y + gy - m_tile_count_y / 2;
			if ((x<0) || (x>m_map->m_size.w - 1) || (y<0) || (y>m_map->m_size.h - 1))
			{
			}
			else
			{
				light[0] = m_map->m_items[y][x]->m_light.R / 100.0;
				light[1] = m_map->m_items[y][x]->m_light.G / 100.0;
				light[2] = m_map->m_items[y][x]->m_light.B / 100.0;
				light[3] = 0.0;
				for (std::list<GameObject*>::iterator Current = m_map->m_items[y][x]->m_items.begin(); Current != m_map->m_items[y][x]->m_items.end(); Current++)
				{
					IsDraw = false;
					if ((*Current)->m_cell->y == m_map->m_items[y][x]->y)
					{
						object_size = (*Current)->m_tile_size;
						dx = object_size.w + m_map->m_items[y][x]->x - (*Current)->m_cell->x;
						x0 = (px + gx)*m_tile_size_x;
						y0 = (m_tile_count_y - py - gy - object_size.h)*m_tile_size_y;
						x1 = x0;
						y1 = (m_tile_count_y - py - gy)*m_tile_size_y;
						x2 = (px + gx + 1)*m_tile_size_x;
						y2 = y1;
						x3 = x2;
						y3 = y0;
						IsDraw = true;
					}
					if (IsDraw)
					{
						glUseProgram(Graph->m_tile_shader);
						(*Current)->m_tile_manager->set_tile(tile, (*Current), Application::instance().m_timer->m_tick / 2,dx);
						GLuint Sprite = tile.unit;
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, Sprite);
						Graph->set_uniform_sampler(Graph->m_tile_shader, "Map");
						Graph->set_uniform_vector(Graph->m_tile_shader, "light", light);
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
						Graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
						if ((*Current)->m_layer == 1)
						{
							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_02, 0);
							Graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
						}
						if ((*Current)->m_layer == 2)
						{
							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
							glUseProgram(Graph->m_mask_shader);
							glBindTexture(GL_TEXTURE_2D, Graph->m_empty_02);
							Graph->set_uniform_sampler(Graph->m_mask_shader, "Map");
							Graph->draw_tile_FBO(x1 / 1024.0, (1024 - y1) / 1024.0, x3 / 1024.0, (1024 - y3) / 1024.0, x0, y0, x1, y1, x2, y2, x3, y3);
						}
						if ((*Current)->m_selected)
						{
							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graph->m_empty_01, 0);
							glUseProgram(0);
							glColor4d(0.0, abs((Application::instance().m_timer->m_tick - 3.5) / 3.5), 0.0, 1.0);
							glBlendFunc(GL_SRC_ALPHA, GL_ONE);
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, Sprite);
							Graph->draw_tile(tile, x0, y0, x1, y1, x2, y2, x3, y3);
							glColor4d(1.0, 1.0, 1.0, 1.0);
						}
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
		x0 = (x - m_cursor_x + 1) * m_tile_size_x;
		y0 = (m_tile_count_y - y - m_cursor_y) * m_tile_size_y;
		x1 = (x - m_cursor_x + 1) * m_tile_size_x;
		y1 = (m_tile_count_y - y) * m_tile_size_y;
		x2 = (x + 1) * m_tile_size_x;
		y2 = (m_tile_count_y - y) * m_tile_size_y;
		x3 = (x + 1) * m_tile_size_x;
		y3 = (m_tile_count_y - y - m_cursor_y) * m_tile_size_y;
		glUseProgram(0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Graph->m_select);
		glColor4f(1.0, 1.0, 1.0, 0.5);
		Graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
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
		glColor4f(1.0, 1.0, 1.0, 1.0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, Graph->m_empty_01);
	glColor4d(1.0, 1.0, 1.0, 1.0);
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
	case VK_UP:
		P = new Parameter_Position();
		P->m_object=m_player;
		P->m_place=m_map->m_items[m_player->m_cell->y+1][m_player->m_cell->x];
		P->m_map=m_map;
		Application::instance().m_action_manager->m_items.push_back(new GameTask(Application::instance().m_actions[action_e::move], P));
		P->~Parameter_Position();
		//app->TurnEnd=true;
		return;
	case VK_DOWN:
		P = new Parameter_Position();
		P->m_object=m_player;
		P->m_place=m_map->m_items[m_player->m_cell->y-1][m_player->m_cell->x];
		P->m_map=m_map;
		Application::instance().m_action_manager->m_items.push_back(new GameTask(Application::instance().m_actions[action_e::move], P));
		P->~Parameter_Position();
		//app->TurnEnd=true;
		return;
	case VK_LEFT:
		P = new Parameter_Position();
		P->m_object=m_player;
		P->m_place=m_map->m_items[m_player->m_cell->y][m_player->m_cell->x-1];
		P->m_map=m_map;
		Application::instance().m_action_manager->m_items.push_back(new GameTask(Application::instance().m_actions[action_e::move], P));
		P->~Parameter_Position();
		//app->TurnEnd=true;
		return;
	case VK_RIGHT:
		P = new Parameter_Position();
		P->m_object=m_player;
		P->m_place=m_map->m_items[m_player->m_cell->y][m_player->m_cell->x+1];
		P->m_map=m_map;
		Application::instance().m_action_manager->m_items.push_back(new GameTask(Application::instance().m_actions[action_e::move], P));
		P->~Parameter_Position();
		//app->TurnEnd=true;
		return;
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
				//m_map->m_items[p.y][p.x]->mouse_click(e);
				Parameter_MapCell* pr = new Parameter_MapCell();
				pr->m_place = m_map->m_items[y][x];
				if (Application::instance().m_message_queue.m_reader)
				{
					Application::instance().m_message_queue.push(pr);
				}
			}
		}
		m_just_focused = false;
	} else {
		GUI_PopMenu* PopMenu;
		PopMenu=new GUI_PopMenu;
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
			for (std::list<GameObject*>::iterator Current = m_map->m_items[y][x]->m_items.begin(); Current !=m_map->m_items[y][x]->m_items.end(); Current++)
			{
				PopMenu->add((*Current)->m_name, (*Current));
			}
		}
		if(PopMenu->m_items.size()==0)
		{
			PopMenu->add("Нет действий",nullptr);
		}
		PopMenu->destroy+=std::bind(&GUI_Layer::remove,m_GUI,std::placeholders::_1);
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
	position_t Result = position_t(p.x / m_tile_size_x, m_tile_count_y - p.y / m_tile_size_y);
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