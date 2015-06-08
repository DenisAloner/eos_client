#include "game/GameMap.h"
#include "game/GameObject.h"
#include "game/Application.h"
#include "log.h"

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
			m_items[i][j] = new MapCell(j, i,this);
			obj = Application::instance().m_game_object_manager->new_object("dirt");
			//add_object(obj, m_items[i][j]);
		}
	}
	for (int y = 0; y < 21; y++)
	{
		for (int x = 0; x< 21; x++)
		{
			m_coefficient[y][x] = (float)sqrt(x*x + y*y)*5;
		}
	}
}

//void GameMap::generate_room(void)
//{
//	for(int i=0;i<m_size.h;i++)
//	{
//		for(int j=0;j<m_size.w;j++)
//		{
//			if ((i==0)||(j==0)||(i==m_size.h-1)||(j==m_size.w-1))
//			{
//				m_items[i][j] = new MapCell(j, i,this);
//				GameObject* obj = Application::instance().m_game_object_manager->new_object("wall");
//				add_object(obj, m_items[i][j]);
//			} else {
//				m_items[i][j] = new MapCell(j, i,this);
//				GameObject* obj = Application::instance().m_game_object_manager->new_object("floor");
//				add_object(obj, m_items[i][j]);
//			}
//		}
//	}
//}

void GameMap::generate_room(void)
{
	for (int i = 0; i<m_size.h; i++)
	{
		for (int j = 0; j<m_size.w; j++)
		{
			if ((i == 0) || (j == 0) || (i == m_size.h - 1) || (j == m_size.w - 1))
			{
				m_items[i][j] = new MapCell(j, i, this);
				GameObject* obj = Application::instance().m_game_object_manager->new_object("iso_floor");
				add_object(obj, m_items[i][j]);
			}
			else {
				m_items[i][j] = new MapCell(j, i, this);
				GameObject* obj = Application::instance().m_game_object_manager->new_object("iso_floor");
				add_object(obj, m_items[i][j]);
			}
		}
	}
}



void GameMap::add_object(GameObject* object, MapCell* cell)
{
	object->m_owner = cell;
	for (int i = 0; i<object->m_active_state->m_size.y; i++)
	{
		for (int j = 0; j<object->m_active_state->m_size.x; j++)
		{
			m_items[cell->y + i][cell->x + j]->add_object(object);
		}
	}
}

void GameMap::remove_object(GameObject* object)
{
	for (int i = 0; i<object->m_active_state->m_size.y; i++)
	{
		for (int j = 0; j<object->m_active_state->m_size.x; j++)
		{
			m_items[object->cell()->y + i][object->cell()->x + j]->m_items.remove(object);
		}
	}
	object->m_owner = nullptr;
}

void GameMap::move_object(GameObject* object,MapCell* cell)
{
	remove_object(object);
	add_object(object, cell);
}

void  GameMap::turn_object(GameObject* object)
{
	MapCell* position = object->cell();
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

void GameMap::add_ai_object(GameObject* Object, MapCell* Element)
{
	add_object(Object, Element);
	m_ai.push_front(Object);
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
					m_items[i][j] = new MapCell(j, i, this);
					GameObject* obj = Application::instance().m_game_object_manager->new_object("floor");
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
					add_object(Application::instance().m_game_object_manager->new_object("dirt"), m_items[y][x]);
				}
				else
				{
					add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[y][x]);
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
			add_object(Application::instance().m_game_object_manager->new_object("floor"), m_items[ac.y+j][i]);
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
			add_object(Application::instance().m_game_object_manager->new_object("floor"), m_items[i][bc.x + j]);
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
						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[rect.y][i - 1]);
						add_object(Application::instance().m_game_object_manager->new_object("door"), m_items[rect.y][i - 2]);
						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[rect.y][i - 7]);
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
						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[rect.y + rect.h][i - 1]);
						add_object(Application::instance().m_game_object_manager->new_object("door"), m_items[rect.y + rect.h][i - 2]);
						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[rect.y + rect.h][i - 7]);
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
						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[i - 1][rect.x]);
						GameObject* obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(ObjectDirection_Left);
						add_object(obj, m_items[i-6][rect.x]);
						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[i - 7][rect.x]);
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
						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[i - 1][rect.x + rect.w]);
						GameObject* obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(ObjectDirection_Left);
						add_object(obj, m_items[i - 6][rect.x + rect.w]);
						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[i - 7][rect.x + rect.w]);
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


//bool GameMap::line2(int x1, int y1, int x2, int y2)
//{
//	int e = 0;
//	int dx = abs(x2 - x1);
//	int dy = abs(y2 - y1);
//	int x;
//	int y;
//	int k;
//	if (dx >= dy)
//	{
//		y = y1;
//		k = (y1 < y2 ? 1 : -1);
//		if (x1 < x2)
//		{
//			for (x = x1; x < x2+1; x++)
//			{
//				if (m_barrier_map[y][x]){ return false; }
//				e = e + dy;
//				if (2 * e >= dx)
//				{
//					y = y + k;
//					e = e - dx;
//				}
//			}
//		}
//		else
//		{
//			for (x = x1; x > x2-1; x--)
//			{
//				if (m_barrier_map[y][x]){ return false; }
//				e = e + dy;
//				if (2 * e >= dx)
//				{
//					y = y + k;
//					e = e - dx;
//				}
//			}
//		}
//	}
//	else
//	{
//		x = x1;
//		k = (x1 < x2 ? 1 : -1);
//		if (y1 <= y2)
//		{
//			for (y = y1; y < y2+1; y++)
//			{
//				if (m_barrier_map[y][x]){ return false; }
//				e = e + dx;
//				if (2 * e >= dy)
//				{
//					x = x + k;
//					e = e - dy;
//				}
//			}
//		}
//		else
//		{
//
//			for (y = y1; y > y2-1; y--)
//			{
//				if (m_barrier_map[y][x]){ return false; }
//				e = e + dx;
//				if (2 * e >= dy)
//				{
//					x = x + k;
//					e = e - dy;
//				}
//			}
//		}
//	}
//	return true;
//}

void GameMap::bresenham_line(MapCell* a, MapCell* b, std::function<void(MapCell*)> f)
{
	int e = 0;
	int dx = abs(b->x - a->x);
	int dy = abs(b->y - a->y);
	int x;
	int y;
	int k;
	if (dx >= dy)
	{
		y = a->y;
		k = (a->y < b->y ? 1 : -1);
		if (a->x < b->x)
		{
			for (x = a->x; x < b->x + 1; x++)
			{
				f(m_items[y][x]);
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
			for (x = a->x; x > b->x - 1; x--)
			{
				f(m_items[y][x]);
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
		x = a->x;
		k = (a->x < b->x ? 1 : -1);
		if (a->y < b->y)
		{
			for (y = a->y; y < b->y + 1; y++)
			{
				f(m_items[y][x]);
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
			for (y = a->y; y > b->y - 1; y--)
			{
				f(m_items[y][x]);
				e = e + dx;
				if (2 * e >= dy)
				{
					x = x + k;
					e = e - dy;
				}
			}
		}
	}
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
		add_new_object(Application::instance().m_game_object_manager->new_object(torch_kind[tk]), m_items[block->rect.y + 1][block->rect.x + 1]);
		tk = rand() % 1;
		add_new_object(Application::instance().m_game_object_manager->new_object(torch_kind[tk]), m_items[block->rect.y + 1][block->rect.x + block->rect.w - 1]);
		tk = rand() % 1;
		add_new_object(Application::instance().m_game_object_manager->new_object(torch_kind[tk]), m_items[block->rect.y + block->rect.h - 1][block->rect.x + block->rect.w - 1]);
		tk = rand() % 1;
		add_new_object(Application::instance().m_game_object_manager->new_object(torch_kind[tk]), m_items[block->rect.y + block->rect.h - 1][block->rect.x + 1]);
		int j=0;
		for (int i = block->rect.x + 1; i < block->rect.x + block->rect.w; i++)
		{
			if (j == 30)
			{
				tk = rand() % 4;
				add_new_object(Application::instance().m_game_object_manager->new_object(torch_kind[tk]), m_items[block->rect.y + 1][i]);
				tk = rand() % 4;
				add_new_object(Application::instance().m_game_object_manager->new_object(torch_kind[tk]), m_items[block->rect.y + block->rect.h - 1][i]);
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
				add_new_object(Application::instance().m_game_object_manager->new_object(torch_kind[tk]), m_items[i][block->rect.x + 1]);
				tk = rand() % 4;
				add_new_object(Application::instance().m_game_object_manager->new_object(torch_kind[tk]), m_items[i][block->rect.x + block->rect.w - 1]);
				j = -1;
			}
			j = j + 1;
		}
	}
}

bool GameMap::check(int x, int y)
{
	if (x<0 && x>m_size.h - 1 && y<0 && y>m_size.w - 1) { return false; };
}

//MapCell* GameMap::get_cell(int x, int y)
//{
//	if (x<0 && x>m_size.h - 1 && y<0 && y>m_size.w - 1) { return false; };
//}

void GameMap::calculate_lighting2()
{
	int lx;
	int ly;
	int c;
	FOV_light fl;
	light_t m_temp[41][41];
	for (int y = 0; y < m_size.h; y++)
	{
		for (int x = 0; x < m_size.w; x++)
		{
			m_items[y][x]->m_light = light_t();
		}
	}
	for (std::list<GameObject*>::iterator l = m_lights.begin(); l != m_lights.end(); l++)
	{
		fl.calculate(20, *l, this);
		for (int y = 0; y < 41; y++)
		{
			for (int x = 0; x < 41; x++)
			{
				lx = abs(x - 20);
				ly = abs(y - 20);
				c = ((*l)->m_active_state->m_light->R - m_coefficient[ly][lx])*fl.m_map[y][x].damping.R;
				if (c < 0){ c = 0; }
				fl.m_map[y][x].light.R = c;
				c = ((*l)->m_active_state->m_light->G - m_coefficient[ly][lx])*fl.m_map[y][x].damping.G;
				if (c < 0){ c = 0; }
				fl.m_map[y][x].light.G = c;
				c = ((*l)->m_active_state->m_light->B - m_coefficient[ly][lx])*fl.m_map[y][x].damping.B;
				if (c < 0){ c = 0; }
				fl.m_map[y][x].light.B = c;
			}
		}

		
			light_t l1;
			float Gauss[] = { 0.44198F, 0.27901F };
			//float Gauss[] = { 0.39894228F, 0.241970725F, 0.053990967F };
			//float Gauss[] = { 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 };
			for (int y = 0; y < 41; y++)
			{
				for (int x = 0; x < 41; x++)
				{
					l1 = light_t();
					for (int i = -1; i < 2; i++)
					{
						int mx = x + i;
						int my = y;
						if (mx < 0){ mx = 0; }
						if (mx > 40){ mx = 40; }
						if (my < 0){ my = 0; }
						if (my > 40){ my = 40; }
						l1.R = l1.R + fl.m_map[my][mx].light.R*Gauss[abs(i)];
						l1.G = l1.G + fl.m_map[my][mx].light.G*Gauss[abs(i)];
						l1.B = l1.B + fl.m_map[my][mx].light.B*Gauss[abs(i)];
					}
					m_temp[y][x] = l1;
				}
			}
			for (int y = 0; y < 41; y++)
			{
				for (int x = 0; x < 41; x++)
				{
					l1 = light_t();
					for (int i = -1; i < 2; i++)
					{
						int mx = x;
						int my = y + i;
						if (mx < 0){ mx = 0; }
						if (mx > 40){ mx = 40; }
						if (my < 0){ my = 0; }
						if (my > 40){ my = 40; }
						l1.R = l1.R + m_temp[my][mx].R*Gauss[abs(i)];
						l1.G = l1.G + m_temp[my][mx].G*Gauss[abs(i)];
						l1.B = l1.B + m_temp[my][mx].B*Gauss[abs(i)];
					}
					fl.m_map[y][x].light = l1;
				}
			}

		for (int y = 0; y < 41; y++)
		{
			ly = (*l)->cell()->y + y - 20;
			if (!((ly<0) || (ly>m_size.h - 1)))
			{
				for (int x = 0; x < 41; x++)
				{
					lx = (*l)->cell()->x + x - 20;
					if (!((lx<0) || (lx>m_size.w - 1)))
					{
						m_items[ly][lx]->m_light.R += fl.m_map[y][x].light.R;
						m_items[ly][lx]->m_light.G += fl.m_map[y][x].light.G;
						m_items[ly][lx]->m_light.B += fl.m_map[y][x].light.B;
					}
				}
			}
		}
	}
}