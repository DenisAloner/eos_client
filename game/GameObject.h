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
#include "Visitors.h"

class Object_feature;
class Application;
class TileManager;
class GameObject;
class Effect;
class Object_interaction;
class Interaction_list;
class Parameter_list;
class Vision_list;
class Vision_component;
class Tag_list;
struct object_parameter_t;
class Action;
class AI;
class Object_tag;
class Icon;

class Game_object_owner : public virtual iSerializable
{
public:
	entity_e m_kind;
	Game_object_owner* m_owner;

	Game_object_owner();

	Game_object_owner* get_owner();
	Game_object_owner* get_owner(entity_e kind);

	Packer_generic& get_packer() override = 0;
	/*{
		return Packer<Game_object_owner>::Instance();
	}*/

	constexpr static auto properties() 
	{
		return std::make_tuple(
			make_property(&Game_object_owner::m_kind, u"kind"),
			make_property(&Game_object_owner::m_owner, u"owner")
		);
	}

	void reset_serialization_index() override;
};

//template<>
//class Packer<MapCell>;

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

	MapCell();
	MapCell(int x, int y, GameMap* map);

	void add_object(GameObject* Object);
	//virtual Object_feature* find_property(property_e kind, GameObject* excluded);

	void reset_serialization_index() override;
	virtual void save();
	virtual void load();

	Packer_generic& get_packer() override;

	constexpr static auto properties()
	{
		return std::make_tuple(
			make_property(&MapCell::x, u"x"),
			make_property(&MapCell::y, u"y"),
			make_property(&MapCell::m_items, u"items")
		);
	}
};

class Tag_getter : public Visitor
{
public:

	object_tag_e m_key;
	Object_tag* m_result;

	Tag_getter(object_tag_e key);
	virtual void visit(Object_interaction& value);

};

class Attribute_map : public iSerializable
{
public:

	std::map<interaction_e, Interaction_list* > m_items;

	Attribute_map();
	void add_effect(interaction_e key, Object_interaction* item);
	Interaction_list* create_feature_list(feature_list_type_e key, interaction_e name);
	Attribute_map* clone();

	void reset_serialization_index() override;
	virtual void save();
	virtual void load();

	Interaction_list* get_list(interaction_e key);
	bool get_stat(object_tag_e key);
	Object_tag* get_tag(object_tag_e key);
	virtual void apply_visitor(Visitor_generic& visitor);

	// Для поддержки iSerializable
	Packer_generic& get_packer() override;

	constexpr static auto properties() 
	{
		return std::make_tuple(
			make_property(&Attribute_map::m_items, u"item")
		);
	}

};


class Object_state : public Attribute_map
{
public:

	object_state_e m_state;
	int m_layer;
	game_object_size_t m_size;
	Icon* m_icon;
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
	void apply_visitor(Visitor_generic& visitor) override;

	void reset_serialization_index() override;
	virtual void save();
	virtual void load();

	Packer_generic& get_packer() override;

	std::u16string icon_to_json(Icon*& value, Parser_context& context);
	std::string icon_to_binary(Icon*& value, Parser_context& context);
	void icon_from_json(std::u16string value, Icon*& prop, Parser_context& context);
	void icon_from_binary(const std::string& value, Icon*& prop, std::size_t& pos, Parser_context& context);

	std::u16string tilemanager_to_json(TileManager*& value, Parser_context& context);
	std::string tilemanager_to_binary(TileManager*& value, Parser_context& context);
	void tilemanager_from_json(std::u16string value, TileManager*& prop, Parser_context& context);
	void tilemanager_from_binary(const std::string& value, TileManager*& prop, std::size_t& pos, Parser_context& context);
	
	constexpr static auto properties() {
		return make_union(
			Attribute_map::properties(),
			std::make_tuple(
				make_property(&Object_state::m_state, u"state"),
				make_property(&Object_state::m_size, u"size"),
				make_property(&Object_state::m_layer, u"layer"),
				make_property(&Object_state::m_ai, u"AI"),
				make_property(&Object_state::m_tile_manager, u"tile_manager", &Object_state::tilemanager_to_json, &Object_state::tilemanager_from_json, &Object_state::tilemanager_to_binary, &Object_state::tilemanager_from_binary),
				make_property(&Object_state::m_icon, u"icon", &Object_state::icon_to_json, &Object_state::icon_from_json, &Object_state::icon_to_binary, &Object_state::icon_from_binary),
				make_property(&Object_state::m_light, u"light"),
				make_property(&Object_state::m_optical, u"optical")
			));
	}

};

class Object_state_equip :public Object_state
{
public:

	body_part_e m_body_part;
	Attribute_map m_equip;
	Object_state_equip();
	virtual Object_state* clone();

	Packer_generic& get_packer() override;

	constexpr static auto properties()
	{
		return make_union(
			Object_state::properties(),
			std::make_tuple(
				make_property(&Object_state_equip::m_body_part, u"body_part"),
				make_property(&Object_state_equip::m_equip, u"equip")
			));
	}
};

class GameObject : public Object_interaction, public GUI_connectable_i, public Game_object_owner
{
public:

	std::u16string m_name;

	object_direction_e m_direction;
	bool m_selected;

	Object_state* m_active_state;
	std::list<Object_state*> m_state;

	GameObject();

	void turn();
	void set_direction(object_direction_e dir);
	void set_state(object_state_e state);
	Object_state* get_state(object_state_e state);
	void add_effect(interaction_e key, Object_interaction* item);
	void add_from(interaction_e key, Object_interaction* item);
	Interaction_list* get_effect(interaction_e key);
	void remove_effect(interaction_e key, Object_interaction* item);
	bool get_stat(object_tag_e key);
	Object_tag* get_tag(object_tag_e key);
	Parameter_list* get_parameter(interaction_e key);
	Parts_list* GameObject::get_parts_list(interaction_e key);
	MapCell* cell();
	void update_interaction();
	void add_from(interaction_e key, Interaction_list* feature);
	void remove_from(interaction_e key, Interaction_list* feature);
	bool is_own(MapCell* cell);
	bool is_own(int x, int y);
	MapCell* get_center(MapCell* c);

	virtual Object_interaction* clone();
	virtual void do_predicat(predicat func);
	void apply_visitor(Visitor_generic& visitor) override;

	void reset_serialization_index() override;
	virtual void save();
	virtual void load();

	virtual void get_actions_list(std::list<Action_helper_t>& value);

	// Для поддержки iJSONSerializable
	Packer_generic& get_packer() override;

	constexpr static auto properties() 
	{
		return std::make_tuple(
			make_property(&GameObject::m_name, u"name"),
			make_property(&GameObject::m_direction, u"direction"),
			make_property(&GameObject::m_owner, u"owner"),
			make_property(&GameObject::m_state, u"state"),
			make_property(&GameObject::m_active_state, u"active_state")
		);
	}

private:

	class Action_getter : public Visitor
	{
	public:

		std::list<Action_helper_t>& m_list;
		GameObject* m_object;

		Action_getter(GameObject* object, std::list<Action_helper_t>& list);
		void visit(Object_interaction& value) override;

	};

};

class Config : public iSerializable
{
public:

	std::map<std::string, Object_interaction*> m_templates;
	std::map<std::string, GameObject> m_items;
	InstanceDictonary<TileManager*> m_tile_managers;
	InstanceDictonary<Icon*> m_icons;

	void save() override {}
	void load() override {}

	Packer_generic& get_packer() override;

	void instancedictonary_icon_from_json(std::u16string value, InstanceDictonary<Icon*>& prop, Parser_context& context);
	void instancedictonary_icon_from_binary(const std::string& value, InstanceDictonary<Icon*>& prop, std::size_t& pos, Parser_context& context);

	void instancedictonary_tilemanager_from_json(std::u16string value, InstanceDictonary<TileManager*>& prop, Parser_context& context);
	void instancedictonary_tilemanager_from_binary(const std::string& value, InstanceDictonary<TileManager*>& prop, std::size_t& pos, Parser_context& context);

	constexpr static auto properties()
	{
		return std::make_tuple(
			make_property(&Config::m_icons, u"icons").from_json(&Config::instancedictonary_icon_from_json).from_binary(&Config::instancedictonary_icon_from_binary),
			make_property(&Config::m_tile_managers, u"tile_managers").from_json(&Config::instancedictonary_tilemanager_from_json).from_binary(&Config::instancedictonary_tilemanager_from_binary),
			make_property(&Config::m_templates, u"templates"),
			make_property(&Config::m_items, u"items")
		);
	}
};

class Player : public GUI_connectable_i
{
public:

	std::list<Action*> m_actions;
	GameObject* m_object;
	GameMap* m_map;
	Player(GameObject* object, GameMap* map);

	virtual void get_actions_list(std::list<Action_helper_t>& value);
};



class Inventory_cell : public Game_object_owner
{
public:

	GameObject* m_item;

	//Inventory_cell();
	Inventory_cell(GameObject* item = nullptr);

	void reset_serialization_index() override;
	virtual void save();
	virtual void load();

	Packer_generic& get_packer() override;

	constexpr static auto properties()
	{
		return make_union(
			Game_object_owner::properties(),
			std::make_tuple(
				make_property(&Inventory_cell::m_item, u"item")
			));
	}
};

class Object_part : public Inventory_cell, virtual public Object_interaction
{
public:

	Attribute_map m_attributes;
	body_part_e m_part_kind;
	std::u16string m_name;

	Object_part(GameObject* item = nullptr);
	virtual Object_part* clone();
	virtual void do_predicat(Visitor& helper);
	void apply_visitor(Visitor_generic& visitor) override;

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();

	Packer_generic& get_packer() override;

	constexpr static auto properties() 
	{
		return make_union(
			Inventory_cell::properties(),
			std::make_tuple(
				make_property(&Object_part::m_part_kind, u"part_kind"),
				make_property(&Object_part::m_name, u"name"),
				make_property(&Object_part::m_attributes, u"attributes")
			));
	}

};

#endif //GAMEOBJECT_H
