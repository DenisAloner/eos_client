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


class Config : public iSerializable
{
public:

	std::map<std::string, Object_interaction*> m_templates;
	std::map<std::string, GameObject*> m_items;
	std::map<std::string, GLuint> m_icons;

	void save() override {}
	void load() override {}

	Packer_generic& get_packer() override
	{
		return Packer<Config>::Instance();
	}

	constexpr static auto properties() {
		return std::make_tuple(
			make_property(&Config::m_items, u"items"),
			make_property(&Config::m_templates, u"templates"),
			make_property<Parser::icon_map_t>(&Config::m_icons, u"icons")
		);
	}
};

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
		mem_slot_set_state,
		mem_slot_timer,
		mem_effect,
		mem_slot_time,
		mem_slot_select_location,
		mem_slot_copyist,
		mem_slot_prefix,
		mem_slot_addon,
		mem_slot_mover,
		mem_slot_allocator,
		mem_impact_random_value,
		mem_impact_copy_chance,
		
		list,
		pop_list,
		pop_map,
		part,
		action,
		new_template_part,
		template_part,
		mem_instruction_slot_link,
		tag,
		instruction_check_part_type,
		instruction_check_tag,
		slot_to_list,
		list_to_slot,
		instruction_arg_extract,
		instruction_get_owner,
		instruction_check_owner_type

	};

	enum class parameter_e
	{
		head,
		hand,
		foot
	};

	typedef std::map<std::string, command_e> commands_t;
	typedef std::map<std::string, parameter_e> parameters_t;
	 
	std::map<std::string, effect_prefix_e> m_to_effect_prefix_e;
	std::map<std::string, feature_list_type_e> m_to_feature_list_type_e;

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

	effect_prefix_e get_effect_prefix_e(const std::string& key);

	void parser(const std::string& command);
	void init();
	GameObject* new_object(std::string unit_name);
	//void register_object(GameObject*& object);

	std::string get_effect_prefix_string(effect_prefix_e key);

	void GameObjectManager::bind_body_predicat(Object_interaction* object, bool add_mode);
	void GameObjectManager::bind_body(GameObject* object);

	virtual void save();
	virtual void load();

	static Config m_config;

private:
	std::list<Game_object_owner*> m_game_object_owner_stack;
};
