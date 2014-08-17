#include "game/GameMap.h"
#include "game/GameObject.h"
#include "game/Application.h"


MapCell::MapCell(int x0, int y0)
{
	x = x0;
	y = y0;
}


void MapCell::add_object(GameObject* Object)
 {
	 m_items.push_back(Object);
 }

GameObjectProperty* MapCell::find_property(property_e kind,GameObject* excluded)
{
	GameObjectProperty* result(nullptr);
	for(std::list<GameObject*>::iterator item=m_items.begin();item!=m_items.end();item++)
	{
		if(excluded!=(*item))
		{
			result = (*item)->find_property(kind);
			if(result!=nullptr)
			{
				return result;
			}
		}
	}
	return nullptr;
}

bool MapCell::check_permit(property_e kind, GameObject* excluded)
{
	GameObjectProperty* result(nullptr);
	for (std::list<GameObject*>::iterator item = m_items.begin(); item != m_items.end(); item++)
	{
		if (excluded != (*item))
		{
			result = (*item)->find_property(kind);
			if (result == nullptr)
			{
				return false;
			}
		}
	}
	return true;
}

GameMap::GameMap(dimension_t size)
{
	m_update = true;
	GameObject* obj;
	m_size = size;
	m_items.resize(m_size.h);
	for (int i = 0; i < m_size.h; i++)
	{
		m_items[i].resize(m_size.w);
		for (int j = 0; j < m_size.w; j++)
		{
			//m_items[i][j] = nullptr;
			m_items[i][j] = new MapCell(j, i);
			obj = Application::instance().m_game_object_manager.new_object("dirt");
			//add_object(obj, m_items[i][j]);
		}
	}
	for (int y = 0; y < 21; y++)
	{
		for (int x = 0; x< 21; x++)
		{
			m_coefficient[y][x] = 5 * sqrt(x*x + y*y);
		}
	}
}

void GameMap::generate_room(void)
{
	for(int i=0;i<m_size.h;i++)
	{
		for(int j=0;j<m_size.w;j++)
		{
			if ((i==0)||(j==0)||(i==m_size.h-1)||(j==m_size.w-1))
			{
				m_items[i][j] = new MapCell(j, i);
				GameObject* obj = Application::instance().m_game_object_manager.new_object("wall");
				add_object(obj, m_items[i][j]);
			} else {
				m_items[i][j] = new MapCell(j, i);
				GameObject* obj = Application::instance().m_game_object_manager.new_object("floor");
				add_object(obj, m_items[i][j]);
			}
		}
	}
}


void GameMap::add_object(GameObject* object, MapCell* cell)
{
	object->m_cell = cell;
	for (int i = 0; i<object->m_size.y; i++)
	{
		for (int j = 0; j<object->m_size.x; j++)
		{
			m_items[cell->y + i][cell->x - j]->add_object(object);
		}
	}
}

void GameMap::remove_object(GameObject* object)
{
	for (int i = 0; i<object->m_size.y; i++)
	{
		for (int j = 0; j<object->m_size.x; j++)
		{
			m_items[object->m_cell->y + i][object->m_cell->x - j]->m_items.remove(object);
		}
	}
	object->m_cell = nullptr;
}

void GameMap::move_object(GameObject* object,MapCell* cell)
{
	remove_object(object);
	add_object(object, cell);
}

void  GameMap::turn_object(GameObject* object)
{
	MapCell* position = object->m_cell;
	remove_object(object);
	object->turn();
	add_object(object, position);
}

void GameMap::add_light(GameObject* Object)
{
	m_lights.push_front(Object);
}

void GameMap::add_new_object(GameObject* Object, MapCell* Element)
{
	add_object(Object, Element);
	add_light(Object);
}

void GameMap::generate_level(void)
{
	srand(time(0));
	block_t* block;
	block = new block_t(0, 0, m_size.w - 1, m_size.h - 1);
	divide_block(block, 5, 0);
	fill();
	connect_room();
	add_wall();
	add_doors();
	add_lighting();
}

bool GameMap::divide_block(block_t* block, int Depth, int Current)
{
	int choise(rand() % 100);
	block_t* sub_block;
	if (choise<50)
	{
		int Range = block->rect.w;
		if (Range<22){ return true; }
		choise = rand() % (Range - 6) + 11;
		if (Range - choise<11)
		{
			choise = 11;
			if (Range - choise - 11<11){ return true; }
		}
		if (Current<Depth)
		{
			sub_block = new block_t(block->rect.x, block->rect.y, choise, block->rect.h);
			if (divide_block(sub_block, Depth, Current + 1))
			{
				m_rooms.push_back(sub_block);
			}
			sub_block = new block_t(block->rect.x + choise, block->rect.y, block->rect.w - choise, block->rect.h);
			if (divide_block(sub_block, Depth, Current + 1))
			{
				m_rooms.push_back(sub_block);
			}
			return false;
		}
	}
	else {
		int Range = block->rect.h;
		if (Range<22){ return true; }
		choise = rand() % (Range - 6) + 11;
		if (Range - choise<11)
		{
			choise = 11;
			if (Range - choise - 11<11){ return true; }
		}
		if (Current<Depth)
		{
			sub_block = new block_t(block->rect.x, block->rect.y, block->rect.w, choise);
			if (divide_block(sub_block, Depth, Current + 1))
			{
				m_rooms.push_back(sub_block);
			}
			sub_block = new block_t(block->rect.x, block->rect.y + choise, block->rect.w, block->rect.h - choise);
			if (divide_block(sub_block, Depth, Current + 1))
			{
				m_rooms.push_back(sub_block);
			}
			return false;
		}
	}
	return true;
}

void GameMap::random_block(block_t* block)
{
	(*block) = block_t(block->rect.x + 1, block->rect.y + 1, block->rect.w - 2, block->rect.h - 2);
	int len;
	len = rand() % block->rect.h;
	if (len > 10)
	{
		block->rect.y = block->rect.y + rand() % (block->rect.h - len);
		block->rect.h = len;
	}
	len = rand() % block->rect.w;
	if (len > 10)
	{
		block->rect.x = block->rect.x + rand() % (block->rect.w - len);
		block->rect.w = len;
	}
}

void GameMap::fill()
{
	block_t* block;
	for (std::list<block_t*>::iterator Current = m_rooms.begin(); Current != m_rooms.end(); Current++)
	{
		block = (*Current);
		random_block(block);
		for (int i = block->rect.y; i < block->rect.y + block->rect.h + 1; i++)
		{
			for (int j = block->rect.x; j < block->rect.x + block->rect.w + 1; j++)
			{
				if ((i == block->rect.y) || (j == block->rect.x) || (i == block->rect.y + block->rect.h) || (j == block->rect.x + block->rect.w))
				{
				/*	m_items[i][j] = new MapCell(j, i);
					GameObject* obj = Application::instance().m_game_object_manager.new_object("wall");
					add_object(obj, m_items[i][j]);*/
				}
				else {
					m_items[i][j] = new MapCell(j, i);
					GameObject* obj = Application::instance().m_game_object_manager.new_object("floor");
					add_object(obj, m_items[i][j]);
				}
			}
		}
	}
}

void GameMap::connect_room()
{ 
	block_t* temp;
	int index;
	while (!m_rooms.empty())
	{
		temp = m_rooms.front();
		if (!m_link_rooms.empty())
		{
			index = rand() % m_link_rooms.size();
			auto nx = std::next(m_link_rooms.begin(),index);
			link_room(temp, *nx);
			m_link_rooms.push_front(temp);
			m_rooms.pop_front();
		}
		else {
			index = (rand() % (m_rooms.size() - 1)) + 1;
			auto nx = std::next(m_rooms.begin(), index);
			m_link_rooms.push_front(temp);
			m_link_rooms.push_front(*nx);
			link_room(temp, *nx);
			m_rooms.pop_front();
			m_rooms.remove(*nx);
		}
	}
}

void  GameMap::add_wall()
{ 
	bool empty;
	for (int y = 1; y < m_size.h-1; y++)
	{
		for (int x = 1; x< m_size.w-1; x++)
		{
			if (m_items[y][x]->m_items.empty())
			{
				empty = true;
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						if (!m_items[y+i][x +j]->m_items.empty()&&(!(i==0&&j==0)))
						{
							if (m_items[y + i][x + j]->m_items.front()->m_name == "floor")
								empty = false;
						}
					}
				}
				if (empty)
				{
					add_object(Application::instance().m_game_object_manager.new_object("dirt"), m_items[y][x]);
				}
				else
				{
					add_object(Application::instance().m_game_object_manager.new_object("wall"), m_items[y][x]);
				}
			}
		}
	}
}

void GameMap::link_room(block_t* a, block_t* b)
{
	position_t ac = position_t(a->rect.x + a->rect.w / 2, a->rect.y + a->rect.h / 2);
	position_t bc = position_t(b->rect.x + b->rect.w / 2, b->rect.y + b->rect.h / 2);
	int s;
	int e;
	if (ac.x < bc.x)
	{
		s = ac.x;
		e = bc.x;
	} else {
		e = ac.x;
		s = bc.x;
	}
	for (int i = s; i < e+4; i++)
	{
		for (int j = -3; j < 4; j++)
		{
			m_items[ac.y+j][i]->m_items.clear();
			add_object(Application::instance().m_game_object_manager.new_object("floor"), m_items[ac.y+j][i]);
		}
	}
	if (ac.y < bc.y)
	{
		s = ac.y;
		e = bc.y;
	}
	else {
		e = ac.y;
		s = bc.y;
	}
	for (int i = s; i < e+4; i++)
	{
		for (int j = -3; j < 4; j++)
		{
			m_items[i][bc.x+j]->m_items.clear();
			add_object(Application::instance().m_game_object_manager.new_object("floor"), m_items[i][bc.x + j]);
		}
	}
}

void  GameMap::add_doors()
{
	rectangle_t rect;
	int s;
	bool b;
	for (std::list<block_t*>::iterator Current = m_link_rooms.begin(); Current != m_link_rooms.end(); Current++)
	{
		rect = (*Current)->rect;
		s = rect.x;
		b = false;
		for (int i = rect.x; i < rect.x + rect.w+1; i++)
		{
			if (m_items[rect.y][i]->m_items.front()->m_name == "floor") 
			{ 
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (m_items[rect.y][i]->m_items.front()->m_name == "wall") 
			{
				if (b)
				{
					if (i - s == 7)
					{
						add_object(Application::instance().m_game_object_manager.new_object("wall"), m_items[rect.y][i - 1]);
						add_object(Application::instance().m_game_object_manager.new_object("door"), m_items[rect.y][i - 2]);
						add_object(Application::instance().m_game_object_manager.new_object("wall"), m_items[rect.y][i - 7]);
					}
				}
				b = false;
			}
		}
		s = rect.x;
		b = false;
		for (int i = rect.x; i < rect.x + rect.w + 1; i++)
		{
			if (m_items[rect.y+rect.h][i]->m_items.front()->m_name == "floor")
			{
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (m_items[rect.y + rect.h][i]->m_items.front()->m_name == "wall")
			{
				if (b)
				{
					if (i - s == 7)
					{
						add_object(Application::instance().m_game_object_manager.new_object("wall"), m_items[rect.y + rect.h][i - 1]);
						add_object(Application::instance().m_game_object_manager.new_object("door"), m_items[rect.y + rect.h][i - 2]);
						add_object(Application::instance().m_game_object_manager.new_object("wall"), m_items[rect.y + rect.h][i - 7]);
					}
				}
				b = false;
			}
		}
		s = rect.y;
		b = false;
		for (int i = rect.y; i < rect.y + rect.h + 1; i++)
		{
			if (m_items[i][rect.x]->m_items.front()->m_name == "floor")
			{
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (m_items[i][rect.x]->m_items.front()->m_name == "wall")
			{
				if (b)
				{
					if (i - s == 7)
					{
						add_object(Application::instance().m_game_object_manager.new_object("wall"), m_items[i - 1][rect.x]);
						GameObject* obj = Application::instance().m_game_object_manager.new_object("door");
						obj->set_tile_direction(ObjectDirection_Left);
						add_object(obj, m_items[i-6][rect.x]);
						add_object(Application::instance().m_game_object_manager.new_object("wall"), m_items[i - 7][rect.x]);
					}
				}
				b = false;
			}
		}
		s = rect.y;
		b = false;
		for (int i = rect.y; i < rect.y + rect.h + 1; i++)
		{
			if (m_items[i][rect.x + rect.w]->m_items.front()->m_name == "floor")
			{
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (m_items[i][rect.x + rect.w]->m_items.front()->m_name == "wall")
			{
				if (b)
				{
					if (i - s == 7)
					{
						add_object(Application::instance().m_game_object_manager.new_object("wall"), m_items[i - 1][rect.x + rect.w]);
						GameObject* obj = Application::instance().m_game_object_manager.new_object("door");
						obj->set_tile_direction(ObjectDirection_Left);
						add_object(obj, m_items[i - 6][rect.x + rect.w]);
						add_object(Application::instance().m_game_object_manager.new_object("wall"), m_items[i - 7][rect.x + rect.w]);
					}
				}
				b = false;
			}
		}
	}
}

//void  GameMap::blur_lighting()
//{
//	light_t l;
//	float Gauss[] = { 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 };
//	for (int y = 0; y < 41; y++)
//	{
//		for (int x = 0; x < 41; x++)
//		{
//			l = light_t();
//			for (int i = -4; i < 5; i++)
//			{
//				int mx = x + i;
//				int my = y;
//				if (mx < 0){ mx = 0; }
//				if (mx > 40){ mx = 40; }
//				if (my < 0){ my = 0; }
//				if (my > 40){ my = 40; }
//				l.R = l.R + m_local_light[0][my][mx].R*Gauss[abs(i)];
//				l.G = l.G + m_local_light[0][my][mx].G*Gauss[abs(i)];
//				l.B = l.B + m_local_light[0][my][mx].B*Gauss[abs(i)];
//			}
//			m_local_light[1][y][x] = l;
//		}
//	}
//	for (int y = 0; y < 41; y++)
//	{
//		for (int x = 0; x < 41; x++)
//		{
//			l = light_t();
//			for (int i = -4; i < 5; i++)
//			{
//				int mx = x;
//				int my = y + i;
//				if (mx < 0){ mx = 0; }
//				if (mx > 40){ mx = 40; }
//				if (my < 0){ my = 0; }
//				if (my > 40){ my = 40; }
//				l.R = l.R + m_local_light[1][my][mx].R*Gauss[abs(i)];
//				l.G = l.G + m_local_light[1][my][mx].G*Gauss[abs(i)];
//				l.B = l.B + m_local_light[1][my][mx].B*Gauss[abs(i)];
//			}
//			m_local_light[2][y][x] = l;
//		}
//	}
//}

void  GameMap::blur_lighting()
{
	light_t l;
	float Gauss[] = { 0.39894228, 0.241970725, 0.053990967 };
	for (int y = 0; y < 41; y++)
	{
		for (int x = 0; x < 41; x++)
		{
			l = light_t();
			for (int i = -2; i < 3; i++)
			{
				int mx = x + i;
				int my = y;
				if (mx < 0){ mx = 0; }
				if (mx > 40){ mx = 40; }
				if (my < 0){ my = 0; }
				if (my > 40){ my = 40; }
				l.R = l.R + m_local_light[0][my][mx].R*Gauss[abs(i)];
				l.G = l.G + m_local_light[0][my][mx].G*Gauss[abs(i)];
				l.B = l.B + m_local_light[0][my][mx].B*Gauss[abs(i)];
			}
			m_local_light[1][y][x] = l;
		}
	}
	for (int y = 0; y < 41; y++)
	{
		for (int x = 0; x < 41; x++)
		{
			l = light_t();
			for (int i = -2; i < 3; i++)
			{
				int mx = x;
				int my = y + i;
				if (mx < 0){ mx = 0; }
				if (mx > 40){ mx = 40; }
				if (my < 0){ my = 0; }
				if (my > 40){ my = 40; }
				l.R = l.R + m_local_light[1][my][mx].R*Gauss[abs(i)];
				l.G = l.G + m_local_light[1][my][mx].G*Gauss[abs(i)];
				l.B = l.B + m_local_light[1][my][mx].B*Gauss[abs(i)];
			}
			m_local_light[2][y][x] = l;
		}
	}
}

void  GameMap::calculate_lighting()
{
	int lx;
	int ly;
	int c;
	for (int y = 0; y < m_size.h; y++)
	{
		for (int x = 0; x < m_size.w; x++)
		{
			m_items[y][x]->m_light = light_t();
		}
	}
	for (std::list<GameObject*>::iterator Current = m_lights.begin(); Current != m_lights.end(); Current++)
	{
		for (int y = 0; y < 41; y++)
		{
			for (int x = 0; x < 41; x++)
			{
				m_barrier_map[y][x] = false;
				m_light_map[y][x] = false;
			}
		}
		for (int y = (*Current)->m_cell->y - 20; y < (*Current)->m_cell->y + 21; y++)
		{
			if (!((y<0) || (y>m_size.h - 1)))
			{
				for (int x = (*Current)->m_cell->x - 20; x < (*Current)->m_cell->x + 21; x++)
				{
					if (!((x<0) || (x>m_size.w - 1)))
					{
						for (std::list<GameObject*>::iterator obj = m_items[y][x]->m_items.begin(); obj != m_items[y][x]->m_items.end(); obj++)
						{
							if ((*obj)->m_name != "floor")
							{
								m_barrier_map[y - ((*Current)->m_cell->y - 20)][x - ((*Current)->m_cell->x - 20)] = true;
							}
						}
					}
				}
			}
		}
		m_barrier_map[20][20] = false;
		for (int y = 0; y < 41; y++)
		{
			for (int x = 0; x < 41; x++)
			{
				m_light_map[y][x] = line2(20, 20, x, y);
				m_light_map2[y][x] = m_light_map[y][x];
			}
		}

		for (int y = 1; y < 40; y++)
		{
			for (int x = 1; x < 40; x++)
			{
				if (m_barrier_map[y][x])
				{
					if (y > 20) //грубое прибллижение к модели освещения Ламберта (для всех объектов ниже источника освещения, косинус угла между нормалью и вектором из точки освещения будет меньше нуля, следовательно они будут в тени)
					{
						for (int i = -1; i < 2; i++)
						{
							for (int j = -1; j < 2; j++)
							{
								if (m_light_map[y + i][x + j])
								{
									m_light_map2[y][x] = true;
								}
							}
						}
					}
				}
			}
		}

		/*std::string a;
		for (int y = 20; y > -1; y--)
		{
		for (int x = 0; x < 21; x++)
		{
		if (y == 10 && x == 10)
		{
		a = a + "2";
		}
		else
		{
		if (m_light_map2[y][x])
		{
		a = a + "1";
		}
		else
		{
		a = a + "0";
		}
		}
		}
		a = a + "\r\n";
		}
		MessageBox(NULL, a.c_str(), "", MB_OK);*/


		for (int y = 0; y < 41; y++)
		{
			for (int x = 0; x < 41; x++)
			{
				if (m_light_map2[y][x])
				{
					lx = abs(x - 20);
					ly = abs(y - 20);
					c = (*Current)->m_light->R - m_coefficient[ly][lx];
					if (c < 0){ c = 0; }
					m_local_light[0][y][x].R = c;
					c = (*Current)->m_light->G - m_coefficient[ly][lx];
					if (c < 0){ c = 0; }
					m_local_light[0][y][x].G = c;
					c = (*Current)->m_light->B - m_coefficient[ly][lx];
					if (c < 0){ c = 0; }
					m_local_light[0][y][x].B = c;
				}
				else
				{
					m_local_light[0][y][x] = light_t();
				}
			}
		}
		blur_lighting();


		for (int y = 0; y < 41; y++)
		{
			ly = (*Current)->m_cell->y + y - 20;
			if (!((ly<0) || (ly>m_size.h - 1)))
			{
				for (int x = 0; x < 41; x++)
				{
					lx = (*Current)->m_cell->x + x - 20;
					if (!((lx<0) || (lx>m_size.w - 1)))
					{
						if (m_light_map2[y][x])
						{
							m_items[ly][lx]->m_light.R += m_local_light[2][y][x].R;
							m_items[ly][lx]->m_light.G += m_local_light[2][y][x].G;
							m_items[ly][lx]->m_light.B += m_local_light[2][y][x].B;
							/*m_items[ly][lx]->m_light = light_t(100,100,100);*/

						}
						else
						{
							m_items[ly][lx]->m_light.R += m_local_light[2][y][x].R;
							m_items[ly][lx]->m_light.G += m_local_light[2][y][x].G;
							m_items[ly][lx]->m_light.B += m_local_light[2][y][x].B;
							/*m_items[ly][lx]->m_light = light_t(100,100,100);*/
						}
					}
				}
			}
		}
	}
}


bool GameMap::line2(int x1, int y1, int x2, int y2)
{
	int e = 0;
	int de;
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int x;
	int y;
	int k;
	if (dx >= dy)
	{
		y = y1;
		k = (y1 < y2 ? 1 : -1);
		if (x1 < x2)
		{
			for (x = x1; x < x2+1; x++)
			{
				if (m_barrier_map[y][x]){ return false; }
				e = e + dy;
				if (2 * e >= dx)
				{
					y = y + k;
					e = e - dx;
				}
			}
		}
		else
		{
			for (x = x1; x > x2-1; x--)
			{
				if (m_barrier_map[y][x]){ return false; }
				e = e + dy;
				if (2 * e >= dx)
				{
					y = y + k;
					e = e - dx;
				}
			}
		}
	}
	else
	{
		x = x1;
		k = (x1 < x2 ? 1 : -1);
		if (y1 <= y2)
		{
			for (y = y1; y < y2+1; y++)
			{
				if (m_barrier_map[y][x]){ return false; }
				e = e + dx;
				if (2 * e >= dy)
				{
					x = x + k;
					e = e - dy;
				}
			}
		}
		else
		{

			for (y = y1; y > y2-1; y--)
			{
				if (m_barrier_map[y][x]){ return false; }
				e = e + dx;
				if (2 * e >= dy)
				{
					x = x + k;
					e = e - dy;
				}
			}
		}
	}
	return true;
}

void GameMap::add_lighting()
{
	block_t* block;
	for (std::list<block_t*>::iterator Current = m_link_rooms.begin(); Current != m_link_rooms.end(); Current++)
	{
		block = (*Current);
		int tk;
		std::string torch_kind[] = { "torch", "red torch", "green torch", "blue torch" };
		tk = rand() % 1;
		add_new_object(Application::instance().m_game_object_manager.new_object(torch_kind[tk]), m_items[block->rect.y + 1][block->rect.x + 1]);
		tk = rand() % 1;
		add_new_object(Application::instance().m_game_object_manager.new_object(torch_kind[tk]), m_items[block->rect.y + 1][block->rect.x + block->rect.w - 1]);
		tk = rand() % 1;
		add_new_object(Application::instance().m_game_object_manager.new_object(torch_kind[tk]), m_items[block->rect.y + block->rect.h - 1][block->rect.x + block->rect.w - 1]);
		tk = rand() % 1;
		add_new_object(Application::instance().m_game_object_manager.new_object(torch_kind[tk]), m_items[block->rect.y + block->rect.h - 1][block->rect.x + 1]);
		int j=0;
		for (int i = block->rect.x + 1; i < block->rect.x + block->rect.w; i++)
		{
			if (j == 30)
			{
				tk = rand() % 4;
				add_new_object(Application::instance().m_game_object_manager.new_object(torch_kind[tk]), m_items[block->rect.y + 1][i]);
				tk = rand() % 4;
				add_new_object(Application::instance().m_game_object_manager.new_object(torch_kind[tk]), m_items[block->rect.y + block->rect.h - 1][i]);
				j = -1;
			}
			j = j + 1;
		}
		j = 0;
		for (int i = block->rect.y + 1; i < block->rect.y + block->rect.h; i++)
		{
			if (j == 30)
			{
				tk = rand() % 4;
				add_new_object(Application::instance().m_game_object_manager.new_object(torch_kind[tk]), m_items[i][block->rect.x + 1]);
				tk = rand() % 4;
				add_new_object(Application::instance().m_game_object_manager.new_object(torch_kind[tk]), m_items[i][block->rect.x + block->rect.w - 1]);
				j = -1;
			}
			j = j + 1;
		}
	}
}