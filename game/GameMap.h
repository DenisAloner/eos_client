#pragma once
#include "GraphicalController.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include "GameObject.h"
#include <list>
#include "GameObjectProperty.h"
#include "Application.h"

class Application;

class GameObject;
class GameObjectProperty;

class MapCell
{

public:

	int x; 
	int y;

	light_t m_light;
	std::list<GameObject*> m_items;


	MapCell(int x0, int y0);

	void add_object(GameObject* Object);
	virtual GameObjectProperty* find_property(property_e kind,GameObject* excluded);

};

class GameMap
{
public:

	struct block_t
	{
		rectangle_t rect;
		block_t(int x, int y, int w, int h) : rect(rectangle_t(x, y, w, h)) {}
	};

	dimension_t m_size;
	float m_coefficient[10000];
	 
	std::vector<std::vector<MapCell*> > m_items;
	std::list<GameObject*> m_lights;
	std::list<block_t*> m_rooms;
	std::list<block_t*> m_link_rooms;

	GameMap(dimension_t size);

	void add_object(GameObject* Object0, MapCell* Cell0);
	void generate_room(void);
	bool divide_block(block_t* Block, int Depth, int Current);
	void generate_level(void);
	void move_object(GameObject* Obj,MapCell* Pos);
	void add_light(GameObject* Object);
	void add_new_object(GameObject* Object, MapCell* Element);
	void fill();
	void random_block(block_t* block);
	void calculate_lighting();
	void connect_room();
	void link_room(block_t* a, block_t* b);
	void add_wall();
	void add_doors();
};

