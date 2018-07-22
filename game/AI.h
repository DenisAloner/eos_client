#ifndef AI_H
#define	AI_H

#include <iostream>
#include <list>
#include <vector>
#include "GameMap.h"
#include "Application.h"
#include "ApplicationGUI.h"
#include "ActionManager.h"
#include "Game_algorithm.h"
#include "FOV.h"
#include <mutex>

class GameMap;
class GameObject;
class MapCell;
class FOV;
class Action_wrapper;
class MapCell;

struct Node
{
	MapCell* cell;
	int g;
	int h;
	int f;
	Node* parent;

	Node(MapCell* c, int G, int H, Node* p = nullptr) : cell(c), g(G), h(H), parent(p), f(g + h) {}
};

class min_heap
{
private:

	void sort_up(std::size_t index);
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

	GameObject* m_unit;
	GameMap* m_game_map;
	MapCell* m_start_cell;
	MapCell* m_goal_cell;
	dimension3_t m_start_size;
	dimension3_t m_goal_size;
	min_heap m_heap;

	int m_open;

	void calculate(GameMap* map, GameObject* object, MapCell* gc, GameObject* goal, int radius);
	void map_costing(GameMap* map, GameObject* object, MapCell* gc, int radius);
	int manhattan(MapCell* a, MapCell* b,MapCell* c);
	std::vector<MapCell*>* get_path();
	std::vector<MapCell*>* get_path_to_cell();
	void insert_into_open(int x, int y, int dg, Node* p);
	int is_in_open(MapCell* c);
	std::vector<MapCell*>* back(Node* c);

private:

	Path();
	Path(const Path& root);
	Path& operator=(const Path&);
};

class Dijkstra_map
{
public:

	GameMap* m_map;
	Dijkstra_map();

	bool opaque_check(int x, int y, int size);
	void calculate_cost(GameMap* map, GameObject* object, GameObject* goal);
	void calculate_cost_autoexplore(GameMap* map, GameObject* object);
	void trace();
	MapCell* next(GameObject* object);
};

class AI: public iSerializable
{
public:

	ai_type_e m_ai_type;
	GameObject* m_object;
	GameMap* m_map;

	//std::list<AI_FOV> m_FOVs;

	Action_wrapper* m_action_controller;

	virtual GameObject* find_goal() = 0;
	virtual void create() = 0;
	virtual AI* clone() = 0;
	virtual void calculate_FOV(GameObject* object, GameMap* map) = 0;

	Packer_generic& get_packer() override = 0;

	constexpr static auto properties() { return std::make_tuple(make_property(&AI::m_ai_type, u"ai_type")); }

};

class AI_enemy: public AI
{
public:

	FOV* m_fov;
	Event<VoidEventArgs> update;

	GameObject* m_goal;
	MapCell* m_memory_goal_cell;

	predicat_t* m_path_qualifier;

	AI_enemy();
	virtual bool check_goal();
	virtual GameObject* find_goal();
	virtual void create();
	virtual AI* clone();

	void reset_serialization_index() override;

	virtual void calculate_FOV(GameObject* object, GameMap* map);

	Packer_generic& get_packer() override;

	constexpr static auto properties() 
	{
		return make_union(AI::properties(),
			std::make_tuple(
			make_property(&AI_enemy::m_path_qualifier, u"path_qualifier")
		));
	}

};

class AI_trap : public AI
{
public:

	AI_trap();
	virtual GameObject* find_goal();
	virtual void create();
	virtual AI* clone() { return nullptr; };

	Packer_generic& get_packer() override;

	virtual void calculate_FOV(GameObject* object, GameMap* map) {};
};

class AI_manager
{
public:

	std::vector<predicat_t*> m_fov_qualifiers;
	std::vector<predicat_t*> m_path_qualifiers;

	AI_manager();
};

#endif //AI_H