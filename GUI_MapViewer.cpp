#include "GUI_MapViewer.h"
#include "GUI_Layer.h"
#include "GUI_PopMenu.h"

GCMapElement::GCMapElement()
{
}

GCMapElement::~GCMapElement(void)
{
}

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
	for (int i = 0; i<128; i++)
	{
		for (int j = 0; j<64; j++)
		{
			m_items[i][j]=new GCMapElement();
			m_items[i][j]->get_focus+=std::bind(&GUI_MapViewer::on_item_get_focus,this,std::placeholders::_1);
			m_items[i][j]->m_position.x = j;
			m_items[i][j]->m_position.y = i;
		}
	}
	start_moving += std::bind(&GUI_MapViewer::on_start_moving, this, std::placeholders::_1);
	move += std::bind(&GUI_MapViewer::on_move, this, std::placeholders::_1);
	end_moving += std::bind(&GUI_MapViewer::on_end_moving, this, std::placeholders::_1);
}

GUI_MapViewer::~GUI_MapViewer(void)
{
}

void GUI_MapViewer::calculate()
{
	m_tile_size_x = static_cast<double>(m_size.x) / static_cast<double>(m_tile_count_x);
	m_tile_size_y = static_cast<double>(m_size.y) / static_cast<double>(m_tile_count_y);
}

void GUI_MapViewer::update()
{
	calculate();
	int x;
	int y;
	for (int i = 0; i<m_tile_count_y; i++)
	{
		for (int j = 0; j<m_tile_count_x; j++)
		{
			x = m_center.x + j - m_tile_count_x / 2;
			y = m_center.y + i - m_tile_count_y / 2;
			if((x<0)||(x>m_map->m_size.x-1)||(y<0)||(y>m_map->m_size.y-1))
			{
				m_items[i][j]->m_map_element=nullptr;
			} else {
				m_items[i][j]->m_map_element=m_map->m_items[y][x];
			}
		}
	}
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
		glEnable(GL_TEXTURE_2D);
		glEnable (GL_BLEND);
		glColor4d(1.0,1.0,1.0,1.0);
			double x0,y0,x1,y1,x2,y2,x3,y3;
			float LightInfo[4];
			float LightPos[4];
			bool IsDraw;
			for (int y = m_tile_count_y - 1; y>-1; y--)
			{
				for (int x = 0; x<m_tile_count_x; x++)
				{
					if(m_items[y][x]->m_map_element!=nullptr)
					{
						for(std::list<GameObject*>::iterator Current=m_items[y][x]->m_map_element->m_items.begin();Current!=m_items[y][x]->m_map_element->m_items.end();Current++)
						{
							IsDraw=false;
							if((*Current)->m_cell->x==m_items[y][x]->m_map_element->x&&(*Current)->m_cell->y==m_items[y][x]->m_map_element->y)
							{
								x0 = (px + x + 1 - (*Current)->m_size.x)*m_tile_size_x;
								y0 = (m_tile_count_y - py - y - (*Current)->m_size.z)*m_tile_size_y;
								x1 = (px + x + 1 - (*Current)->m_size.x)*m_tile_size_x;
								y1 = (m_tile_count_y - py - y)*m_tile_size_y;
								x2 = (px + x + 1)*m_tile_size_x;
								y2 = (m_tile_count_y - py - y)*m_tile_size_y;
								x3 = (px + x + 1)*m_tile_size_x;
								y3 = (m_tile_count_y - py - y - (*Current)->m_size.z)*m_tile_size_y;
								LightPos[0]=m_items[y][x]->m_map_element->x;
								LightPos[1]=m_items[y][x]->m_map_element->y;
								LightPos[2]=0;
								LightPos[3]=0;
								IsDraw=true;
							} else {
								/*if((x==Width-1&&Current->Item->Cell->y==Items[y][x]->y)||(y==0&&Current->Item->Cell->x==Items[y][x]->x)||(x==Width-1&&y==0))
								{
									x0=(Current->Item->Cell->x+1-Current->Item->Width)*32;
									y0=(64-Current->Item->Cell->y-Current->Item->Height)*16;
									x1=(Current->Item->Cell->x+1-Current->Item->Width)*32;
									y1=(64-Current->Item->Cell->y)*16;
									x2=(Current->Item->Cell->x+1)*32;
									y2=(64-Current->Item->Cell->y)*16;
									x3=(Current->Item->Cell->x+1)*32;
									y3=(64-Current->Item->Cell->y-Current->Item->Height)*16;
									LightPos[0]=Current->Item->Cell->x;
									LightPos[1]=Current->Item->Cell->y;
									LightPos[2]=0;
									LightPos[3]=0;
									IsDraw=true;
								}*/
							}
							if(IsDraw)
							{
								unsigned int Sprite = (*Current)->m_sprites[(*Current)->m_direction][Application::instance().m_timer->m_tick / 2];
								(*Current)->Render(x,y);
								glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
								glUseProgramObjectARB(Graph->m_render_shader);
								glActiveTextureARB(GL_TEXTURE0_ARB);
								glBindTexture( GL_TEXTURE_2D, Graph->m_sprites[Sprite]);
								Graph->set_uniform_sampler(Graph->m_render_shader,"Map");
								Graph->draw_sprite(x0,y0,x1,y1,x2,y2,x3,y3);
								glUseProgramObjectARB(Graph->m_light_shader);
								glActiveTextureARB(GL_TEXTURE0_ARB);
								glBindTexture(GL_TEXTURE_2D, Graph->m_sprites[Sprite]);
								Graph->set_uniform_sampler(Graph->m_light_shader,"Map");
								glBlendFunc (GL_SRC_ALPHA, GL_ONE);
								for(std::list<GameObject*>::iterator CurrentLight=m_map->m_lights.begin();CurrentLight!=m_map->m_lights.end();CurrentLight++)
								{
									LightPos[2]=(*CurrentLight)->m_cell->x;
									LightPos[3]=(*CurrentLight)->m_cell->y;
									Graph->set_uniform_vector(Graph->m_light_shader,"LightPos",LightPos);
									LightInfo[0]=(*CurrentLight)->m_light->RGB[0];
									LightInfo[1]=(*CurrentLight)->m_light->RGB[1];
									LightInfo[2]=(*CurrentLight)->m_light->RGB[2];
									LightInfo[3]=(*CurrentLight)->m_light->Power;
									Graph->set_uniform_vector(Graph->m_light_shader,"LightInfo",LightInfo);
									//char buf2[32];
									//itoa(glGetAttribLocationARB(Graph->m_light_shader,"in_vertex"), buf2, 10);
									//MessageBox(0, buf2, "Caption", MB_OK);
									Graph->draw_sprite(x0,y0,x1,y1,x2,y2,x3,y3);
								}
								if ((*Current)->m_selected)
								{
									glColor4d(0.0, abs((Application::instance().m_timer->m_tick - 3.5) / 3.5), 0.0, 1.0);
									glBlendFunc(GL_SRC_ALPHA, GL_ONE);
									glUseProgramObjectARB(0);
									glActiveTextureARB(GL_TEXTURE0_ARB);
									glBindTexture(GL_TEXTURE_2D, Graph->m_sprites[Sprite]);
									Graph->draw_sprite(x0, y0, x1, y1, x2, y2, x3, y3);
									glColor4d(1.0, 1.0, 1.0, 1.0);
								}
							}
						}
					}
				}
			}
			if(m_cursored != nullptr)
			{
				GCMapElement* Item = m_cursored;
				if(Item->m_map_element!=nullptr)
				{
					int x = px+Item->m_position.x;
					int y =py+ Item->m_position.y;
					x0 = (x - m_cursor_x + 1) * m_tile_size_x;
					y0 = (m_tile_count_y - y - m_cursor_y) * m_tile_size_y;
					x1 = (x - m_cursor_x + 1) * m_tile_size_x;
					y1 = (m_tile_count_y - y) * m_tile_size_y;
					x2 = (x + 1) * m_tile_size_x;
					y2 = (m_tile_count_y - y) * m_tile_size_y;
					x3 = (x + 1) * m_tile_size_x;
					y3 = (m_tile_count_y - y - m_cursor_y) * m_tile_size_y;
					glUseProgramObjectARB(0);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glEnable(GL_TEXTURE_2D);
					glActiveTextureARB(GL_TEXTURE0_ARB);
					glBindTexture(GL_TEXTURE_2D, Graph->m_sprites[8]);
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
					glColor4f(1.0, 1.0, 1.0, 1);
				}
			}
			glDisable(GL_BLEND);
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
		Application::instance().m_action_manager->m_items.push_back(new GameTask(Application::instance().m_actions[ActionKind_Move], P));
		P->~Parameter_Position();
		//app->TurnEnd=true;
		return;
	case VK_DOWN:
		P = new Parameter_Position();
		P->m_object=m_player;
		P->m_place=m_map->m_items[m_player->m_cell->y-1][m_player->m_cell->x];
		P->m_map=m_map;
		Application::instance().m_action_manager->m_items.push_back(new GameTask(Application::instance().m_actions[ActionKind_Move], P));
		P->~Parameter_Position();
		//app->TurnEnd=true;
		return;
	case VK_LEFT:
		P = new Parameter_Position();
		P->m_object=m_player;
		P->m_place=m_map->m_items[m_player->m_cell->y][m_player->m_cell->x-1];
		P->m_map=m_map;
		Application::instance().m_action_manager->m_items.push_back(new GameTask(Application::instance().m_actions[ActionKind_Move], P));
		P->~Parameter_Position();
		//app->TurnEnd=true;
		return;
	case VK_RIGHT:
		P = new Parameter_Position();
		P->m_object=m_player;
		P->m_place=m_map->m_items[m_player->m_cell->y][m_player->m_cell->x+1];
		P->m_map=m_map;
		Application::instance().m_action_manager->m_items.push_back(new GameTask(Application::instance().m_actions[ActionKind_Move], P));
		P->~Parameter_Position();
		//app->TurnEnd=true;
		return;
	case VK_SPACE:
		if (Application::instance().m_message_queue.m_reader)
		{
			TParameter* p = new TParameter(ParameterKind_Cancel);
			Application::instance().m_message_queue.push(p);
		}
		return;
		}
}

void GUI_MapViewer::on_mouse_click(MouseEventArgs const& e)
{
	if (e.flags &MK_LBUTTON)
	{
		GPosition p = local_xy(GPosition(e.position.x, e.position.y));
		if (m_items[p.y][p.x] != nullptr)
		{
			m_items[p.y][p.x]->mouse_click(e);
			Parameter_MapCell* pr = new Parameter_MapCell();
			if ((m_items[p.y][p.x]->m_map_element != nullptr) && (!m_just_focused))
			{
				pr->m_place = m_items[p.y][p.x]->m_map_element;
				if (Application::instance().m_message_queue.m_reader)
				{
					Application::instance().m_message_queue.push(pr);
				}
			}
			m_just_focused = false;
		}
	} else {
		GUI_PopMenu* PopMenu;
		PopMenu=new GUI_PopMenu;
		PopMenu->m_position.x = e.position.x;
		PopMenu->m_position.y = e.position.y;
		GPosition p = local_xy(GPosition(e.position.x, e.position.y));
		if (m_items[p.y][p.x] != nullptr)
		{
			//UnderCursor(CursorEventArgs(e.x, e.y));
			for (std::list<GameObject*>::iterator Current = m_items[p.y][p.x]->m_map_element->m_items.begin(); Current !=m_items[p.y][p.x]->m_map_element->m_items.end(); Current++)
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

void GUI_MapViewer :: on_mouse_down(MouseEventArgs const& e)
{
	if ((e.flags &MK_LBUTTON) || (e.flags &MK_RBUTTON))
	{
		GPosition p =local_xy(e.position);
		if (m_items[p.y][p.x] != nullptr)
		{
			m_items[p.y][p.x]->mouse_down(e);
		}

	}
}

void GUI_MapViewer::on_mouse_wheel(MouseEventArgs const& e)
{
	int delta = GET_WHEEL_DELTA_WPARAM(e.flags);
	if (delta>0)
	{
		m_tile_count_x += 2;
		m_tile_count_y += 4;
		if (m_tile_count_x > 64){ m_tile_count_x = 64; }
		if (m_tile_count_y > 128){ m_tile_count_y = 128; }
	}
	else
	{
		m_tile_count_x -= 2;
		m_tile_count_y -= 4;
		if (m_tile_count_x < 16){ m_tile_count_x = 16; }
		if (m_tile_count_y < 32){ m_tile_count_y = 32; }
	}
	calculate();
	GPosition p = local_xy(GPosition(e.position.x, e.position.y));
	if (m_items[p.y][p.x] != nullptr)
	{
		m_cursored = m_items[p.y][p.x];
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

void GUI_MapViewer::on_under_cursor(MouseEventArgs const& e)
{
	GPosition p =local_xy(GPosition(e.position.x,e.position.y));
	if (m_items[p.y][p.x] != nullptr)
	{
		m_cursored = m_items[p.y][p.x];
	}
}

void GUI_MapViewer::on_lose_focus(GUI_Object* sender)
{
	m_cursored = nullptr;
}

GPosition GUI_MapViewer::local_xy(GPosition p)
{
	GPosition Result = GPosition(p.x / m_tile_size_x, m_tile_count_y - p.y / m_tile_size_y);
	if (Result.x > m_tile_count_x - 1){ Result.x = m_tile_count_x - 1; }
	if (Result.x <0){ Result.x = 0; }
	if (Result.y > m_tile_count_y - 1){ Result.y = m_tile_count_y - 1; }
	if (Result.y <0){ Result.y = 0; }
	return Result;
}
void GUI_MapViewer::on_mouse_move(MouseEventArgs const& e)
{
	if (e.flags&MK_LBUTTON)
	{
		if (!m_is_moving)
		{
			start_moving(e);
		}
		move(e);
	}
	else
	{
		if (m_is_moving)
		{
			end_moving(e);
		}
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
	GPosition p = local_xy(GPosition(e.position.x, e.position.y));
	m_initial_position.x = m_center.x - p.x;
	m_initial_position.y = m_center.y - p.y;
	//MessageBox(NULL, "Start", "", MB_OK);
}

void GUI_MapViewer::on_move(MouseEventArgs const& e)
{
	GPosition p = local_xy(GPosition(e.position.x, e.position.y));
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
	return MouseEventArgs(GPosition(source.position.x - m_position.x, source.position.y - m_position.y), source.flags);
}