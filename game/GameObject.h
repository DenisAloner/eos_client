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
#include <vector>
#include <map>
#include "Effect.h"

class GameObjectProperty;
class Game_object_owner;
class Action;
class Application;
class TileManager;
class GameObject;
class GameObjectProperty;
class Effect;
class Reaction;

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

struct AI_configuration
{
public:

	int m_fov_radius;
	std::function<bool(GameObject*)> m_fov_qualifier;
	std::function<bool(GameObject*)> m_path_qualifier;

};

class Game_state
{
public:

	state_e m_state;
	int m_layer;

	game_object_size_t m_size;
	dimension_t m_tile_size;
	TileManager* m_tile_manager;
	light_t* m_light;
	GLuint m_icon;

	std::list<Action*> m_actions;
	std::list<GameObjectProperty*> m_properties;

	AI_configuration* m_ai;

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
	std::map<effect_e, std::list<Effect*> > m_effect;
	std::map<reaction_e, Reaction*> m_reaction;

	GameObject();

	void set_direction(ObjectDirection dir);
	void turn();
	void set_state(state_e state);
	void add_effect(effect_e kind, Effect* effect);

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

	virtual GameObjectProperty* clone();
};

class GameObjectParameter : public GameObjectProperty
{
public:

	GameObjectParameter(property_e kind, float value);
	float m_value;
	GameObjectProperty* clone();

};

class Game_object_feature : public GameObjectProperty
{
public:

	Game_object_feature(property_e kind, int value);
	int m_value;
	GameObjectProperty* clone();

};

class GameObjectLink : public GameObjectProperty
{
public:

	GameObjectLink(property_e kind, GameObject* object);
	GameObject* m_object;
	GameObjectProperty* clone();

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
	GameObjectProperty* clone();
};

class AI_manager
{
public:

	std::vector<std::function<bool(GameObject*)> > m_fov_qualifiers;
	std::vector<std::function<bool(GameObject*)> > m_path_qualifiers;

	AI_manager();
};



#endif //GAMEOBJECT_H
