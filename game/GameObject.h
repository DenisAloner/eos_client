#ifndef GAMEOBJECT_H
#define	GAMEOBJECT_H

#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <list>
#include <algorithm>
#include <Application.h>
#include "GameObjectProperty.h"
#include "FOV.h"

class GameObjectProperty;
class Game_object_owner;
class Action;
class Application;
class TileManager;
class GameObject;
class GameObjectProperty;

class Game_object_owner{

public:

	entity_e m_kind;
	Game_object_owner();

};

class MapCell : public Game_object_owner
{
public:

	int x;
	int y;

	std::list<GameObject*> m_items;

	unsigned int m_path_info;

	light_t m_light;
	light_t m_light_blur;

	MapCell(int x, int y);

	void add_object(GameObject* Object);
	virtual GameObjectProperty* find_property(property_e kind, GameObject* excluded);
	bool check_permit(property_e kind, GameObject* excluded);
};

class Game_state
{
public:

	state_e m_kind;
	int m_layer;

	game_object_size_t m_size;
	dimension_t m_tile_size;
	TileManager* m_tile_manager;
	light_t* m_light;
	GLuint m_icon;

	std::list<Action*> m_actions;
	std::list<GameObjectProperty*> m_properties;

	virtual Action* find_action(action_e kind);
	virtual GameObjectProperty* find_property(property_e kind);
	virtual void set_tile_size();

	Game_state();

};

class GameObject
{
public:

	std::string m_name;
	float m_weight;
	ObjectDirection m_direction;
	bool m_selected;
	Game_object_owner* m_owner;

	Game_state* m_active_state;
	std::list<Game_state*> m_state;

	GameObject();

	void set_direction(ObjectDirection dir);
	void turn();
	void set_state(state_e state);

	MapCell* cell();

};

class Player
{
public:

	GameObject* m_object;
	GameMap* m_map;
	FOV* m_fov;
	Player(GameObject* object, GameMap* map);

};

class GameObjectProperty
{
public:
	property_e m_kind;

	GameObjectProperty(property_e _kind);
	~GameObjectProperty(void);
};

class GameObjectParameter : public GameObjectProperty
{
public:

	GameObjectParameter(property_e kind, float value);
	float m_value;

};

class Inventory_cell: public Game_object_owner
{
public:
	GameObject* m_item;
	Inventory_cell(GameObject* item);
};

class Property_Container : public GameObjectProperty
{
public:

	std::list<Inventory_cell*> m_items;

	dimension_t m_size;
	std::string m_name;

	Property_Container(int width, int height, std::string name);
	~Property_Container();

};



#endif //GAMEOBJECT_H
