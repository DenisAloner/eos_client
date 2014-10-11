#include "Game_object_manager.h"

void GameObjectManager::parser(const std::string& command)
{
	std::size_t found = 0;
	std::string args;
	command_e key;
	found = command.find(" ");
	if (found != std::string::npos)
	{
		//MessageBox(NULL, command.substr(0, found).c_str(), "", MB_OK);
		key = m_commands.find(command.substr(0, found))->second;
		args = command.substr(found + 1);
	}
	else {
		key = m_commands.find(command)->second;
	}
	switch (key)
	{
	case command_e::obj:
	{
		m_object = new GameObject();
		m_object->m_name = args;
		m_items.insert(std::pair<std::string, GameObject*>(args, m_object));
		break;
	}
	case command_e::state_alive:
	{
		m_object->m_active_state = new Game_state();
		m_object->m_active_state->m_state = state_e::alive;
		m_object->m_state.push_back(m_object->m_active_state);
		break;
	}
	case command_e::state_dead:
	{
		m_object->m_active_state = new Game_state();
		m_object->m_active_state->m_state = state_e::dead;
		m_object->m_state.push_back(m_object->m_active_state);
		break;
	}
	case command_e::state_on:
	{
		m_object->m_active_state = new Game_state();
		m_object->m_active_state->m_state = state_e::on;
		m_object->m_state.push_back(m_object->m_active_state);
		break;
	}
	case command_e::state_off:
	{
		m_object->m_active_state = new Game_state();
		m_object->m_active_state->m_state = state_e::off;
		m_object->m_state.push_back(m_object->m_active_state);
		break;
	}
	case command_e::ai:
	{
		AI_configuration* ai = new AI_configuration;
		std::size_t pos = 0;
		found = args.find(" ");
		ai->m_fov_radius = std::stoi(args.substr(0, found));
		pos = found + 1;
		found = args.find(" ", pos);
		ai->m_fov_qualifier = Application::instance().m_ai_manager->m_fov_qualifiers[std::stoi(args.substr(pos, found - pos))];
		pos = found + 1;
		ai->m_path_qualifier = Application::instance().m_ai_manager->m_path_qualifiers[std::stoi(args.substr(pos))];
		m_object->m_active_state->m_ai = ai;
		break;
	}
	case command_e::size:
	{
		std::size_t pos = 0;
		game_object_size_t size;
		found = args.find(" ");
		size.x = std::stoi(args.substr(0, found));
		pos = found + 1;
		found = args.find(" ", pos);
		size.y = std::stoi(args.substr(pos, found - pos));
		pos = found + 1;
		size.z = std::stoi(args.substr(pos));
		m_object->m_active_state->m_size = size;
		m_object->set_direction(ObjectDirection_Down);
		break;
	}
	case command_e::weight:
	{
		m_object->m_weight = std::stof(args);
		break;
	}
	case command_e::layer:
	{
		m_object->m_active_state->m_layer = std::stoi(args);
		break;
	}
	case command_e::icon:
	{
		m_object->m_active_state->m_icon = Application::instance().m_graph->load_texture(FileSystem::instance().m_resource_path + "Tiles\\" + args + ".bmp");
		break;
	}
	case command_e::tile_manager_single:
	{
		m_object->m_active_state->m_tile_manager = new TileManager_Single();
		m_object->m_active_state->m_tile_manager->load_from_file(args, ObjectDirection_Down, 0);
		break;
	}
	case command_e::tile_manager_map:
	{
		if (m_object->m_active_state->m_tile_manager == nullptr)
		{
			m_object->m_active_state->m_tile_manager = new TileManager_Map();
		}
		ObjectDirection dir;
		int frame;
		std::string name;
		std::size_t pos = 0;
		found = args.find(" ");
		name = args.substr(0, found);
		pos = found + 1;
		found = args.find(" ", pos);
		dir = static_cast<ObjectDirection>(std::stoi(args.substr(pos, found - pos)));
		pos = found + 1;
		frame = std::stoi(args.substr(pos));
		m_object->m_active_state->m_tile_manager->load_from_file(name, dir, frame);
		break;
	}
	case command_e::tile_manager_rotating:
	{
		if (m_object->m_active_state->m_tile_manager == nullptr)
		{
			m_object->m_active_state->m_tile_manager = new TileManager_rotating();
		}
		ObjectDirection dir;
		std::string name;
		std::size_t pos = 0;
		found = args.find(" ");
		name = args.substr(0, found);
		pos = found + 1;
		found = args.find(" ", pos);
		dir = static_cast<ObjectDirection>(std::stoi(args.substr(pos, found - pos)));
		m_object->m_active_state->m_tile_manager->load_from_file(name, dir, 0);
		break;
	}
	case command_e::light:
	{
		std::size_t pos = 0;
		light_t* light = new light_t(0, 0, 0);
		found = args.find(" ");
		light->R = std::stoi(args.substr(0, found));
		pos = found + 1;
		found = args.find(" ", pos);
		light->G = std::stoi(args.substr(pos, found - pos));
		pos = found + 1;
		light->B = std::stoi(args.substr(pos));
		m_object->m_active_state->m_light = light;
		break;
	}
	case command_e::action_move:
	{
		m_object->m_active_state->m_actions.push_back(Application::instance().m_actions[action_e::move]);
		break;
	}
	case command_e::property_permit_move:
	{
		m_object->m_active_state->m_properties.push_back(new GameObjectProperty(property_e::permit_move));
		break;
	}
	case command_e::property_permit_pick:
	{
		m_object->m_active_state->m_properties.push_back(new GameObjectProperty(property_e::permit_pick));
		break;
	}
	case command_e::property_container:
	{
		std::size_t pos = 0;
		std::string name;
		int x;
		int y;
		found = args.find(" ");
		name = args.substr(0, found);
		pos = found + 1;
		found = args.find(" ", pos);
		x = std::stoi(args.substr(pos, found - pos));
		pos = found + 1;
		y = std::stoi(args.substr(pos));
		m_object->m_active_state->m_properties.push_back(new Property_Container(x, y, name));
		break;
	}
	case command_e::property_body:
	{
		std::size_t pos = 0;
		Body_part part;
		found = args.find(",");
		auto kind = m_parameters.find(args.substr(0, found));
		if (kind != m_parameters.end())
		{
			switch (kind->second)
			{
			case parameter_e::head:
			{
				part.m_part_kind = body_part_e::head;
				break;
			}
			case parameter_e::hand:
			{
				part.m_part_kind = body_part_e::hand;
				break;
			}
			case parameter_e::foot:
			{
				part.m_part_kind = body_part_e::foot;
				break;
			}
			}
		}
		pos = found + 1;
		part.m_name = args.substr(pos);
		Property_body* prop = static_cast<Property_body*>(m_object->m_active_state->find_property(property_e::body));
		if (!prop)
		{
			prop = new Property_body();
			m_object->m_active_state->m_properties.push_back(prop);
			LOG(INFO) << "Есть такое свойство";
		}
		prop->m_item.push_back(part);
		break;
	}
	case command_e::property_strenght:
	{
		m_object->m_active_state->m_properties.push_back(new GameObjectParameter(property_e::strength, std::stof(args)));
		break;
	}
	case command_e::property_health:
	{
		m_object->m_active_state->m_properties.push_back(new Game_object_feature(property_e::health, std::stoi(args)));
		break;
	}
	case command_e::reaction_get_damage:
	{
		Reaction_subtype* r;
		r = new Reaction_subtype();
		r->m_applicator = Application::instance().m_reaction_manager->m_items[reaction_applicator_e::get_damage];
		m_object->m_reaction[reaction_e::get_damage] = r;
		Reaction_mods* r1 = new Reaction_mods();
		r1->m_applicator = Application::instance().m_reaction_manager->m_items[reaction_applicator_e::change_health];
		m_object->m_reaction[reaction_e::change_health] = r1;
		break;
	}
	case command_e::effect_physical_damage:
	{
		Effect* e = new Effect();
		e->m_kind = reaction_e::get_damage;
		e->m_subtype = effect_subtype_e::physical_damage;
		e->m_value = std::stof(args);
		m_object->m_effect[effect_e::damage].push_back(e);
		break;
	}
	}
}

void GameObjectManager::init()
{
	m_commands.insert(std::pair<std::string, command_e>("object", command_e::obj));
	m_commands.insert(std::pair<std::string, command_e>("state_alive", command_e::state_alive));
	m_commands.insert(std::pair<std::string, command_e>("state_dead", command_e::state_dead));
	m_commands.insert(std::pair<std::string, command_e>("state_on", command_e::state_on));
	m_commands.insert(std::pair<std::string, command_e>("state_off", command_e::state_off));
	m_commands.insert(std::pair<std::string, command_e>("size", command_e::size));
	m_commands.insert(std::pair<std::string, command_e>("ai", command_e::ai));
	m_commands.insert(std::pair<std::string, command_e>("weight", command_e::weight));
	m_commands.insert(std::pair<std::string, command_e>("layer", command_e::layer));
	m_commands.insert(std::pair<std::string, command_e>("icon", command_e::icon));
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_single", command_e::tile_manager_single));
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_map", command_e::tile_manager_map));
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_rotating", command_e::tile_manager_rotating));
	m_commands.insert(std::pair<std::string, command_e>("light", command_e::light));
	m_commands.insert(std::pair<std::string, command_e>("action_move", command_e::action_move));
	m_commands.insert(std::pair<std::string, command_e>("property_permit_move", command_e::property_permit_move));
	m_commands.insert(std::pair<std::string, command_e>("property_permit_pick", command_e::property_permit_pick));
	m_commands.insert(std::pair<std::string, command_e>("property_container", command_e::property_container));
	m_commands.insert(std::pair<std::string, command_e>("property_strength", command_e::property_strenght));
	m_commands.insert(std::pair<std::string, command_e>("property_health", command_e::property_health));
	m_commands["property_body"] = command_e::property_body;
	m_commands.insert(std::pair<std::string, command_e>("reaction_get_damage", command_e::reaction_get_damage));
	m_commands.insert(std::pair<std::string, command_e>("effect_physical_damage", command_e::effect_physical_damage));

	bytearray buffer;
	FileSystem::instance().load_from_file(FileSystem::instance().m_resource_path + "Configs\\Objects.txt", buffer);
	std::string config(buffer);
	std::size_t pos = 0;
	std::size_t found = 0;
	while (pos != std::string::npos)
	{
		found = config.find("\r\n", pos);
		if (found != std::string::npos)
		{
			if (found - pos > 0)
			{
				parser(config.substr(pos, found - pos));

			}
			pos = found + 2;
		}
		else {
			parser(config.substr(pos));
			pos = std::string::npos;
		}
	}

};

GameObject* GameObjectManager::new_object(std::string unit_name)
{
	GameObject* obj = new GameObject();
	items_t::iterator it = m_items.find(unit_name);
	if (it == m_items.end())
	{
		LOG(FATAL) << "Элемент `" << unit_name << "` отсутствует в m_items";
	}
	GameObject* config = m_items.find(unit_name)->second;
	obj->m_direction = config->m_direction;
	obj->m_name = config->m_name;
	obj->m_weight = config->m_weight;
	Game_state* state;
	for (std::list<Game_state*>::iterator item = config->m_state.begin(); item != config->m_state.end(); item++)
	{
		state = new Game_state();
		state->m_state = (*item)->m_state;
		state->m_layer = (*item)->m_layer;
		state->m_size = (*item)->m_size;
		state->m_tile_size = (*item)->m_tile_size;
		state->m_tile_manager = (*item)->m_tile_manager;
		state->m_light = (*item)->m_light;
		state->m_icon = (*item)->m_icon;
		state->m_actions = (*item)->m_actions;
		state->m_ai = (*item)->m_ai;
		for (auto prop = (*item)->m_properties.begin(); prop != (*item)->m_properties.end(); prop++)
		{
			state->m_properties.push_back((*prop)->clone());
		}
		obj->m_state.push_back(state);
	}
	Reaction* reaction;
	for (auto item = config->m_reaction.begin(); item != config->m_reaction.end(); item++)
	{
		obj->m_reaction[item->first] = item->second->clone();
	}
	obj->m_active_state = obj->m_state.front();
	obj->m_reaction = config->m_reaction;
	obj->m_effect = config->m_effect;
	return obj;
}
