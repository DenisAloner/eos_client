#pragma once
#include "log.h"
#include <map>
#include "GameObject.h"
#include "Effect.h"
#include "game\impact\forwards.h"


class Effect;
class Buff;
class Object_interaction;
class Interaction_slot;
class Interaction_copyist;
class Object_tag;
class Attribute_map;

class GameObjectManager:public iSerializable
{
public:

	enum class command_e
	{ 
		obj,
		state,
		ai,
		size,
		weight,
		optical,
		visibility,
		layer,
		icon,
		tile_manager,
		light,
		add_action,
		feature_list,
		feature_list_mem,
		mem_slot_set_state,
		mem_part,
		mem_slot_timer,
		mem_effect,
		mem_slot_time,
		mem_slot_select_location,
		mem_slot_copyist,
		mem_slot_prefix,
		mem_slot_addon,
		mem_slot_mover,
		mem_slot_allocator,
		mem_action,
		mem_impact_random_value,
		mem_impact_copy_chance,
		add_slot,
		add_slot_mem,
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
	 
	std::map<std::string, interaction_e> m_to_interaction_e;
	std::map<std::string, effect_e> m_to_effect_e;
	std::map<std::string, object_state_e> m_to_object_state_e;
	std::map<std::string, object_tag_e> m_to_object_tag_e;
	std::map<std::string, effect_prefix_e> m_to_effect_prefix_e;
	std::map<std::string, action_e> m_to_action_e;
	std::map<std::string, body_part_e> m_to_body_part_e;
	std::map<std::string, feature_list_type_e> m_to_feature_list_type_e;
	std::map<std::string, ai_type_e> m_to_ai_type_e;

	std::map<interaction_e, std::string> m_effect_string;
	std::map<effect_e, std::string> m_effect_subtype_string;
	std::map<object_tag_e, std::string> m_object_tag_string;
	std::map<effect_prefix_e, std::string> m_effect_prefix_string;

	items_t m_items;
	commands_t m_commands;
	parameters_t m_parameters;

	GameObject* m_object;
	Effect* m_effect;
	Object_interaction* m_slot;
	Attribute_map* m_mem_state;

	std::list<GameObject*> m_objects;
	std::list<GameObject*> m_update_buff;

	interaction_e get_interaction_e(const std::string& key);
	effect_e get_effect_e(const std::string& key);
	object_state_e get_object_state_e(const std::string& key);
	object_tag_e get_object_tag_e(const std::string& key);
	effect_prefix_e get_effect_prefix_e(const std::string& key);
	action_e get_action_e(const std::string& key);
	body_part_e get_body_part_e(const std::string& key);
	feature_list_type_e get_feature_list_type_e(const std::string& key);
	ai_type_e get_ai_type_e(const std::string& key);

	void parser(const std::string& command);
	void init();
	GameObject* new_object(std::string unit_name);
	void update_buff();

	std::string get_effect_string(interaction_e key);
	std::string get_effect_subtype_string(effect_e key);
	std::string get_object_tag_string(object_tag_e key);
	std::string get_effect_prefix_string(effect_prefix_e key);

	void calculate_ai(GameMap* game_map);

	virtual void save(FILE* file);
	virtual void load(FILE* file);
};
