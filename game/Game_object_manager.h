#pragma once
#include "log.h"
#include <map>
#include "GameObject.h"
#include "Effect.h"

struct label_t;
class Reaction;
class Reaction_effect;
class Effect;
class Buff;
class Object_interaction;
class Interaction_slot;

class Reaction_manager
{
public:

	typedef std::function<void(Reaction*, GameObject*, Object_interaction*)> func;
	typedef std::list < func > list;

	void get_damage_basic(Reaction* reaction, GameObject* object, Object_interaction* effect);
	void change_health_basic(Reaction* reaction, GameObject* object, Object_interaction* effect);
	void get_buff_basic(Reaction* reaction, GameObject* object, Object_interaction* effect);
	void add_modificator_generic(Reaction* reaction, GameObject* object, Object_interaction* effect);

	std::map<reaction_applicator_e, func> m_items;
	Reaction_manager();
};

class GameObjectManager
{
public:

	enum class command_e
	{ 
		label,
		add_to_label,
		obj,
		state,
		add_label,
		ai,
		size,
		weight,
		layer,
		icon,
		tile_manager_single,
		tile_manager_map,
		tile_manager_rotating,
		light,
		action_move,
		attribute,
		property_container,
		property_body,
		parameter,
		effect,
		reaction_effect,
		buff,
		slot
	};

	enum class parameter_e
	{
		head,
		hand,
		foot
	};

	typedef std::map<std::string, GameObject*> items_t;
	typedef std::map<std::string, command_e> commands_t;
	typedef std::map<std::string, parameter_e> parameters_t;
	 
	std::map<std::string, label_t*> m_labels;

	std::map<std::string, object_feature_e> m_to_object_feature_e;
	std::map<std::string, reaction_e> m_to_reaction_e;
	std::map<std::string, interaction_e> m_to_interaction_e;
	std::map<std::string, effect_e> m_to_effect_e;
	std::map<std::string, reaction_applicator_e> m_to_reaction_applicator_e;
	std::map<std::string, object_attribute_e> m_to_object_attribute_e;
	std::map<std::string, object_parameter_e> m_to_object_parameter_e;
	std::map<std::string, object_state_e> m_to_object_state_e;

	std::map<interaction_e, std::string> m_effect_string;
	std::map<effect_e, std::string> m_effect_subtype_string;
	std::map<object_parameter_e, std::string> m_object_parameter_string;

	items_t m_items;
	commands_t m_commands;
	parameters_t m_parameters;

	GameObject* m_object;
	label_t* m_label;
	Reaction_effect* m_reaction;
	Effect* m_effect;
	Buff* m_buff;
	Interaction_slot* m_slot;

	std::list<GameObject*> m_objects;

	Reaction_manager* m_reaction_manager;

	object_feature_e get_object_feature_e(const std::string& key);
	reaction_e get_reaction_e(const std::string& key);
	interaction_e get_interaction_e(const std::string& key);
	effect_e get_effect_e(const std::string& key);
	reaction_applicator_e get_reaction_applicator_e(const std::string& key);
	object_attribute_e get_object_attribute_e(const std::string& key);
	object_parameter_e get_object_parameter_e(const std::string& key);
	object_state_e get_object_state_e(const std::string& key);

	void parser(const std::string& command);
	void init();
	GameObject* new_object(std::string unit_name);
	void update_buff();

	std::string get_effect_string(interaction_e key);
	std::string get_effect_subtype_string(effect_e key);
	std::string get_object_parameter_string(object_parameter_e key);
};
