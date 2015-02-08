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
#include "FOV.h"

class GameMap;
class GameObject;
class MapCell;
class FOV;

struct Path_cell
{
	int state;
	bool closed;
	position_t pos;
};

struct Node
{
	Path_cell* cell;
	int g;
	int h;
	int f;
	Node* parent;

	Node(Path_cell* c, int G, int H, Node* p = nullptr) : cell(c), g(G), h(H), parent(p), f(g + h) {}
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

	static Path& instance()
	{
		static Path Singleton;
		return Singleton;
	}

	static const int m_max_size = 101;
	static const int m_middle = (m_max_size - 1) >> 1;

	Path_cell m_map[m_max_size][m_max_size];
	GameObject* m_unit;
	GameMap* m_game_map;
	Path_cell* m_start_cell;
	Path_cell* m_goal_cell;
	game_object_size_t m_start_size;
	game_object_size_t m_goal_size;
	min_heap m_heap;

	void calculate(GameMap* map, GameObject* object, GameObject* goal, int radius);
	int manhattan(Path_cell* a, Path_cell* b);
	std::vector<MapCell*>* get_path();
	void insert_into_open(int x, int y, int dg, Node* p);
	int is_in_open(Path_cell* c);
	std::vector<MapCell*>* back(Node* c);

private:

	Path();
	Path(const Path& root);
	Path& operator=(const Path&);
};

class AI
{
public:

	ai_type_e m_ai_type;
	GameObject* m_object;
	GameMap* m_map;

	virtual GameObject* find_goal() = 0;
	virtual void create() = 0;
};

class AI_enemy: public AI
{
public:

	FOV* m_fov;

	int m_fov_radius;
	std::function<bool(GameObject*)> m_fov_qualifier;
	std::function<bool(GameObject*)> m_path_qualifier;

	AI_enemy();
	virtual GameObject* find_goal();
	virtual void create();

};

class AI_trap : public AI
{
public:

	AI_trap();
	virtual GameObject* find_goal();
	virtual void create();

};

