#ifndef GAMEMAP_H
#define	GAMEMAP_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <list>
#include "GraphicalController.h"
#include "Event.h"
#include <functional>
#include "AI.h"
#include "FOV_light.h"

class Application;
class GameObject;
class Object_feature;
class MapCell;
class AI;
class Dijkstra_map;
class Player;

const int max_light_radius = 20;

class Object_manager: public iSerializable
{

public:

	std::list<GameObject*> m_items;
	Object_manager() {};

	void update_buff();
	void calculate_ai();

	void reset_serialization_index() override;
	virtual void save();
	virtual void load();

	Packer_generic& get_packer()
	{
		return Packer<Object_manager>::Instance();
	}

	constexpr static auto properties()
	{
		return std::make_tuple(
			make_property(&Object_manager::m_items, u"items")
		);
	}

};

class GameMap: public iSerializable
{
public:

	Event<VoidEventArgs> update;

	std::size_t m_index;

	Dijkstra_map* m_dijkstra_map;

	struct block_t
	{
		rectangle_t rect;
		block_t(int x, int y, int w, int h) : rect(rectangle_t(x, y, w, h)) {}
	};

	dimension_t m_size;

	std::vector<MapCell> m_items;

	std::list<block_t*> m_rooms;
	std::list<block_t*> m_link_rooms;

	GameMap(dimension_t size);
	GameMap();

	MapCell& get(int y, int x);

	void init(dimension_t size);
	void add_object(GameObject* object, MapCell& cell);
	void remove_object(GameObject* object);
	void move_object(GameObject* object, MapCell& cell);
	void turn_object(GameObject* object);

	void generate_room(void);
	bool divide_block(block_t* Block, int Depth, int Current);
	void generate_level(void);

	void add_to_map(GameObject* object, MapCell& cell);

	void fill();
	void random_block(block_t* block);
	void connect_room();
	void link_room(block_t* a, block_t* b);
	void add_wall();
	void add_doors();
	void generate_traps();
	//bool line2(int x1, int y1, int x2, int y2);
	void add_lighting();
	void bresenham_line(MapCell* a, MapCell* b, std::function<void(MapCell*)> f);
	MapCell* bresenham_line2(MapCell* a, MapCell* b, std::function<bool(MapCell*)> f);
	MapCell* bresenham_line2(MapCell* a, MapCell* b, Parameter* p, std::function<bool(Parameter*,MapCell*)> f);
	bool check(int x,int y);
	//MapCell* get_cell(int x, int y);

	void blur_lighting();

	void reset_serialization_index() override;
	virtual void save();
	virtual void load();

	Packer_generic& get_packer() override
	{
		return Packer<GameMap>::Instance();
	}

	std::u16string vector_mapcell_to_json(std::vector<MapCell>& value);
	void vector_mapcell_from_json(std::u16string value, std::vector<MapCell>& prop);
	std::string vector_mapcell_to_binary(std::vector<MapCell>& value);
	void vector_mapcell_from_binary(const std::string& value, std::vector<MapCell>& prop, std::size_t& pos);

	constexpr static auto properties()
	{
		return std::make_tuple(
			make_property(&GameMap::m_size, u"size"),
			make_property(&GameMap::m_items, u"item", &GameMap::vector_mapcell_to_json, &GameMap::vector_mapcell_from_json, &GameMap::vector_mapcell_to_binary, &GameMap::vector_mapcell_from_binary)
		);
	}

};

class Game_world : public iSerializable
{
public:

	float m_coefficient[21][21];

	Object_manager m_object_manager;

	std::list<GameMap*> m_maps;

	Player* m_player;

	Game_world();
	void calculate_lighting();
	void reset_serialization_index() override;
	virtual void save();
	virtual void load();


	Packer_generic& get_packer()
	{
		return Packer<Game_world>::Instance();
	}

	constexpr static auto properties()
	{
		return std::make_tuple(
			make_property(&Game_world::m_maps, u"maps")
		);
	}

};

#endif //GAMEMAP_H
