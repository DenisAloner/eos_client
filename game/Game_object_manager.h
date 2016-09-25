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
class Interaction_list;
class Game_object_owner;

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
		copy_list_to_slot,
		add_slot_to_mem_list,
		mem_list,
		create_list,
		add_part,
		stack_list_push,
		stack_list_pop,
		stack_attribute_map_pop,
		part,
		action,
		new_template_part,
		template_part,
		mem_instruction_slot_link,
		stack_tag,
		instruction_check_part_type,
		instruction_check_tag,
		slot_to_list,
		list_to_slot
	};

	enum class parameter_e
	{
		head,
		hand,
		foot
	};

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

	std::map<std::string, GameObject*> m_items;
	commands_t m_commands;
	parameters_t m_parameters;

	GameObject* m_object;
	Effect* m_effect;
	Object_interaction* m_slot;
	Attribute_map* m_mem_state;
	AI* m_ai;
	Interaction_list* m_current_list;
	Interaction_list* m_mem_list;
	Object_part* m_part;

	std::list<Interaction_list*> m_stack_list;
	std::list<Attribute_map*> m_stack_attribute_map;

	std::map<std::string, Object_part*> m_template_part;

	//std::list<GameObject*> m_object_templates;

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
	//void register_object(GameObject*& object);

	std::string get_effect_string(interaction_e key);
	std::string get_effect_subtype_string(effect_e key);
	std::string get_object_tag_string(object_tag_e key);
	std::string get_effect_prefix_string(effect_prefix_e key);

	void GameObjectManager::bind_body_predicat(Object_interaction* object, bool add_mode);
	void GameObjectManager::bind_body(GameObject* object);

	virtual void save();
	virtual void load();

private:
	std::list<Game_object_owner*> m_game_object_owner_stack;
};
