#pragma once
#include<iostream>
#include <list>
#include <vector>
#include "GameMap.h"
#include "GameObject.h"
#include "Application.h"
#include "ApplicationGUI.h"
#include "ActionManager.h"
#include "Game_algorithm.h"

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

class min_heap
{
private:

	void sort_up(unsigned int index);
	void sort_down(unsigned int index);

public:
	std::vector<Node*> m_items;
	min_heap(void);

	void push(Node* value);
	Node* pop();
	void edit(unsigned int index, int value);
};

class Path
{

	
public:

	MapCell* m_start;
	MapCell* m_goal;
	GameMap* m_map;
	GameObject* m_object;
	game_object_size_t m_start_size;
	game_object_size_t m_goal_size;
	min_heap m_heap;

	Path();
	virtual ~Path();
	int manhattan(MapCell* A, MapCell* B);
	std::vector<MapCell*>* get_path(MapCell* A, MapCell* B);
	void insert_into_open(int x, int y, int dg, Node* p);
	bool is_in_closed(MapCell* c);
	int is_in_open(MapCell* c);
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

