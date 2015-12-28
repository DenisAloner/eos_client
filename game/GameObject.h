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
#include "Action.h"

class Object_feature;
class Application;
class TileManager;
class GameObject;
class Effect;
class Object_interaction;
class Interaction_list;
class Parameter_list;
class Tag_list;
struct object_parameter_t;
class Action;
class AI;

class Game_object_owner: public virtual iSerializable
{
public:
	entity_e m_kind;
};

class MapCell : public Game_object_owner
{
public:

	GameMap* m_map;

	bool m_closed;
	int m_state;

	bool m_notable;

	bool m_mark;

	int x;
	int y;

	cell_t m_pathfind_info;

	std::list<GameObject*> m_items;

	unsigned int m_path_info;

	light_t m_light;
	light_t m_light_blur;

	MapCell(int x, int y, GameMap* map);

	void add_object(GameObject* Object);
	//virtual Object_feature* find_property(property_e kind, GameObject* excluded);

	virtual void reset_serialization_index() { m_serialization_index = 1; };
	virtual void save();
	virtual void load();
};

class Attribute_map: public iSerializable
{
public:

	std::map<interaction_e, Interaction_list* > m_item;

	Attribute_map();
	void add_effect(interaction_e key, Object_interaction* item);
	Interaction_list* create_feature_list(feature_list_type_e key, interaction_e name);
	Attribute_map* clone();

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();

	Interaction_list* get_list(interaction_e key);

};

class Object_state : public Attribute_map
{
public:

	object_state_e m_state;
	int m_layer;
	game_object_size_t m_size;
	GLuint m_icon;
	float m_weight;
	light_t* m_light;
	optical_properties_t* m_optical;
	
	float* m_visibility;

	TileManager* m_tile_manager;
	dimension_t m_tile_size;

	AI* m_ai;

	//virtual Action* find_action(action_e kind);
	virtual void set_tile_size();
	Object_state();
	virtual Object_state* clone();

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();
};

class Object_state_equip :public Object_state
{
public:

	body_part_e m_body_part;
	Object_state_equip();
	virtual Object_state* clone();

};

class GameObject : public  Object_interaction
{
public:

	Event<VoidEventArgs> event_update;

	std::string m_name;
	object_direction_e m_direction;
	bool m_selected;

	Game_object_owner* m_owner;

	Object_state* m_active_state;
	std::list<Object_state*> m_state;

	GameObject();

	void turn();
	void set_direction(object_direction_e dir);
	void set_state(object_state_e state);
	Object_state* get_state(object_state_e state);
	void add_effect(interaction_e key, Object_interaction* item);
	Interaction_list* get_effect(interaction_e key);
	void remove_effect(interaction_e key, Object_interaction* item);
	bool get_stat(object_tag_e key);
	Object_tag* get_tag(object_tag_e key);
	Parameter_list* get_parameter(interaction_e key);
	Parts_list* GameObject::get_parts_list(interaction_e key);
	MapCell* cell();
	void update_interaction();
	Interaction_list* create_feature_list(feature_list_type_e key, interaction_e name);
	void add_from(interaction_e key, Interaction_list* feature);
	void remove_from(interaction_e key, Interaction_list* feature);
	bool is_own(MapCell* cell);
	bool is_own(int x, int y);
	MapCell* get_center(MapCell* c);

	virtual bool on_turn();
	virtual std::string get_description();
	virtual Object_interaction* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void do_predicat(predicat func);

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();
};

class Player
{
public:

	std::list<Action*> m_actions;
	GameObject* m_object;
	GameMap* m_map;
	FOV* m_fov;
	Player(GameObject* object, GameMap* map);

};

class Inventory_cell: public Game_object_owner
{
public:

	GameObject* m_item;
	Inventory_cell(GameObject* item);

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();
};

class Object_part : public Inventory_cell, public Object_interaction
{
public:

	Attribute_map m_object_state;
	body_part_e m_part_kind;
	std::string m_name;
	Object_part(GameObject* item = nullptr);
	virtual bool on_turn();
	virtual std::string get_description();
	virtual Object_part* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void do_predicat(predicat func);

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();
};

#endif //GAMEOBJECT_H
