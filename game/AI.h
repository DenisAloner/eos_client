#pragma once
#include<iostream>
#include <list>
#include <vector>
#include "GameMap.h"
#include "GameObject.h"
#include "Application.h"
#include "ApplicationGUI.h"
#include "ActionManager.h"

class GameMap;
class GameObject;
class MapCell;

struct Node
{
	MapCell* cell;
	int g;
	int h;
	int f;
	Node* parent;

	Node(MapCell* c, int G, int H, Node* p = nullptr) : cell(c), g(G),h(H),parent(p),f(g+h) {}
};

class Path
{
private:
	std::list<Node*> OpenList, ClosedList;
	
public:

	MapCell* m_start;
	MapCell* m_goal;
	GameMap* m_map;

	Path();
	virtual ~Path();
	int manhattan(MapCell* A, MapCell* B);
	Node* find_min();
	std::vector<MapCell*>* get_path(MapCell* A, MapCell* B);
	void insert_into_open(int x, int y, int dg, Node* p);
	bool is_in_closed(MapCell* c);
	Node* is_in_open(MapCell* c);
	std::vector<MapCell*>* back(Node* c);
};

class AI
{
public:

	Path m_path_creator;
	GameObject* m_object;

	void create();

	GameMap* m_map;

	AI();
	~AI();
};

