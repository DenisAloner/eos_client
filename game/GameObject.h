#ifndef GAMEOBJECT_H
#define	GAMEOBJECT_H

#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <list>
#include <algorithm>
#include <Application.h>
#include "FOV.h"
#include <vector>
#include <map>
#include "Effect.h"

class Object_feature;
class Game_object_owner;
class Action;
class Application;
class TileManager;
class GameObject;
class Object_feature;
class Effect;
class Reaction;

struct label_t
{
	std::string m_name;
	std::vector<object_attribute_e> m_stat;
	label_t(std::string name) :m_name(name){};
};

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
	//virtual Object_feature* find_property(property_e kind, GameObject* excluded);
};

struct AI_configuration
{
public:

	int m_fov_radius;
	std::function<bool(GameObject*)> m_fov_qualifier;
	std::function<bool(GameObject*)> m_path_qualifier;

};

class Object_state
{
public:

	object_state_e m_state;
	int m_layer;

	game_object_size_t m_size;
	dimension_t m_tile_size;
	TileManager* m_tile_manager;
	light_t* m_light;
	GLuint m_icon;

	std::list<Action*> m_actions;
	std::map<object_feature_e,Object_feature*> m_feature;
	
	AI_configuration* m_ai;

	virtual Action* find_action(action_e kind);
	virtual void set_tile_size();

	Object_state();

};

class GameObject
{
public:

	std::string m_name;
	float m_weight;
	ObjectDirection m_direction;
	bool m_selected;
	Game_object_owner* m_owner;

	Object_state* m_active_state;
	std::list<Object_state*> m_state;
	
	GameObject();

	void turn();
	void set_direction(ObjectDirection dir);
	void set_state(object_state_e state);
	Object_state* get_state(object_state_e state);
	void add_attribute(object_attribute_e key);
	void add_label(const std::string& key);
	void add_parameter(object_parameter_e key, object_parameter_t* item);
	void add_effect(effect_e key, Effect* item);
	void add_reaction(reaction_e key, Reaction* item);
	void remove_effect(effect_e key, Effect* item);
	Object_feature* get_feature(object_feature_e key);
	bool get_stat(object_attribute_e key);
	std::list<Effect*>* get_effect(effect_e key);
	object_parameter_t* get_parameter(object_parameter_e key);
	Reaction* get_reaction(reaction_e key);
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

class Object_feature
{
public:

	object_feature_e m_kind;

	Object_feature(object_feature_e kind);
	virtual Object_feature* clone();
};

class Inventory_cell: public Game_object_owner
{
public:
	GameObject* m_item;
	Inventory_cell(GameObject* item);
};

class Property_Container : public Object_feature
{
public:

	std::list<Inventory_cell*> m_items;

	dimension_t m_size;
	std::string m_name;

	Property_Container(int width, int height, std::string name);
	~Property_Container();
	Object_feature* clone();
};

class AI_manager
{
public:

	std::vector<std::function<bool(GameObject*)> > m_fov_qualifiers;
	std::vector<std::function<bool(GameObject*)> > m_path_qualifiers;

	AI_manager();
};

class Body_part : public Inventory_cell
{
public:

	body_part_e m_part_kind;
	std::string m_name;
	Body_part(GameObject* item = nullptr) :Inventory_cell(item){ m_kind = entity_e::body_part; };
};

class Property_body : public Object_feature
{
public:

	std::list<Body_part> m_item;

	Property_body();
	Object_feature* clone();
};

class Attribute_list : public Object_feature
{
public:

	std::list<label_t*> m_label;
	std::list<object_attribute_e> m_stat;

	Attribute_list();
	Object_feature* clone();
};

class Parameter_list : public Object_feature
{
public:

	std::map<object_parameter_e, object_parameter_t > m_parameter;

	Parameter_list();
	Object_feature* clone();
};

class Effect_list : public Object_feature
{
public:

	std::map<effect_e, std::list<Effect*> > m_effect;

	Effect_list();
	Object_feature* clone();
};

class Reaction_list : public Object_feature
{
public:

	std::map<reaction_e, Reaction> m_reaction;

	Reaction_list();
	Object_feature* clone();
};

#endif //GAMEOBJECT_H
