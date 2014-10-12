#pragma once
#include "log.h"
#include <map>
#include "GameObject.h"

struct label_t;

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
		reaction_get_damage,
		effect_physical_damage
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

	items_t m_items;
	commands_t m_commands;
	parameters_t m_parameters;

	GameObject* m_object;
	label_t* m_label;

	property_e get_property_e(const std::string& key);
	void parser(const std::string& command);

	GameObject* new_object(std::string unit_name);

	void init();

};
