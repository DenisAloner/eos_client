#include "Game_object_manager.h"

object_feature_e GameObjectManager::get_object_feature_e(const std::string& key)
{
	auto value = m_to_object_feature_e.find(key);
	if (value == m_to_object_feature_e.end())
	{
		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

reaction_e GameObjectManager::get_reaction_e(const std::string& key)
{
	auto value = m_to_reaction_e.find(key);
	if (value == m_to_reaction_e.end())
	{
		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

interaction_e GameObjectManager::get_interaction_e(const std::string& key)
{
	auto value = m_to_interaction_e.find(key);
	if (value == m_to_interaction_e.end())
	{
		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

effect_e GameObjectManager::get_effect_e(const std::string& key)
{
	auto value = m_to_effect_e.find(key);
	if (value == m_to_effect_e.end())
	{ 
		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

object_state_e GameObjectManager::get_object_state_e(const std::string& key)
{
	auto value = m_to_object_state_e.find(key);
	if (value == m_to_object_state_e.end())
	{
		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

reaction_applicator_e GameObjectManager::get_reaction_applicator_e(const std::string& key)
{
	auto value = m_to_reaction_applicator_e.find(key);
	if (value == m_to_reaction_applicator_e.end())
	{
		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

object_attribute_e GameObjectManager::get_object_attribute_e(const std::string& key)
{
	auto value = m_to_object_attribute_e.find(key);
	if (value == m_to_object_attribute_e.end())
	{
		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

//object_parameter_e GameObjectManager::get_object_parameter_e(const std::string& key)
//{
//	auto value = m_to_object_parameter_e.find(key);
//	if (value == m_to_object_parameter_e.end())
//	{
//		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
//	}
//	return value->second;
//}

void GameObjectManager::parser(const std::string& command)
{
	std::size_t found = 0;
	std::string args;
	command_e key;
	std::vector<std::string> arg;
	found = command.find(" ");
	if (found != std::string::npos)
	{
		key = m_commands.find(command.substr(0, found))->second;
		args = command.substr(found + 1);
		std::size_t pos=0;
		do
		{
			found = args.find(",", pos);
			if (found != -1)
			{
				arg.push_back(args.substr(pos, found - pos));
				pos = found + 1;
			}
			else break;
		} while (true);
		arg.push_back(args.substr(pos));
	}
	else {
		key = m_commands.find(command)->second;
	}
	switch (key)
	{
	case command_e::label:
	{
		m_label = new label_t(args);
		m_labels[args] = m_label;
		break;
	}
	case command_e::add_to_label:
	{
		m_label->m_stat.push_back(get_object_attribute_e(arg[0]));
		break;
	}
	case command_e::obj:
	{
		m_object = new GameObject();
		m_object->m_name = args;
		m_items.insert(std::pair<std::string, GameObject*>(args, m_object));
		break;
	}
	case command_e::state:
	{
		m_object->m_active_state = new Object_state();
		m_object->m_active_state->m_state = get_object_state_e(arg[0]);
		m_object->m_state.push_back(m_object->m_active_state);
		m_object->m_active_state->m_ai = nullptr;
		break;
	}
	case command_e::add_label:
	{
		m_object->add_label(arg[0]);
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
	case command_e::add_action:
	{
		m_object->m_active_state->m_actions.push_back(Application::instance().m_actions[get_action_e(arg[0])]);
		break;
	}
	case command_e::attribute:
	{
		m_object->add_attribute(get_object_attribute_e(arg[0]));
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
		m_object->m_active_state->m_feature[object_feature_e::container] = new Property_Container(x, y, name);
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
		auto prop =static_cast<Property_body*>(m_object->get_feature(object_feature_e::body));
		if (!prop)
		{
			prop = new Property_body();
			m_object->m_active_state->m_feature[object_feature_e::body] = prop;
		}
		prop->m_item.push_back(part);
		break;
	}
	case command_e::parameter:
	{
		m_object->add_parameter(get_interaction_e(arg[0]), std::stoi(arg[1]), std::stoi(arg[2]));
		break;
	}
	case command_e::effect:
	{
		m_effect = new Effect();
		m_effect->m_kind = get_reaction_e(arg[0]);
		m_effect->m_subtype = get_effect_e(arg[1]);
		m_effect->m_value = std::stoi(arg[2]);
		m_object->add_effect(get_interaction_e(arg[3]), m_effect);
		break;
	}
	case command_e::mem_effect:
	{
		Effect* item = new Effect();
		item->m_kind = get_reaction_e(arg[0]);
		item->m_subtype = get_effect_e(arg[1]);
		item->m_value = std::stoi(arg[2]);
		m_slot = item;
		break;
	}
	case command_e::mem_slot_timer:
	{
		Interaction_timer* item = new Interaction_timer();
		//item->m_kind = get_reaction_e(arg[0]);
		item->m_period = std::stoi(arg[0]);
		item->m_value = m_slot;
		item->m_turn = 0;
		m_slot = item;
		break;
	}
	case command_e::mem_slot_time:
	{
		Interaction_time* item = new Interaction_time();
		item->m_turn = std::stoi(arg[0]);
		item->m_value = m_slot;
		m_slot = item;
		break;
	}
	case command_e::slot_copyist:
	{
		m_slot_copyist = new Interaction_copyist();
		m_slot_copyist->m_kind = get_reaction_e(arg[0]);
		m_slot_copyist->m_subtype = get_interaction_e(arg[1]);
		m_slot_copyist->m_value = m_slot;
		m_object->add_effect(get_interaction_e(arg[2]), m_slot_copyist);
		break;
	}
	case command_e::slot_addon:
	{
		Interaction_addon* item = new Interaction_addon();
		item->m_kind = get_reaction_e(arg[0]);
		item->m_subtype = get_interaction_e(arg[1]);
		item->m_value = m_slot;
		m_object->add_effect(get_interaction_e(arg[2]), item);
		break;
	}
	case command_e::mem_slot_copyist:
	{
		Interaction_copyist* item = new Interaction_copyist();
		item->m_kind = get_reaction_e(arg[0]);
		item->m_subtype = get_interaction_e(arg[1]);
		item->m_value = m_slot;
		m_slot = item;
		break;
	}
	case command_e::mem_slot_addon:
	{
		Interaction_addon* item = new Interaction_addon();
		item->m_kind = reaction_e::none;
		item->m_subtype = get_interaction_e(arg[0]);
		item->m_value = m_slot;
		m_slot = item;
		break;
	}
	case command_e::mem_slot_prefix:
	{
		Interaction_prefix* item = new Interaction_prefix();
		item->m_subtype = get_effect_prefix_e(arg[0]);
		item->m_value = m_slot;
		m_slot = item;
		break;
	}
	case command_e::tag:
	{
		Object_tag* tag;
		switch (get_object_tag_e(arg[0]))
		{
		case object_tag_e::poison_resist:
		{
			tag = new ObjectTag::Poison_resist();
			break;
		}
		case object_tag_e::mortal:
		{
			tag = new ObjectTag::Mortal();
			break;
		}
		case object_tag_e::purification_from_poison:
		{
			tag = new ObjectTag::Purification_from_poison();
			break;
		}
		}
		m_object->add_effect(interaction_e::tag,tag);
		break;
	}
	}
}

void GameObjectManager::init()
{
	m_commands["label"] = command_e::label;
	m_commands["add_to_label"] = command_e::add_to_label;
	m_commands.insert(std::pair<std::string, command_e>("object", command_e::obj));
	m_commands["add_label"] = command_e::add_label;
	m_commands.insert(std::pair<std::string, command_e>("size", command_e::size));
	m_commands.insert(std::pair<std::string, command_e>("ai", command_e::ai));
	m_commands.insert(std::pair<std::string, command_e>("weight", command_e::weight));
	m_commands.insert(std::pair<std::string, command_e>("layer", command_e::layer));
	m_commands.insert(std::pair<std::string, command_e>("icon", command_e::icon));
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_single", command_e::tile_manager_single));
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_map", command_e::tile_manager_map));
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_rotating", command_e::tile_manager_rotating));
	m_commands.insert(std::pair<std::string, command_e>("light", command_e::light));
	m_commands["add_action"] = command_e::add_action;
	m_commands.insert(std::pair<std::string, command_e>("property_container", command_e::property_container));
	m_commands["property_body"] = command_e::property_body;
	m_commands["effect"] = command_e::effect;
	m_commands["attribute"] = command_e::attribute;
	m_commands["parameter"] = command_e::parameter;
	m_commands["state"] = command_e::state;
	m_commands["slot_copyist"] = command_e::slot_copyist;
	m_commands["mem_effect"] = command_e::mem_effect;
	m_commands["mem_slot_timer"] = command_e::mem_slot_timer;
	m_commands["mem_slot_time"] = command_e::mem_slot_time;
	m_commands["mem_slot_prefix"] = command_e::mem_slot_prefix;
	m_commands["mem_slot_copyist"] = command_e::mem_slot_copyist;
	m_commands["mem_slot_addon"] = command_e::mem_slot_addon;
	m_commands["slot_addon"] = command_e::slot_addon;
	m_commands["tag"] = command_e::tag;

	m_to_object_state_e["alive"] = object_state_e::alive;
	m_to_object_state_e["dead"] = object_state_e::dead;
	m_to_object_state_e["on"] = object_state_e::on;
	m_to_object_state_e["off"] = object_state_e::off;
	m_to_object_state_e["equip"] = object_state_e::equip;

	m_to_interaction_e["damage"] = interaction_e::damage;
	m_to_interaction_e["buff"] = interaction_e::buff;
	m_to_interaction_e["health"] = interaction_e::health;
	m_to_interaction_e["strength"] = interaction_e::strength;
	m_to_interaction_e["hunger"] = interaction_e::hunger;
	m_to_interaction_e["thirst"] = interaction_e::thirst;
	m_to_interaction_e["poison"] = interaction_e::poison;
	m_to_interaction_e["action"] = interaction_e::action;

	m_to_effect_e["value"] = effect_e::value;
	m_to_effect_e["limit"] = effect_e::limit;

	m_to_reaction_e["get_damage"] = reaction_e::get_damage;
	m_to_reaction_e["change_health"] = reaction_e::change_health;
	m_to_reaction_e["change_parameter"] = reaction_e::change_parameter;
	m_to_reaction_e["apply_effect"] = reaction_e::apply_effect;

	m_to_reaction_applicator_e["get_damage_basic"] = reaction_applicator_e::get_damage_basic;
	m_to_reaction_applicator_e["change_health_basic"] = reaction_applicator_e::change_health_basic;
	m_to_reaction_applicator_e["add_modificator_generic"] = reaction_applicator_e::add_modificator_generic;
	m_to_reaction_applicator_e["apply_effect_generic"] = reaction_applicator_e::apply_effect_generic;

	m_to_object_attribute_e["pass_able"] = object_attribute_e::pass_able;
	m_to_object_attribute_e["pick_able"] = object_attribute_e::pick_able;

	m_effect_string[interaction_e::damage] = "урон";
	m_effect_string[interaction_e::buff] = "баффы";
	m_effect_string[interaction_e::health] = "здоровье";
	m_effect_string[interaction_e::strength] = "сила";
	m_effect_string[interaction_e::hunger] = "голод";
	m_effect_string[interaction_e::thirst] = "жажда";
	m_effect_string[interaction_e::tag] = "метки";
	m_effect_string[interaction_e::poison] = "€д";
	m_effect_string[interaction_e::action] = "действи€";

	m_effect_subtype_string[effect_e::value] = "модификатор значени€";
	m_effect_subtype_string[effect_e::limit] = "модификатор лимита";

	m_effect_prefix_string[effect_prefix_e::physical_damage] = "физический урон";
	m_effect_prefix_string[effect_prefix_e::poison_damage] = "урон от €да";

	m_to_effect_prefix_e["physical_damage"] = effect_prefix_e::physical_damage;
	m_to_effect_prefix_e["poison_damage"] = effect_prefix_e::poison_damage;

	m_to_object_tag_e["poison_resist"] = object_tag_e::poison_resist;
	m_to_object_tag_e["purification_from_poison"] = object_tag_e::purification_from_poison;
	m_to_object_tag_e["mortal"] = object_tag_e::mortal;

	m_object_tag_string[object_tag_e::poison_resist] = "сопротивление к €ду";
	m_object_tag_string[object_tag_e::purification_from_poison] = "очищение от €да";
	m_object_tag_string[object_tag_e::mortal] = "смертное существо";

	m_to_action_e["equip"] = action_e::equip;
	m_to_action_e["hit"] = action_e::hit;
	m_to_action_e["move"] = action_e::move;
	m_to_action_e["open"] = action_e::open;
	m_to_action_e["pick"] = action_e::pick;
	m_to_action_e["push"] = action_e::push;
	m_to_action_e["set_motion_path"] = action_e::set_motion_path;
	m_to_action_e["turn"] = action_e::turn;

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
		LOG(FATAL) << "Ёлемент `" << unit_name << "` отсутствует в m_items";
	}
	GameObject* config = m_items.find(unit_name)->second;
	obj->m_direction = config->m_direction;
	obj->m_name = config->m_name;
	obj->m_weight = config->m_weight;
	Object_state* state;
	for (std::list<Object_state*>::iterator item = config->m_state.begin(); item != config->m_state.end(); item++)
	{
		state = new Object_state();
		state->m_state = (*item)->m_state;
		state->m_layer = (*item)->m_layer;
		state->m_size = (*item)->m_size;
		state->m_tile_size = (*item)->m_tile_size;
		state->m_tile_manager = (*item)->m_tile_manager;
		state->m_light = (*item)->m_light;
		state->m_icon = (*item)->m_icon;
		state->m_actions = (*item)->m_actions;
		state->m_ai = (*item)->m_ai;
		for (auto prop = (*item)->m_feature.begin(); prop != (*item)->m_feature.end(); prop++)
		{
			state->m_feature[prop->first] = prop->second->clone();
		}
		obj->m_state.push_back(state);
	}
	Reaction* reaction;
	obj->m_active_state = obj->m_state.front();
	m_objects.push_back(obj);
	obj->update_interaction();
	return obj;
}

void GameObjectManager::update_buff()
{
	Interaction_list* list;
	Object_interaction* e;
	for (auto object = m_objects.begin(); object != m_objects.end(); object++)
	{
		list = (*object)->get_effect(interaction_e::buff);
		if (list)
		{
			for (auto buff = list->m_effect.begin(); buff != list->m_effect.end();)
			{
				e = (*buff)->clone();
				e->apply_effect(*object,nullptr);
				if ((*buff)->on_turn())
				{
					buff = list->m_effect.erase(buff);
				}
				else
				{
					++buff;
				}
			}
		}
		(*object)->update_interaction();
		(*object)->event_update(VoidEventArgs());
	}
}

std::string GameObjectManager::get_effect_string(interaction_e key)
{
	auto value = m_effect_string.find(key);
	if (value != m_effect_string.end())
	{
		return value->second;
	}
	return "неизвестный тип";
}

std::string GameObjectManager::get_effect_subtype_string(effect_e key)
{
	auto value = m_effect_subtype_string.find(key);
	if (value != m_effect_subtype_string.end())
	{
		return value->second;
	}
	return "неизвестный тип";
}

std::string GameObjectManager::get_object_tag_string(object_tag_e key)
{
	auto value = m_object_tag_string.find(key);
	if (value != m_object_tag_string.end())
	{
		return value->second;
	}
	return "неизвестный тип";
}

std::string GameObjectManager::get_effect_prefix_string(effect_prefix_e key)
{
	auto value = m_effect_prefix_string.find(key);
	if (value != m_effect_prefix_string.end())
	{
		return value->second;
	}
	return "неизвестный тип";
}

object_tag_e GameObjectManager::get_object_tag_e(const std::string& key)
{
	auto value = m_to_object_tag_e.find(key);
	if (value == m_to_object_tag_e.end())
	{
		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

effect_prefix_e GameObjectManager::get_effect_prefix_e(const std::string& key)
{
	auto value = m_to_effect_prefix_e.find(key);
	if (value == m_to_effect_prefix_e.end())
	{
		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

action_e GameObjectManager::get_action_e(const std::string& key)
{
	auto value = m_to_action_e.find(key);
	if (value == m_to_action_e.end())
	{
		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}