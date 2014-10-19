#pragma once
#include "log.h"
#include <map>
#include "GameObject.h"
#include "Effect.h"

struct label_t;
class Reaction;
class Effect;
class Buff;

class Reaction_manager
{
public:

	typedef std::function<void(Reaction*, GameObject*, Effect*)> func;
	typedef std::list < func > list;

	void get_damage_basic(Reaction* reaction, GameObject* object, Effect* effect);
	void change_health_basic(Reaction* reaction, GameObject* object, Effect* effect);
	void get_buff_basic(Reaction* reaction, GameObject* object, Effect* effect);

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
		state_alive,
		state_dead,
		state_on,
		state_off,
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
		property_permit_move,
		property_permit_pick,
		property_container,
		property_strenght,
		property_health,
		property_body,
		effect,
		reaction,
		buff
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

	std::map<std::string, property_e> m_to_property_e;
	std::map<std::string, reaction_e> m_to_reaction_e;
	std::map<std::string, effect_e> m_to_effect_e;
	std::map<std::string, effect_subtype_e> m_to_effect_subtype_e;
	std::map<std::string, reaction_applicator_e> m_to_reaction_applicator_e;

	items_t m_items;
	commands_t m_commands;
	parameters_t m_parameters;

	GameObject* m_object;
	label_t* m_label;
	Reaction* m_reaction;
	Effect* m_effect;
	Buff* m_buff;

	std::list<GameObject*> m_objects;

	Reaction_manager* m_reaction_manager;

	property_e get_property_e(const std::string& key);
	reaction_e get_reaction_e(const std::string& key);
	effect_e get_effect_e(const std::string& key);
	effect_subtype_e get_effect_subtype_e(const std::string& key);
	reaction_applicator_e get_reaction_applicator_e(const std::string& key);

	void parser(const std::string& command);
	void init();
	GameObject* new_object(std::string unit_name);
	void update_buff();
};
