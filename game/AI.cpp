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

Path::~Path()
{

}


int Path::manhattan(MapCell* A, MapCell* B) {
	return abs(A->x - B->x) + abs(A->y - B->y);
}

bool Path::is_in_closed(MapCell* c)
{
	return c->m_closed;
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
	for (int i = 0; i < m_object->m_active_state->m_size.y; i++)
	{
		for (int j = 0; j < m_object->m_active_state->m_size.x; j++)
		{
			if (x - j >= 0 && y + i >= 0 && x - j < m_map->m_size.w&&y + i < m_map->m_size.h)
			{
				if (m_map->m_items[y + i][x - j]->m_path_info != 0)
				{
					return;
				}
			}
			else return;
		}
	}
	MapCell* c = m_map->m_items[y][x];
	if (!is_in_closed(c))
	{
		int n = is_in_open(c);
		if (n == -1)
		{
			m_heap.push(new Node(c, p->g + dg, manhattan(c, m_goal) * 10, p));
		}
		else {
			if (p->g + dg < m_heap.m_items[n]->g){
				m_heap.m_items[n]->parent = p;
				m_heap.m_items[n]->g = p->g + dg;
				m_heap.m_items[n]->h = manhattan(m_heap.m_items[n]->cell, m_goal) * 10;
				m_heap.edit(n, m_heap.m_items[n]->g + m_heap.m_items[n]->h);
			}
		}
	}
}

std::vector<MapCell*>* Path::back(Node* c)
{
	std::vector<MapCell*>* result = new std::vector<MapCell*>();
	Node* current=c;
	while (current)
	{
		result->push_back(current->cell);
		current = current->parent;
	}
	return result;
}

std::vector<MapCell*>* Path::get_path(MapCell* A, MapCell* B){
	Node* current;
	m_heap.push(new Node(A,0,manhattan(A,B)*10));
	bool closed;
	while (!m_heap.m_items.empty())
	{
		current = m_heap.pop();
		if (current->cell == m_goal)
		{
			return back(current);
		}
		current->cell->m_closed = true;
		insert_into_open(current->cell->x + 1, current->cell->y, 10, current);
		insert_into_open(current->cell->x - 1, current->cell->y, 10, current);
		insert_into_open(current->cell->x, current->cell->y + 1, 10, current);
		insert_into_open(current->cell->x, current->cell->y - 1, 10, current);
		insert_into_open(current->cell->x + 1, current->cell->y+1, 14, current);
		insert_into_open(current->cell->x + 1, current->cell->y-1, 14, current);
		insert_into_open(current->cell->x - 1, current->cell->y + 1, 14, current);
		insert_into_open(current->cell->x - 1, current->cell->y - 1, 14, current);
	}
	return nullptr;
}

AI::AI()
{
}


AI::~AI()
{
}

void AI::create()
{
	for (int y = 0; y < m_map->m_size.h; y++)
	{
		for (int x = 0; x < m_map->m_size.w; x++)
		{
			m_map->m_items[y][x]->m_closed = false;
			if (m_map->m_items[y][x]->m_path_info == 4)
			{
				m_map->m_items[y][x]->m_path_info = 0;
			}
				
		}
	}
	MapCell* c = static_cast<MapCell*>(m_object->m_owner);
	m_path_creator.m_map = m_map;
	m_path_creator.m_object = m_object;
	for (int i = 0; i < m_object->m_active_state->m_size.y; i++)
	{
		for (int j = 0; j < m_object->m_active_state->m_size.x; j++)
		{
			m_map->m_items[c->y + i][c->x - j]->m_path_info = 0;
		}
	}
	m_path_creator.m_start = c;
	m_path_creator.m_start->m_path_info = 0;
	c = static_cast<MapCell*>(Application::instance().m_GUI->MapViewer->m_player->m_owner);
	m_path_creator.m_goal = c;
	for (int i = 0; i < Application::instance().m_GUI->MapViewer->m_player->m_active_state->m_size.y; i++)
	{
		for (int j = 0; j < Application::instance().m_GUI->MapViewer->m_player->m_active_state->m_size.x; j++)
		{
			m_map->m_items[c->y + i][c->x - j]->m_path_info = 0;
		}
	}
	std::vector<MapCell*>* path;
	path=m_path_creator.get_path(m_path_creator.m_start, m_path_creator.m_goal);
	if (path)
	{
		for (std::vector<MapCell*>::iterator item = path->begin(); item != path->end(); item++)
		{
			(*item)->m_path_info = 4;
		}
		Parameter_Position* P;
		P = new Parameter_Position();
		P->m_object = m_object;
		P->m_place = (*path)[path->size()-2];
		P->m_map = m_map;
		Application::instance().m_action_manager->add(new GameTask(Application::instance().m_actions[action_e::move], P));
	}
}