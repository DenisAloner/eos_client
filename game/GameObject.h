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
class Effect;
class Reaction;
class Object_interaction;
class Interaction_list;
class Parameter_list;
struct object_parameter_t;

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

	Event<VoidEventArgs> event_update;

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
	void add_parameter(interaction_e key, int value, int limit);
	void add_to_parameter(interaction_e key, Object_interaction* item);
	void add_effect(interaction_e key, Object_interaction* item);
	void add_reaction(reaction_e key, Reaction* item);
	void remove_effect(interaction_e key, Object_interaction* item);
	Object_feature* get_feature(object_feature_e key);
	bool get_stat(object_attribute_e key);
	Interaction_list* get_effect(interaction_e key);
	Parameter_list* get_parameter(interaction_e key);
	Reaction* get_reaction(reaction_e key);
	MapCell* cell();
	void update_interaction();

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

class Attribute_feature : public Object_feature
{
public:

	std::list<label_t*> m_label;
	std::list<object_attribute_e> m_stat;

	Attribute_feature();
	Object_feature* clone();
};

class Object_parameter_modifier
{
public:
	Object_parameter_modifier();
};

class Object_parameter_modifier_single: public Object_parameter_modifier
{
public:
	int m_value;
	int m_duration;
	Object_parameter_modifier_single(int value, int duration);
};

class Object_parameter_modifier_list : public Object_parameter_modifier
{
public:
	std::list<Object_parameter_modifier*> m_item;
	Object_parameter_modifier_list();
};

class object_parameter_t
{
public:
	int m_value;
	int m_limit;
	Object_parameter_modifier_list m_modifier;
	object_parameter_t(int value, int limit);
};

//class Parameter_feature : public Object_feature
//{
//public:
//
//	std::map<object_parameter_e, object_parameter_t > m_parameter;
//
//	Parameter_feature();
//	Object_feature* clone();
//};

class Interaction_feature : public Object_feature
{
public:

	std::map<interaction_e, Interaction_list* > m_effect;

	Interaction_feature();
	Object_feature* clone();
};

class Reaction_feature : public Object_feature
{
public:

	std::map<reaction_e, Reaction*> m_reaction;

	Reaction_feature();
	Object_feature* clone();
};

#endif //GAMEOBJECT_H
