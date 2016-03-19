#include "AI.h"
#include "log.h"
#include "GameObject.h"

min_heap::min_heap(void)
{
}

void min_heap::push(Node* value)
{
	std::size_t index, parent;
	m_items.push_back(value);
	index = m_items.size() - 1;
	while (index > 0)
	{
		parent = (index - 1) >> 1;
		if (m_items[index]->f < m_items[parent]->f)
		{
			std::swap(m_items[index], m_items[parent]);
		}
		index = parent;
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
	int size = m_items.size();
	left = 2 * index + 1;
	right = 2 * index + 2;
	if (left < size)
	{
		if (m_items[index]->f > m_items[left]->f)
		{
			std::swap(m_items[index], m_items[left]);
			sort_down(left);
		}
	}
	if (right <size)
	{
		if (m_items[index]->f > m_items[right]->f)
		{
			std::swap(m_items[index], m_items[right]);
			sort_down(right);
		}
	}
}

void min_heap::sort_up(std::size_t index)
{
	std::size_t parent;
	while (index > 0)
	{
		parent = (index - 1) >> 1;
		if (m_items[index]->f < m_items[parent]->f)
		{
			std::swap(m_items[index], m_items[parent]);
		}
		index = parent;
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

void Path::calculate(GameMap* map, GameObject* object, MapCell* gc,GameObject* goal, int radius)
{
	/*int left_limit = (object->cell()->x - radius < 0) ? 0 : object->cell()->x - radius;
	int right_limit = (object->cell()->x + radius + 1 > map->m_size.w) ? map->m_size.w : object->cell()->y + radius + 1;
	int bottom_limit = (object->cell()->y - radius < 0) ? 0 : object->cell()->y - radius;
	int top_limit = (object->cell()->y + radius + 1 > map->m_size.h) ? map->m_size.h : object->cell()->y + radius + 1;*/
	m_open = 0;
	MapCell* c;
	for (int y = 0; y < map->m_size.h; y++)
	{
		for (int x = 0; x < map->m_size.w; x++)
		{
			c = map->m_items[y][x];
			c->m_closed = false;
		}
	}
	MapCell* pc;
	m_game_map = map;
	m_unit = object;
	m_start_cell = object->cell();
	m_start_size = object->m_active_state->m_size;
	m_goal_cell = gc;
	m_goal_size = goal->m_active_state->m_size;
	std::function<bool(GameObject*)> qualifier = static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_path_qualifier->predicat;
	for (int y = 0; y < map->m_size.h; y++)
	{
		for (int x = 0; x < map->m_size.w; x++)
		{
			c = map->m_items[y][x];
			c->m_closed = false;
			c->m_state = 0;
			c->m_mark = false;
			for (std::list<GameObject*>::iterator item =c->m_items.begin(); item != c->m_items.end(); item++)
			{
				if (((*item) != object) && ((*item) != goal) && qualifier((*item)))
				{
					c->m_state = 1;
					break;
				}
			}
		}
	}
}

void Path::map_costing(GameMap* map, GameObject* object, MapCell* gc, int radius)
{
	/*int left_limit = (object->cell()->x - radius < 0) ? 0 : object->cell()->x - radius;
	int right_limit = (object->cell()->x + radius + 1 > map->m_size.w) ? map->m_size.w : object->cell()->y + radius + 1;
	int bottom_limit = (object->cell()->y - radius < 0) ? 0 : object->cell()->y - radius;
	int top_limit = (object->cell()->y + radius + 1 > map->m_size.h) ? map->m_size.h : object->cell()->y + radius + 1;*/
	MapCell* c;
	for (int y = 0; y < map->m_size.h; y++)
	{
		for (int x = 0; x < map->m_size.w; x++)
		{
			c = map->m_items[y][x];
			c->m_closed = false;
		}
	}
	MapCell* pc;
	m_game_map = map;
	m_unit = object;
	m_start_cell = object->cell();
	m_start_size = object->m_active_state->m_size;
	m_goal_cell = gc;
	std::function<bool(GameObject*)> qualifier = static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_path_qualifier->predicat;
	for (int y = 0; y < map->m_size.h; y++)
	{
		for (int x = 0; x < map->m_size.w; x++)
		{
			c = map->m_items[y][x];
			c->m_closed = false;
			c->m_state = 0;
			for (std::list<GameObject*>::iterator item = c->m_items.begin(); item != c->m_items.end(); item++)
			{
				if (((*item) != object) && qualifier((*item)))
				{
					c->m_state = 1;
					break;
				}
			}
		}
	}
}

//int Path::manhattan(MapCell* a, MapCell* b) {
//	return abs(a->x - b->x) + abs(a->y - b->y);
//}

int Path::manhattan(MapCell* a, MapCell* b, MapCell* c)
{
	return (abs(a->x - b->x) + abs(a->y - b->y)) * 10;

	/*int dx = abs(a->x - b->x);
	int dy = abs(a->y - b->y);

	return (dx < dy) ? 4 * dx + 10 * dy : 4 * dy + 10 * dx;*/

	/*return (dx > dy) ? 14 * dy + 10 * (dx - dy) : 14 * dx + 10 * (dy - dx);*/

	//int xdist = a->x - b->x;
	//int	ydist = a->y - b->y;
	//float sqrt2 = 1.41421356; //(this should be precomputed and stored as a define somewhere)
	//float	h = min(xdist, ydist) * sqrt2 + max(xdist, ydist) - min(xdist, ydist);
	//return h * 10;

	//int h;
	//int xDistance = abs(a->x - b->x);
	//int yDistance = abs(a->y - b->y);
	//if (xDistance > yDistance)
	//{
	//	h = 14 * yDistance + 10 * (xDistance - yDistance);
	//}	
	//else
	//{
	//	h = 14 * xDistance + 10 * (yDistance - xDistance);
	//}
	//return h;

	//return (h + abs((a->x - b->x)*(c->y - b->y) - (c->x - b->x)*(a->y - b->y))*0.001)*10;
}

int Path::is_in_open(MapCell* c)
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
			if (x + j >= 0 && y - i >= 0 && x + j < m_game_map->m_size.w&&y - i <m_game_map->m_size.h)
			{
				if (m_game_map->m_items[y - i][x + j]->m_state != 0)
				{
					return;
				}
			}
			else return;
		}
	}
	MapCell* c = m_game_map->m_items[y][x];
	if (!c->m_closed)
	{
		m_open += 1;
		m_game_map->m_items[y][x]->m_mark = true;
		int n = is_in_open(c);
		if (n == -1)
		{
			m_heap.push(new Node(c, p->g + dg, manhattan(c, m_goal_cell, m_start_cell), p));
		}
		else {
			if (p->g + dg < m_heap.m_items[n]->g){
				m_heap.m_items[n]->parent = p;
				m_heap.m_items[n]->g = p->g + dg;
				m_heap.m_items[n]->h = manhattan(m_heap.m_items[n]->cell, m_goal_cell, m_start_cell);
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
		result->push_back(current->cell);
		current = current->parent;
	}
	return result;
}

std::vector<MapCell*>* Path::get_path(){
	Node* current;
	m_heap.push(new Node(m_start_cell, 0, manhattan(m_start_cell, m_goal_cell, m_start_cell)));
	while (!m_heap.m_items.empty())
	{
		current = m_heap.pop();
		if (Game_algorithm::check_distance(current->cell,m_start_size,m_goal_cell,m_goal_size))
		{
			return back(current);
		}
		current->cell->m_closed = true;
		insert_into_open(current->cell->x + 1, current->cell->y, 10, current);
		insert_into_open(current->cell->x - 1, current->cell->y, 10, current);
		insert_into_open(current->cell->x, current->cell->y + 1, 10, current);
		insert_into_open(current->cell->x, current->cell->y - 1, 10, current);
		insert_into_open(current->cell->x + 1, current->cell->y + 1, 14, current);
		insert_into_open(current->cell->x + 1, current->cell->y - 1, 14, current);
		insert_into_open(current->cell->x - 1, current->cell->y + 1, 14, current);
		insert_into_open(current->cell->x - 1, current->cell->y - 1, 14, current);
	}
	return nullptr;
}

std::vector<MapCell*>* Path::get_path_to_cell() {
	Node* current;
	m_heap.push(new Node(m_start_cell, 0, manhattan(m_start_cell, m_goal_cell, m_start_cell)));
	while (!m_heap.m_items.empty())
	{
		current = m_heap.pop();
		if ((current->cell->x== m_goal_cell->x)&& (current->cell->y == m_goal_cell->y))
		{
			return back(current);
		}
		current->cell->m_closed = true;
		insert_into_open(current->cell->x + 1, current->cell->y, 10, current);
		insert_into_open(current->cell->x - 1, current->cell->y, 10, current);
		insert_into_open(current->cell->x, current->cell->y + 1, 10, current);
		insert_into_open(current->cell->x, current->cell->y - 1, 10, current);
		insert_into_open(current->cell->x + 1, current->cell->y + 1, 14, current);
		insert_into_open(current->cell->x + 1, current->cell->y - 1, 14, current);
		insert_into_open(current->cell->x - 1, current->cell->y + 1, 14, current);
		insert_into_open(current->cell->x - 1, current->cell->y - 1, 14, current);
	}
	return nullptr;
}

Dijkstra_map::Dijkstra_map()
{
}

bool Dijkstra_map::opaque_check(int x, int y, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (y - i < 0) { return false; }
		for (int j = 0; j < size; j++)
		{
			if (x + j > m_map->m_size.w - 1) { return false; }
			if (m_map->m_items[y - i][x + j]->m_pathfind_info.opaque) { return false; }
		}
	}
	return true;
}

void Dijkstra_map::calculate_cost(GameMap* map, GameObject* object, GameObject* goal)
{
	m_map = map;
	MapCell* c;
	std::function<bool(GameObject*)> qualifier = static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_path_qualifier->predicat;
	for (int y = 0; y < m_map->m_size.h; y++)
	{
		for (int x = 0; x < m_map->m_size.w; x++)
		{
			c = m_map->m_items[y][x];
			c->m_pathfind_info.opaque = false;
			c->m_pathfind_info.value = 100000;
			for (std::list<GameObject*>::iterator item = c->m_items.begin(); item != c->m_items.end(); item++)
			{
				if ((*item) == goal)
				{
					c->m_pathfind_info.opaque = false;
					c->m_pathfind_info.value = 0;
				}
				else
				{
					if (((*item) != object) && qualifier((*item)))
					{
						c->m_pathfind_info.opaque = true;
						break;
					}
				}
			}
		}
	}
}

void Dijkstra_map::calculate_cost_autoexplore(GameMap* map, GameObject* object)
{
	m_map = map;
	MapCell* c;
	std::function<bool(GameObject*)> qualifier = static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_path_qualifier->predicat;
	for (int y = 0; y < m_map->m_size.h; y++)
	{
		for (int x = 0; x < m_map->m_size.w; x++)
		{
			c = m_map->m_items[y][x];
			if (!c->m_notable)
			{
				c->m_pathfind_info.opaque = false;
				c->m_pathfind_info.value = 0;
			}
			else
			{
				c->m_pathfind_info.opaque = false;
				c->m_pathfind_info.value = 100000;
				for (std::list<GameObject*>::iterator item = c->m_items.begin(); item != c->m_items.end(); item++)
				{
					if (((*item) != object) && qualifier((*item)))
					{
						c->m_pathfind_info.opaque = true;
						break;
					}

				}
			}
		}
	}
}

void Dijkstra_map::trace()
{
	bool was_change;
	int low;
	int i = 0;
	std::vector<std::vector<MapCell*> >& cells = m_map->m_items;
	do
	{
		was_change = false;
		for (int y = 1; y < m_map->m_size.h-1; y++)
		{
			for (int x = 1; x <  m_map->m_size.w - 1; x++)
			{
				if (opaque_check(x, y, 2))
				{
					low = cells[y - 1][x - 1]->m_pathfind_info.value;
					low = (cells[y - 1][x]->m_pathfind_info.value < low) ? cells[y - 1][x]->m_pathfind_info.value : low;
					low = (cells[y - 1][x + 1]->m_pathfind_info.value < low) ? cells[y - 1][x + 1]->m_pathfind_info.value : low;
					low = (cells[y][x - 1]->m_pathfind_info.value < low) ? cells[y][x - 1]->m_pathfind_info.value : low;
					low = (cells[y][x + 1]->m_pathfind_info.value < low) ? cells[y][x + 1]->m_pathfind_info.value : low;
					low = (cells[y + 1][x - 1]->m_pathfind_info.value < low) ? cells[y + 1][x - 1]->m_pathfind_info.value : low;
					low = (cells[y + 1][x]->m_pathfind_info.value < low) ? cells[y + 1][x]->m_pathfind_info.value : low;
					low = (cells[y + 1][x + 1]->m_pathfind_info.value < low) ? cells[y + 1][x + 1]->m_pathfind_info.value : low;
					if (cells[y][x]->m_pathfind_info.value > low + 1)
					{
						was_change = true;
						cells[y][x]->m_pathfind_info.value = low + 1;
					}
				}
			}
		}
		i += 1;
	} while (was_change);
	LOG(INFO) << std::to_string(i);
}

MapCell* Dijkstra_map::next(GameObject* object)
{
	MapCell* c = nullptr;
	int value = object->cell()->m_pathfind_info.value;
	int x = object->cell()->x;
	int y = object->cell()->y;
	if (m_map->m_items[y - 1][x - 1]->m_pathfind_info.value < value)
	{
		c = m_map->m_items[y - 1][x - 1];
		value = m_map->m_items[y - 1][x - 1]->m_pathfind_info.value;
	}
	if (m_map->m_items[y - 1][x]->m_pathfind_info.value < value)
	{
		c = m_map->m_items[y - 1][x];
		value = m_map->m_items[y - 1][x]->m_pathfind_info.value;
	}
	if (m_map->m_items[y - 1][x + 1]->m_pathfind_info.value < value)
	{
		c = m_map->m_items[y - 1][x + 1];
		value = m_map->m_items[y - 1][x + 1]->m_pathfind_info.value;
	}
	if (m_map->m_items[y][x - 1]->m_pathfind_info.value < value)
	{
		c = m_map->m_items[y][x - 1];
		value = m_map->m_items[y][x - 1]->m_pathfind_info.value;
	}
	if (m_map->m_items[y][x + 1]->m_pathfind_info.value < value)
	{
		c = m_map->m_items[y][x + 1];
		value = m_map->m_items[y][x + 1]->m_pathfind_info.value;
	}
	if (m_map->m_items[y + 1][x - 1]->m_pathfind_info.value < value)
	{
		c = m_map->m_items[y + 1][x - 1];
		value = m_map->m_items[y + 1][x - 1]->m_pathfind_info.value;
	}
	if (m_map->m_items[y + 1][x]->m_pathfind_info.value < value)
	{
		c = m_map->m_items[y + 1][x];
		value = m_map->m_items[y + 1][x]->m_pathfind_info.value;
	}
	if (m_map->m_items[y + 1][x + 1]->m_pathfind_info.value < value)
	{
		c = m_map->m_items[y + 1][x + 1];
		value = m_map->m_items[y + 1][x + 1]->m_pathfind_info.value;
	}
	return c;
}

AI_enemy::AI_enemy()
{
	m_ai_type = ai_type_e::non_humanoid;
	m_fov = new FOV();
	m_goal = nullptr;
	m_memory_goal_cell = nullptr;
	m_action_controller = new Action_wrapper();
}

AI* AI_enemy::clone()
{
	AI_enemy* c = new AI_enemy();
	c->m_ai_type = m_ai_type;
	//for (auto current = m_FOVs.begin(); current != m_FOVs.end(); current++)
	//{
	//	c->m_FOVs.push_back(AI_FOV(current->radius, current->qualifier, current->start_angle, current->end_angle));
	//}
	c->m_path_qualifier = m_path_qualifier;
	return c;
}

GameObject* AI_enemy::find_goal()
{
	FOV::cell* fc;
	MapCell* mc;
	Vision_list* vl = static_cast<Vision_list*>(m_object->m_active_state->get_list(interaction_e::vision));
	int radius = vl->m_max_radius;

	int xc = m_object->cell()->x;
	int yc = m_object->cell()->y;
	int dx = m_object->m_active_state->m_size.x;
	int dy = m_object->m_active_state->m_size.y;

	int xs = ((dx - 1) >> 1);
	int ys = ((dy - 1) >> 1);

	int x_start = xc - radius + xs;
	x_start = max(x_start, 0);
	x_start = min(x_start, m_map->m_size.w - 1);
	int x_end = xc + radius + xs;
	x_end = max(x_end, 0);
	x_end = min(x_end, m_map->m_size.w - 1);
	int y_start = yc - radius - ys;
	y_start = max(y_start, 0);
	y_start = min(y_start, m_map->m_size.h - 1);
	int y_end = yc + radius - ys;
	y_end = max(y_end, 0);
	y_end = min(y_end, m_map->m_size.h - 1);

	for (int y = y_start; y < y_end + 1; y++)
	{
		for (int x = x_start; x < x_end + 1; x++)
		{
			fc = &m_fov->m_map[y - y_start][x - x_start];
			if (fc->visible)
			{
				for (auto item = m_map->m_items[y][x]->m_items.begin(); item != m_map->m_items[y][x]->m_items.end(); item++)
				{
					if ((*item) == Application::instance().m_GUI->MapViewer->m_player->m_object)
					{
						return Application::instance().m_GUI->MapViewer->m_player->m_object;
					}
				}
			}
		}
	}
	return nullptr;
}

bool AI_enemy::check_goal()
{
	FOV::cell* fc;
	MapCell* mc;
	Vision_list* vl = static_cast<Vision_list*>(m_object->m_active_state->get_list(interaction_e::vision));
	int radius = vl->m_max_radius;

	int xc = m_object->cell()->x;
	int yc = m_object->cell()->y;
	int dx = m_object->m_active_state->m_size.x;
	int dy = m_object->m_active_state->m_size.y;

	int xs = ((dx - 1) >> 1);
	int ys = ((dy - 1) >> 1);

	int x_start = xc - radius + xs;
	x_start = max(x_start, 0);
	x_start = min(x_start, m_map->m_size.w - 1);
	int x_end = xc + radius + xs;
	x_end = max(x_end, 0);
	x_end = min(x_end, m_map->m_size.w - 1);
	int y_start = yc - radius - ys;
	y_start = max(y_start, 0);
	y_start = min(y_start, m_map->m_size.h - 1);
	int y_end = yc + radius - ys;
	y_end = max(y_end, 0);
	y_end = min(y_end, m_map->m_size.h - 1);

	for (int y = y_start; y < y_end + 1; y++)
	{
		for (int x = x_start; x < x_end + 1; x++)
		{
			fc = &m_fov->m_map[y - y_start][x - x_start];
			if (fc->visible)
			{
				for (auto item = m_map->m_items[y][x]->m_items.begin(); item != m_map->m_items[y][x]->m_items.end(); item++)
				{
					if ((*item) == m_goal)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

void AI_enemy::calculate_FOV(GameObject* object,GameMap* map)
{
	m_fov->calculate_FOV(object, map);
	update(VoidEventArgs());
}

void AI_enemy::create()
{
	if (m_object->m_active_state->m_ai == nullptr){ return; }
	if (!m_action_controller->m_action)
	{
		calculate_FOV(m_object,m_map);
		MapCell* c;
		GameObject* goal;
		if (m_goal)
		{
			if (!check_goal())
			{
				goal = find_goal();
				if (goal)
				{
					c = m_goal->cell();
					m_memory_goal_cell = c;
				}
				else c = m_memory_goal_cell;
			}
			else
			{
				c = m_goal->cell();
				m_memory_goal_cell = c;
			}
		}
		else
		{
			m_goal = find_goal();
			if (m_goal)
			{
				c = m_goal->cell();
				m_memory_goal_cell = c;
			}
			else return;
		}
		if (Game_algorithm::check_distance(static_cast<MapCell*>(m_object->m_owner), m_object->m_active_state->m_size, static_cast<MapCell*>(m_goal->m_owner), m_goal->m_active_state->m_size))
		{
			P_unit_interaction* p = new P_unit_interaction();
			p->m_unit = m_object;
			p->m_object = m_goal;
			p->m_unit_body_part = nullptr;
			//Application::instance().m_action_manager->add(p->m_unit, new GameTask(Application::instance().m_actions[action_e::hit], p));
			m_action_controller->set(p->m_unit,Application::instance().m_actions[action_e::hit], p);
		}
		else
		{
		/*	m_map->m_dijkstra_map->calculate(m_map, m_object, m_goal);
			MapCell* c = nullptr;
			int value = m_map->m_dijkstra_map->m_map[m_object->cell()->y][m_object->cell()->x].value;
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y - 1][m_object->cell()->x - 1].value < value)
			{
				c = m_map->m_items[m_object->cell()->y - 1][m_object->cell()->x - 1];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y - 1][m_object->cell()->x - 1].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y - 1][m_object->cell()->x].value < value)
			{
				c = m_map->m_items[m_object->cell()->y - 1][m_object->cell()->x];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y - 1][m_object->cell()->x].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y - 1][m_object->cell()->x + 1].value < value)
			{
				c = m_map->m_items[m_object->cell()->y - 1][m_object->cell()->x + 1];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y - 1][m_object->cell()->x + 1].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y][m_object->cell()->x - 1].value < value)
			{
				c = m_map->m_items[m_object->cell()->y][m_object->cell()->x - 1];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y][m_object->cell()->x - 1].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y][m_object->cell()->x + 1].value < value)
			{
				c = m_map->m_items[m_object->cell()->y][m_object->cell()->x + 1];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y][m_object->cell()->x + 1].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y + 1][m_object->cell()->x - 1].value < value)
			{
				c = m_map->m_items[m_object->cell()->y + 1][m_object->cell()->x - 1];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y + 1][m_object->cell()->x - 1].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y + 1][m_object->cell()->x].value < value)
			{
				c = m_map->m_items[m_object->cell()->y + 1][m_object->cell()->x];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y + 1][m_object->cell()->x].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y + 1][m_object->cell()->x + 1].value < value)
			{
				c = m_map->m_items[m_object->cell()->y + 1][m_object->cell()->x + 1];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y + 1][m_object->cell()->x + 1].value;
			}
			if (c != nullptr)
			{
				Parameter_Position* P;
						P = new Parameter_Position();
						P->m_object = m_object;
						P->m_place = c;
						P->m_map = m_map;
						m_action_controller->set(P->m_object, Application::instance().m_actions[action_e::move], P);
			}*/
			int radius = 0;
			Vision_list* vl = static_cast<Vision_list*>(m_object->m_active_state->get_list(interaction_e::vision));
			AI_FOV current;
			for (auto item = vl->m_effect.begin(); item != vl->m_effect.end(); item++)
			{
				current = static_cast<Vision_component*>(*item)->m_value;
				radius = max(radius, current.radius);
			}
			Path::instance().calculate(m_map, m_object, c, m_goal, radius);
			std::vector<MapCell*>* path;
			path = Path::instance().get_path();
			if (path)
			{
				if (path->size() >= 2)
				{
					Parameter_Position* P;
					P = new Parameter_Position();
					P->m_object = m_object;
					P->m_place = (*path)[path->size() - 2];
					P->m_map = m_map;
					//Application::instance().m_action_manager->add(m_object, new GameTask(Application::instance().m_actions[action_e::move], P));
					m_action_controller->set(P->m_object, Application::instance().m_actions[action_e::move], P);
				}
				else
				{
					m_goal = nullptr;
					m_memory_goal_cell = nullptr;
				}

			}
			Path::instance().m_heap.m_items.clear();
		}
	}
	m_action_controller->update();
}

void AI_enemy::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_action_controller)
	{
		if (m_action_controller->m_serialization_index > 1)
		{
			m_action_controller->reset_serialization_index();
		}
	}
}

void AI_enemy::save()
{
	//???
	/*LOG(INFO) << "ÈÈ";
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::ai_enemy;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&m_ai_type, sizeof(ai_type_e), 1, file);
	fwrite(&m_fov_radius, sizeof(int), 1, file);
	fwrite(&m_fov_qualifier->index, sizeof(size_t), 1, file);
	fwrite(&m_path_qualifier->index, sizeof(size_t), 1, file);
	Serialization_manager::instance().serialize(m_action_controller);
	LOG(INFO) << "Êîíåö ÈÈ";*/
}

void AI_enemy::load()
{
	//???
	/*LOG(INFO) << "ÈÈ";
	FILE* file = Serialization_manager::instance().m_file;
	fread(&m_ai_type, sizeof(ai_type_e), 1, file);
	fread(&m_fov_radius, sizeof(int), 1, file);
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	m_fov_qualifier = Application::instance().m_ai_manager->m_fov_qualifiers[s];
	fread(&s, sizeof(size_t), 1, file);
	m_path_qualifier = Application::instance().m_ai_manager->m_path_qualifiers[s];
	m_action_controller = dynamic_cast<Action_wrapper*>(Serialization_manager::instance().deserialize());
	LOG(INFO) << "Êîíåö ÈÈ";*/
}

AI_trap::AI_trap(){};

GameObject* AI_trap::find_goal(){ return nullptr; };

void AI_trap::create()
{
	m_object->set_state(object_state_e::off);
	for (auto item = m_object->cell()->m_items.begin(); item != m_object->cell()->m_items.end(); item++)
	{
		if (m_object != (*item) && (*item)->m_name != "floor")
		{
			m_object->set_state(object_state_e::on);
		}
	}
	if (m_object->m_active_state->m_state == object_state_e::on)
	{
		for (auto item = m_object->cell()->m_items.begin(); item != m_object->cell()->m_items.end(); item++)
		{
			if (m_object != (*item) && (*item)->m_name != "floor")
			{
				P_unit_interaction* p = new P_unit_interaction();
				p->m_unit = m_object;
				p->m_object = (*item);
				p->m_unit_body_part = nullptr;
				Application::instance().m_action_manager->add(new GameTask(Application::instance().m_actions[action_e::hit], p));
			}
		}
	}
};

void AI_trap::save()
{
}

void AI_trap::load()
{
}

AI_manager::AI_manager()
{
	m_fov_qualifiers.push_back(new predicat_t([](GameObject* object)->bool{return !object->get_stat(object_tag_e::seethrough_able); },0));
	m_fov_qualifiers.push_back(new predicat_t([](GameObject* object)->bool{return false; },1));

	m_path_qualifiers.push_back(new predicat_t([](GameObject* object)->bool {return !object->get_stat(object_tag_e::pass_able); }, 0));
	m_path_qualifiers.push_back(new predicat_t([](GameObject* object)->bool{return !object->get_stat(object_tag_e::pass_able) && object->m_name != "wall"; },1));
}