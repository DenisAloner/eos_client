#pragma once
#include "log.h"
#include <map>
#include "GameObject.h"
#include "Effect.h"

struct label_t;
class Effect;
class Buff;
class Object_interaction;
class Interaction_slot;
class Interaction_copyist;
class Object_tag;
//class Object_ObjectTag::Poison_resist;

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
		add_action,
		attribute,
		property_container,
		property_body,
		parameter,
		mem_slot_timer,
		mem_effect,
		mem_slot_time,
		mem_slot_copyist,
		mem_slot_prefix,
		mem_slot_addon,
		add_slot,
		tag,
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
	std::map<std::string, interaction_e> m_to_interaction_e;
	std::map<std::string, effect_e> m_to_effect_e;
	std::map<std::string, object_attribute_e> m_to_object_attribute_e;
	std::map<std::string, object_state_e> m_to_object_state_e;
	std::map<std::string, object_tag_e> m_to_object_tag_e;
	std::map<std::string, effect_prefix_e> m_to_effect_prefix_e;
	std::map<std::string, action_e> m_to_action_e;

	std::map<interaction_e, std::string> m_effect_string;
	std::map<effect_e, std::string> m_effect_subtype_string;
	std::map<object_tag_e, std::string> m_object_tag_string;
	std::map<effect_prefix_e, std::string> m_effect_prefix_string;

	items_t m_items;
	commands_t m_commands;
	parameters_t m_parameters;

	GameObject* m_object;
	label_t* m_label;
	Effect* m_effect;
	//Interaction_copyist* m_slot_copyist;
	Object_interaction* m_slot;

	std::list<GameObject*> m_objects;

	object_feature_e get_object_feature_e(const std::string& key);
	interaction_e get_interaction_e(const std::string& key);
	effect_e get_effect_e(const std::string& key);
	object_attribute_e get_object_attribute_e(const std::string& key);
	object_state_e get_object_state_e(const std::string& key);
	object_tag_e get_object_tag_e(const std::string& key);
	effect_prefix_e get_effect_prefix_e(const std::string& key);
	action_e get_action_e(const std::string& key);

	void parser(const std::string& command);
	void init();
	GameObject* new_object(std::string unit_name);
	void update_buff();

	std::string get_effect_string(interaction_e key);
	std::string get_effect_subtype_string(effect_e key);
	std::string get_object_tag_string(object_tag_e key);
	std::string get_effect_prefix_string(effect_prefix_e key);
};
