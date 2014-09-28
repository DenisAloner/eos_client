#include "AI.h"
#include "log.h"

min_heap::min_heap(void)
{
}

void min_heap::push(Node* value)
{
	unsigned int i, parent;
	m_items.push_back(value);
	i = m_items.size() - 1;
	parent = (i - 1) >> 1;
	int temp;
	while (parent >= 0 && i > 0)
	{
		if (m_items[i]->f < m_items[parent]->f)
		{
			std::swap(m_items[i], m_items[parent]);
		}
		i = parent;
		parent = (i - 1) >> 1;
	}
}

Node* min_heap::pop(){
	Node* n = m_items[0];
	m_items[0] = m_items[m_items.size() - 1];
	m_items.pop_back();
	sort_down(0);
	return n;
}

void min_heap::sort_down(unsigned int index)
{
	int left, right;
	int temp;
	left = 2 * index + 1;
	right = 2 * index + 2;
	if (left < m_items.size())
	{
		if (m_items[index]->f > m_items[left]->f)
		{
			std::swap(m_items[index], m_items[left]);
			sort_down(left);
		}
	}
	if (right <m_items.size())
	{
		if (m_items[index]->f > m_items[right]->f)
		{
			std::swap(m_items[index], m_items[right]);
			sort_down(right);
		}
	}
}

void min_heap::sort_up(unsigned int index)
{
	unsigned int parent = (index - 1) >> 1;
	int temp;
	while (parent >= 0 && index > 0)
	{
		if (m_items[index]->f < m_items[parent]->f)
		{
			std::swap(m_items[index], m_items[parent]);
		}
		index = parent;
		parent = (index - 1) >> 1;
	}
}

void min_heap::edit(unsigned int index, int value)
{
	if (m_items[index]->f != value)
	{
		if (m_items[index]->f < value)
		{
			m_items[index]->f = value;
			sort_down(index);
		}
		else {
			m_items[index]->f = value;
			sort_up(index);
		}
	}
}

Path::Path()
{

}

void Path::calculate(GameMap* map, GameObject* object, GameObject* goal, int radius)
{
	for (int y = m_middle - radius; y < m_middle + radius + 1; y++)
	{
		for (int x = m_middle - radius; x < m_middle + radius + 1; x++)
		{
			m_map[y][x].closed = false;
			m_map[y][x].state = 1;
			m_map[y][x].pos.y = y;
			m_map[y][x].pos.x = x;
		}
	}
	Path_cell* pc;
	MapCell* mc;
	MapCell* c2;
	m_game_map = map;
	m_unit = object;
	m_start_cell = &m_map[m_middle][m_middle];
	m_start_size = object->m_active_state->m_size;
	mc = static_cast<MapCell*>(object->m_owner);
	c2 = static_cast<MapCell*>(goal->m_owner);
	m_goal_cell = &m_map[c2->y - mc->y + m_middle][c2->x - mc->x +m_middle];
	m_goal_size = goal->m_active_state->m_size;
	std::function<bool(GameObject*)> qualifier = object->m_active_state->m_ai->m_path_qualifier;
	for (int y = -radius; y < radius + 1; y++)
	{
		if ((object->cell()->y + y >= 0) && (object->cell()->y + y < map->m_size.h))
		{
			for (int x = -radius; x < radius + 1; x++)
			{
				if ((object->cell()->x + x >= 0) && (object->cell()->x + x < map->m_size.w))
				{
					pc = &m_map[m_middle + y][m_middle + x];
					mc = map->m_items[object->cell()->y + y][object->cell()->x + x];
					pc->state = 0;
					for (std::list<GameObject*>::iterator item = mc->m_items.begin(); item != mc->m_items.end(); item++)
					{
						if (((*item) != object) && ((*item) != goal) && qualifier((*item)))
						{
							pc->state = 1;
							break;
						}
					}
				}
			}
		}
	}
}


int Path::manhattan(Path_cell* a, Path_cell* b) {
	return abs(a->pos.x - b->pos.x) + abs(a->pos.y - b->pos.y);
}

int Path::is_in_open(Path_cell* c)
{
	for (int i = 0; i < m_heap.m_items.size();i++)
	{
		if (m_heap.m_items[i]->cell == c)
		{
			return i;
		}
	}
	return -1;
}

void Path::insert_into_open(int x, int y, int dg, Node* p)
{
	for (int i = 0; i < m_start_size.y; i++)
	{
		for (int j = 0; j < m_start_size.x; j++)
		{
			if (x - j >= 0 && y + i >= 0 && x - j < m_max_size&&y + i < m_max_size)
			{
				if (m_map[y + i][x - j].state != 0)
				{
					return;
				}
			}
			else return;
		}
	}
	Path_cell* c = &m_map[y][x];
	if (!c->closed)
	{
		int n = is_in_open(c);
		if (n == -1)
		{
			m_heap.push(new Node(c, p->g + dg, manhattan(c, m_goal_cell) * 10, p));
		}
		else {
			if (p->g + dg < m_heap.m_items[n]->g){
				m_heap.m_items[n]->parent = p;
				m_heap.m_items[n]->g = p->g + dg;
				m_heap.m_items[n]->h = manhattan(m_heap.m_items[n]->cell, m_goal_cell) * 10;
				m_heap.edit(n, m_heap.m_items[n]->g + m_heap.m_items[n]->h);
			}
		}
	}
}

std::vector<MapCell*>* Path::back(Node* c)
{
	MapCell* cell = static_cast<MapCell*>(m_unit->m_owner);
	std::vector<MapCell*>* result = new std::vector<MapCell*>();
	Node* current=c;
	while (current)
	{
		result->push_back(m_game_map->m_items[cell->y + current->cell->pos.y - m_middle][cell->x + current->cell->pos.x - m_middle]);
		current = current->parent;
	}
	return result;
}

std::vector<MapCell*>* Path::get_path(){
	Node* current;
	m_heap.push(new Node(m_start_cell, 0, manhattan(m_start_cell, m_goal_cell) * 10));
	LOG(INFO) << m_start_cell->pos.x << " " << m_start_cell->pos.y;
	LOG(INFO) << m_goal_cell->pos.x << " " << m_goal_cell->pos.y;
	while (!m_heap.m_items.empty())
	{
		current = m_heap.pop();
		if (Game_algorithm::check_distance(&current->cell->pos,m_start_size,&m_goal_cell->pos,m_goal_size))
		{
			LOG(INFO) << "New step";
			return back(current);
		}
		current->cell->closed = true;
		insert_into_open(current->cell->pos.x + 1, current->cell->pos.y, 10, current);
		insert_into_open(current->cell->pos.x - 1, current->cell->pos.y, 10, current);
		insert_into_open(current->cell->pos.x, current->cell->pos.y + 1, 10, current);
		insert_into_open(current->cell->pos.x, current->cell->pos.y - 1, 10, current);
		insert_into_open(current->cell->pos.x + 1, current->cell->pos.y + 1, 14, current);
		insert_into_open(current->cell->pos.x + 1, current->cell->pos.y - 1, 14, current);
		insert_into_open(current->cell->pos.x - 1, current->cell->pos.y + 1, 14, current);
		insert_into_open(current->cell->pos.x - 1, current->cell->pos.y - 1, 14, current);
	}
	return nullptr;
}

AI::AI()
{
	m_fov = new FOV();
}


AI::~AI()
{
}

GameObject* AI::find_goal()
{
	m_fov->calculate(m_object->m_active_state->m_ai->m_fov_radius, m_object, m_map);
	FOV::cell* fc;
	MapCell* mc;
	for (int y = -m_fov->m_radius; y < m_fov->m_radius + 1; y++)
	{
		if ((m_object->cell()->y + y >= 0) && (m_object->cell()->y + y < m_map->m_size.h))
		{
			for (int x = -m_fov->m_radius; x < m_fov->m_radius + 1; x++)
			{
				if ((m_object->cell()->x + x >= 0) && (m_object->cell()->x + x < m_map->m_size.w))
				{
					fc = &m_fov->m_map[m_fov->m_middle + y][m_fov->m_middle + x];
					if ((!fc->opaque) && fc->visible)
					{
						mc = m_map->m_items[m_object->cell()->y + y][m_object->cell()->x + x];
						for (std::list<GameObject*>::iterator item = mc->m_items.begin(); item != mc->m_items.end(); item++)
						{
							if ((*item) == Application::instance().m_GUI->MapViewer->m_player->m_object)
							{
								return Application::instance().m_GUI->MapViewer->m_player->m_object;
							}
						}
					}
				}
			}
		}
	}
	return nullptr;
}

void AI::create()
{

		GameObject* goal = find_goal();
		if (goal == nullptr) {
			return;
		}
		else
		{
			LOG(INFO) << "VISIBLE";
		}
		if (Game_algorithm::check_distance(static_cast<MapCell*>(m_object->m_owner), m_object->m_active_state->m_size, static_cast<MapCell*>(goal->m_owner), goal->m_active_state->m_size)){
			return;
		}
		Path::instance().calculate(m_map, m_object, goal, m_object->m_active_state->m_ai->m_fov_radius);
		std::vector<MapCell*>* path;
		path = Path::instance().get_path();
		if (path)
		{
			Parameter_Position* P;
			P = new Parameter_Position();
			P->m_object = m_object;
			P->m_place = (*path)[path->size() - 2];
			P->m_map = m_map;
			Application::instance().m_action_manager->add(m_object, new GameTask(Application::instance().m_actions[action_e::move], P));
		}
		Path::instance().m_heap.m_items.clear();
}