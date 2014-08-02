#include "GameMap.h"

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
			result=(*item)->find_property(kind);
			if(result!=nullptr)
			{
				return result;
			}
		}
	}
	return nullptr;
}

GameMap::GameMap(dimension_t size)
{
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
			obj = Application::instance().m_game_object_manager.new_object("Грязь");
			//add_object(obj, m_items[i][j]);
		}
	}
	for (int y = 0; y < 100; y++)
	{
		for (int x = 0; x< 100; x++)
		{
			m_coefficient[y * 100 + x] = sqrt(x*x+y*y);
		}
	}
}

void GameMap::add_object(GameObject* Object0,MapCell* Cell0)
{
	Object0->m_cell=Cell0;
	for(int i=0;i<Object0->m_size.y;i++)
	{
		for(int j=0;j<Object0->m_size.x;j++)
		{
			m_items[Cell0->y + i][Cell0->x - j]->add_object(Object0);
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
				GameObject* obj = Application::instance().m_game_object_manager.new_object("Стена");
				add_object(obj, m_items[i][j]);
			} else {
				m_items[i][j] = new MapCell(j, i);
				GameObject* obj = Application::instance().m_game_object_manager.new_object("Пол");
				add_object(obj, m_items[i][j]);
			}
		}
	}
}

void GameMap::move_object(GameObject* Obj,MapCell* Pos)
{
	for (int i = 0; i<Obj->m_size.y; i++)
	{
		for (int j = 0; j<Obj->m_size.x; j++)
		{
			m_items[Obj->m_cell->y + i][Obj->m_cell->x - j]->m_items.remove(Obj);
		}
	}
	add_object(Obj,Pos);
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
					GameObject* obj = Application::instance().m_game_object_manager.new_object("Стена");
					add_object(obj, m_items[i][j]);*/
				}
				else {
					m_items[i][j] = new MapCell(j, i);
					GameObject* obj = Application::instance().m_game_object_manager.new_object("Пол");
					add_object(obj, m_items[i][j]);
				}
			}
		}
		add_new_object(Application::instance().m_game_object_manager.new_object("Факел"), m_items[block->rect.y + 1][block->rect.x + 1]);
		add_new_object(Application::instance().m_game_object_manager.new_object("Факел"), m_items[block->rect.y + 1][block->rect.x + block->rect.w - 1]);
		add_new_object(Application::instance().m_game_object_manager.new_object("Факел"), m_items[block->rect.y + block->rect.h - 1][block->rect.x + block->rect.w - 1]);
		add_new_object(Application::instance().m_game_object_manager.new_object("Факел"), m_items[block->rect.y + block->rect.h - 1][block->rect.x + 1]);
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
							if (m_items[y + i][x + j]->m_items.front()->m_name == "Пол")
								empty = false;
						}
					}
				}
				if (empty)
				{
					add_object(Application::instance().m_game_object_manager.new_object("Грязь"), m_items[y][x]);
				}
				else
				{
					add_object(Application::instance().m_game_object_manager.new_object("Стена"), m_items[y][x]);
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
	for (int i = s; i < e+3; i++)
	{
		for (int j = -2; j < 3; j++)
		{
			m_items[ac.y+j][i]->m_items.clear();
			add_object(Application::instance().m_game_object_manager.new_object("Пол"), m_items[ac.y+j][i]);
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
	for (int i = s; i < e+3; i++)
	{
		for (int j = -2; j < 3; j++)
		{
			m_items[i][bc.x+j]->m_items.clear();
			add_object(Application::instance().m_game_object_manager.new_object("Пол"), m_items[i][bc.x + j]);
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
			if (m_items[rect.y][i]->m_items.front()->m_name == "Пол") 
			{ 
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (m_items[rect.y][i]->m_items.front()->m_name == "Стена") 
			{
				if (b)
				{
					if (i - s == 5)
					{
						add_object(Application::instance().m_game_object_manager.new_object("Дверь"), m_items[rect.y][i-1]);
					}
				}
				b = false;
			}
		}
		s = rect.x;
		b = false;
		for (int i = rect.x; i < rect.x + rect.w + 1; i++)
		{
			if (m_items[rect.y+rect.h][i]->m_items.front()->m_name == "Пол")
			{
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (m_items[rect.y + rect.h][i]->m_items.front()->m_name == "Стена")
			{
				if (b)
				{
					if (i - s == 5)
					{
						add_object(Application::instance().m_game_object_manager.new_object("Дверь"), m_items[rect.y + rect.h][i - 1]);
					}
				}
				b = false;
			}
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
		for (int x = 0; x< m_size.w; x++)
		{
			m_items[y][x]->m_light = light_t(25,25,25);
		}
	}
	for (std::list<GameObject*>::iterator Current = m_lights.begin(); Current != m_lights.end(); Current++)
	{
		for (int y = (*Current)->m_cell->y - 99; y < (*Current)->m_cell->y + 100; y++)
		{
			if (!((y<0)||(y>m_size.h-1)))
			{
				for (int x = (*Current)->m_cell->x - 99; x < (*Current)->m_cell->x + 100; x++)
				{
					if (!((x<0) || (x>m_size.w - 1)))
					{
						lx = abs(x - (*Current)->m_cell->x);
						ly = abs(y - (*Current)->m_cell->y);
						c = (*Current)->m_light->R - m_coefficient[ly * 100 + lx];
						if (c < 0){ c = 0; }
						m_items[y][x]->m_light.R += c;
						c = (*Current)->m_light->G - m_coefficient[ly * 100 + lx];
						if (c < 0){ c = 0; }
						m_items[y][x]->m_light.G += c;
						c = (*Current)->m_light->B - m_coefficient[ly * 100 + lx];
						if (c < 0){ c = 0; }
						m_items[y][x]->m_light.B += c;
					}
				}
			}
		}
	}
}