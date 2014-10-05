#pragma once
#include "log.h"
#include <map>
#include "GameObject.h"

class GameObjectManager
{
public:

	enum command_e
	{
		obj,
		state_alive,
		state_dead,
		state_on,
		state_off,
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
		reaction_get_damage,
		effect_physical_damage
	};

	typedef std::map<std::string, GameObject*> items_t;
	typedef std::map<std::string, command_e> commands_t;
	items_t m_items;
	commands_t m_commands;

	GameObject* m_object;

	void parser(const std::string& command);
	GameObject* new_object(std::string unit_name);

	void init();

};
