#include "AI.h"
#include "log.h"
Node::Node(void)
{
}


Node::~Node(void)
{
}

Path::Path()
{

}

Path::~Path()
{

}

void Path::InsertIntoOpenList(int x, int y)
{

	//node must be inside the boundary of the map
	if (x >= 0 && y >= 0 && x <m_map->m_size.w && y<m_map->m_size.h && ClosedList.size() > 0 && !IsElementExits(x, y))
	{
		NODE tempNode;
		//the tile must be ground tile, so it will be passable
		if (m_map->m_items[y][x]->m_path_info == 0)
		{
			int base_cost, cost_to_start, cost_to_goal;

			base_cost = m_map->m_items[y][x]->m_path_info;
			cost_to_goal = abs(x - goal_x) + abs(y - goal_y);
			cost_to_start = ClosedList.back().StartCost + base_cost;
			//cost_to_start=ClosedList.back().StartCost + abs(x - ClosedList.back().X)+abs(y-ClosedList.back().Y);


			tempNode.StartCost = cost_to_start;
			tempNode.TotalCost = base_cost + cost_to_goal + cost_to_start;
			tempNode.X = x;
			tempNode.Y = y;
			tempNode.ParentX = ClosedList.back().X;
			tempNode.ParentY = ClosedList.back().Y;

			OpenList.push_back(tempNode);
		}
		else if (m_map->m_items[y][x]->m_path_info == 9)
		{
			tempNode.StartCost = 0;
			tempNode.TotalCost = 0;
			tempNode.X = x;
			tempNode.Y = y;
			tempNode.ParentX = ClosedList.back().X;
			tempNode.ParentY = ClosedList.back().Y;

			OpenList.push_back(tempNode);
		}

	}
}
void Path::InsertIntoClosedList(int total_cost, int start_cost, int x, int y)
{

	//node must be inside the boundary of the map
	if (x >= 0 && y >= 0 && x <m_map->m_size.w && y<m_map->m_size.h && !IsElementExits(x, y))
	{
		NODE tempNode;

		tempNode.StartCost = start_cost;
		tempNode.TotalCost = total_cost;
		tempNode.X = x;
		tempNode.Y = y;
		tempNode.ParentX = x;
		tempNode.ParentY = y;
		ClosedList.push_back(tempNode);
	}
}
void Path::Create()
{
	//Clear previous lists
	ClearAll();

	//add the start node to the closed list
	
	InsertIntoClosedList(0, 0, start_x, start_y);
	int next_x = start_x;
	int next_y = start_y;
	while (m_map->m_items[next_y][next_x]->m_path_info != 9 && next_x <m_map->m_size.w && next_x >= 0 && next_y <m_map->m_size.h && next_y >= 0)
	{
		//take all the neighboring passable nodes and add them to the OpenList
		InsertIntoOpenList(next_x + 1, next_y);
		InsertIntoOpenList(next_x + 1, next_y + 1);
		InsertIntoOpenList(next_x, next_y + 1);
		InsertIntoOpenList(next_x - 1, next_y + 1);
		InsertIntoOpenList(next_x - 1, next_y);
		InsertIntoOpenList(next_x - 1, next_y - 1);
		InsertIntoOpenList(next_x, next_y - 1);
		InsertIntoOpenList(next_x + 1, next_y - 1);

		//find the min node

		if (MinNode(OpenList).StartCost != -1)
		{
			NODE minNode = MinNode(OpenList);

			ClosedList.push_back(minNode);

			next_x = minNode.X;
			next_y = minNode.Y;

			//delete the minimum node from the open list
			OpenList = DeleteElement(OpenList, next_x, next_y);
		}

	}
}
std::vector<NODE> Path::BackTrack()
{
	std::vector<NODE> pathL;
	int k;
	k = 0;

	for (int i = ClosedList.size() - 1; i> -1; i--)
	{
		NODE tempNode;
		tempNode = GetNodeAt(ClosedList[i].ParentX, ClosedList[i].ParentY);
		if (tempNode.TotalCost != -1)
			pathL.push_back(tempNode);
	}
	return pathL;
}
NODE Path::GetNodeAt(int x, int y)
{
	NODE tempNode;
	tempNode.TotalCost = -1;
	for (int i = 0; i<ClosedList.size(); i++)
	{
		if (ClosedList[i].X == x && ClosedList[i].Y == y)
		{
			tempNode = ClosedList[i];
		}
	}
	return tempNode;
}
NODE Path::MinNode(std::vector<NODE> nodesList)
{
	NODE minNode;
	if (nodesList.size()>0)
	{
		minNode = nodesList[0];
		for (int i = 1; i<nodesList.size(); i++)
		{
			if (nodesList[i].TotalCost < minNode.TotalCost)
				minNode = nodesList[i];
		}
		return minNode;
	}
	else
	{
		//return an error
		minNode.StartCost = -1;
		minNode.TotalCost = -1;
		minNode.X = 0;
		minNode.Y = 0;

		return minNode;
	}
}

std::vector<NODE> Path::DeleteElement(std::vector<NODE> nodesList, int x, int y)
{
	std::vector<NODE> tempL = nodesList;

	for (int i = 0; i<tempL.size(); i++)
	{
		if (tempL[i].X == x && tempL[i].Y == y)
		{
			tempL.erase(tempL.begin() + i);
			break;
		}
	}
	return tempL;
}

bool Path::IsElementExits(int x, int y)
{
	//check to see if the element is not exit in both lists or not
	for (int i = 0; i<OpenList.size(); i++)
	{
		if (OpenList[i].X == x && OpenList[i].Y == y)
		{
			return true;
		}
	}
	for (int i = 0; i<ClosedList.size(); i++)
	{
		if (ClosedList[i].X == x && ClosedList[i].Y == y)
		{
			return true;
		}
	}
	return false;
}

void Path::ClearAll()
{
	OpenList.erase(OpenList.begin(), OpenList.begin() + OpenList.size());
	ClosedList.erase(ClosedList.begin(), ClosedList.begin() + ClosedList.size());
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
	static_cast<MapCell*>(m_object->m_owner)->m_path_info = 8;
	static_cast<MapCell*>(Application::instance().m_GUI->MapViewer->m_player->m_owner)->m_path_info = 9;
	m_path_creator.start_x=static_cast<MapCell*>(m_object->m_owner)->x;
	m_path_creator.start_y = static_cast<MapCell*>(m_object->m_owner)->y;
	m_path_creator.goal_x = static_cast<MapCell*>(Application::instance().m_GUI->MapViewer->m_player->m_owner)->x;
	m_path_creator.goal_y = static_cast<MapCell*>(Application::instance().m_GUI->MapViewer->m_player->m_owner)->y;
	m_path_creator.m_map = m_map;
	m_path_creator.Create();
	m_path = m_path_creator.BackTrack();
	//set the path to the map
	int i;
	for (i = 0; i<m_path.size() - 1; i++)
	{
		m_map->m_items[m_path[i].Y][m_path[i].X]->m_path_info = 4;
	}
	Parameter_Position* P;
	P = new Parameter_Position();
	P->m_object = m_object;
	P->m_place = m_map->m_items[m_path[m_path.size() - 3].Y][m_path[m_path.size() - 3].X];
	P->m_map = m_map;
	Application::instance().m_action_manager->add(new GameTask(Application::instance().m_actions[action_e::move], P));
	//P->~Parameter_Position();
	m_map->m_items[m_path[i].Y][m_path[i].X]->m_path_info = 2;
	m_path.erase(m_path.begin(), m_path.begin() + m_path.size());
}