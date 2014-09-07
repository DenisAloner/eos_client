#include "AI.h"
#include "log.h"

Path::Path()
{

}

Path::~Path()
{

}


int Path::manhattan(MapCell* A, MapCell* B) {
	return abs(A->x - B->x) + abs(A->y - B->y);
}

Node* Path::find_min()
{
	Node* result = (*OpenList.begin());
	int f = (*OpenList.begin())->f;
	for (std::list<Node*>::iterator item = OpenList.begin(); item != OpenList.end(); item++)
	{
		if ((*item)->f < f)
		{
			f = (*item)->f;
			result = (*item);
		}
	}
	return result;
}

bool Path::is_in_closed(MapCell* c)
{
	for (std::list<Node*>::iterator item = ClosedList.begin(); item != ClosedList.end(); item++)
	{
		if ((*item)->cell==c)
		{
			return true;
		}
	}
	return false;
}

Node* Path::is_in_open(MapCell* c)
{
	for (std::list<Node*>::iterator item = OpenList.begin(); item != OpenList.end(); item++)
	{
		if ((*item)->cell == c)
		{
			return (*item);
		}
	}
	return nullptr;
}

void Path::insert_into_open(int x, int y, int dg, Node* p)
{
	if (x >= 0 && y >= 0 && x < m_map->m_size.w&&y < m_map->m_size.h)
	{
		MapCell* c = m_map->m_items[y][x];
		if ((!is_in_closed(c)) && (c->m_path_info == 0))
		{
			bool refresh = false;
			Node* n = is_in_open(c);
			if (n==nullptr)
			{
				n = new Node(c, 0, 0, p);
				refresh = true;
			}
			else {
				if (p->g + dg<n->g){
					refresh = true;
				}
				else refresh = false;
			}
			if (refresh)
			{
				n->parent = p;
				n->g = p->g + dg;
				n->h = manhattan(n->cell, m_goal)*10;
				n->f = n->g + n->h;
				OpenList.push_front(n);
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
	OpenList.clear();
	ClosedList.clear();
	OpenList.push_back(new Node(A,0,manhattan(A,B)*10));
	bool closed;
	while (!OpenList.empty())
	{
		current = find_min();
		if (current->cell == m_goal)
		{
			return back(current);
		}
		OpenList.remove(current);
		ClosedList.push_front(current);
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
			if (m_map->m_items[y][x]->m_path_info == 4)
			{
				m_map->m_items[y][x]->m_path_info = 0;
			}
				
		}
	}
	m_path_creator.m_map = m_map;
	m_path_creator.m_start = static_cast<MapCell*>(m_object->m_owner);
	m_path_creator.m_start->m_path_info = 0;
	m_path_creator.m_goal = static_cast<MapCell*>(Application::instance().m_GUI->MapViewer->m_player->m_owner);
	m_path_creator.m_goal->m_path_info = 0;
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