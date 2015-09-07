#include "AI.h"
#include "log.h"
#include "GameObject.h"

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

void Path::calculate(GameMap* map, GameObject* object, MapCell* gc,GameObject* goal, int radius)
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
	m_goal_size = goal->m_active_state->m_size;
	std::function<bool(GameObject*)> qualifier = static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_path_qualifier->predicat;
	for (int y = 0; y < map->m_size.h; y++)
	{
		for (int x = 0; x < map->m_size.w; x++)
		{
			c = map->m_items[y][x];
			c->m_closed = false;
			c->m_state = 0;
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

int Path::manhattan(MapCell* a, MapCell* b) {
	return abs(a->x - b->x) + abs(a->y - b->y);
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
		result->push_back(current->cell);
		current = current->parent;
	}
	return result;
}

std::vector<MapCell*>* Path::get_path(){
	Node* current;
	m_heap.push(new Node(m_start_cell, 0, manhattan(m_start_cell, m_goal_cell) * 10));
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
	m_heap.push(new Node(m_start_cell, 0, manhattan(m_start_cell, m_goal_cell) * 10));
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
	c->m_fov_radius = m_fov_radius;
	c->m_fov_qualifier = m_fov_qualifier;
	c->m_path_qualifier = m_path_qualifier;
	return c;
}

GameObject* AI_enemy::find_goal()
{
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
					if (fc->visible)
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

bool AI_enemy::check_goal()
{
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
					if (fc->visible)
					{
						mc = m_map->m_items[m_object->cell()->y + y][m_object->cell()->x + x];
						for (std::list<GameObject*>::iterator item = mc->m_items.begin(); item != mc->m_items.end(); item++)
						{
							if ((*item) == m_goal)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

void AI_enemy::create()
{
	if (m_object->m_active_state->m_ai == nullptr){ return; }
	if (!m_action_controller->m_action)
	{
		m_fov->calculate(m_fov_radius, m_object, m_map);
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
		if (Game_algorithm::check_distance(static_cast<MapCell*>(m_object->m_owner), m_object->m_active_state->m_size, static_cast<MapCell*>(m_goal->m_owner), m_goal->m_active_state->m_size)) {
			P_unit_interaction* p = new P_unit_interaction();
			p->m_unit = m_object;
			p->m_object = m_goal;
			p->m_unit_body_part = nullptr;
			//Application::instance().m_action_manager->add(p->m_unit, new GameTask(Application::instance().m_actions[action_e::hit], p));
			m_action_controller->set(p->m_unit,Application::instance().m_actions[action_e::hit], p);
		}
		else
		{
			Path::instance().calculate(m_map, m_object, c, m_goal, m_fov_radius);
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
	LOG(INFO) << "ÈÈ";
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::ai_enemy;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&m_ai_type, sizeof(ai_type_e), 1, file);
	fwrite(&m_fov_radius, sizeof(int), 1, file);
	fwrite(&m_fov_qualifier->index, sizeof(size_t), 1, file);
	fwrite(&m_path_qualifier->index, sizeof(size_t), 1, file);
	Serialization_manager::instance().serialize(m_action_controller);
	LOG(INFO) << "Êîíåö ÈÈ";
}

void AI_enemy::load()
{
	LOG(INFO) << "ÈÈ";
	FILE* file = Serialization_manager::instance().m_file;
	fread(&m_ai_type, sizeof(ai_type_e), 1, file);
	fread(&m_fov_radius, sizeof(int), 1, file);
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	m_fov_qualifier = Application::instance().m_ai_manager->m_fov_qualifiers[s];
	fread(&s, sizeof(size_t), 1, file);
	m_path_qualifier = Application::instance().m_ai_manager->m_path_qualifiers[s];
	m_action_controller = dynamic_cast<Action_wrapper*>(Serialization_manager::instance().deserialize());
	LOG(INFO) << "Êîíåö ÈÈ";
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