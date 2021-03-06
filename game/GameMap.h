#ifndef GAMEMAP_H
#define	GAMEMAP_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <list>
#include "GraphicalController.h"
#include "Event.h"

class Application;
class GameObject;
class GameObjectProperty;

class MapCell
{
public:
	int x; 
	int y;

	light_t m_light;
	light_t m_light_blur;
	std::list<GameObject*> m_items;

	MapCell(int x0, int y0);

	void add_object(GameObject* Object);
	virtual GameObjectProperty* find_property(property_e kind,GameObject* excluded);
	bool check_permit(property_e kind, GameObject* excluded);
};

class GameMap
{
public:
	bool m_update;
	Event<VoidEventArgs> update;

	struct block_t
	{
		rectangle_t rect;
		block_t(int x, int y, int w, int h) : rect(rectangle_t(x, y, w, h)) {}
	};

	dimension_t m_size;

	float m_coefficient[21][21];
	bool m_light_map[41][41];
	bool m_light_map2[41][41];
	light_t m_local_light[3][41][41];
	bool m_barrier_map[41][41];

	 
	std::vector<std::vector<MapCell*> > m_items;
	std::list<GameObject*> m_lights;
	std::list<block_t*> m_rooms;
	std::list<block_t*> m_link_rooms;

	GameMap(dimension_t size);

	void add_object(GameObject* object, MapCell* cell);
	void remove_object(GameObject* object);
	void move_object(GameObject* object, MapCell* cell);
	void turn_object(GameObject* object);

	void generate_room(void);
	bool divide_block(block_t* Block, int Depth, int Current);
	void generate_level(void);
	void add_light(GameObject* Object);
	void add_new_object(GameObject* Object, MapCell* Element);
	void fill();
	void random_block(block_t* block);
	void calculate_lighting();
	void connect_room();
	void link_room(block_t* a, block_t* b);
	void add_wall();
	void add_doors();
	bool line2(int x1, int y1, int x2, int y2);
	void blur_lighting();
	void add_lighting();

};

#endif //GAMEMAP_H
