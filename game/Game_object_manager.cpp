#include "Game_object_manager.h"
#include "game\impact\Impact_random_value.h"
#include "game\impact\Impact_copy_chance.h"

interaction_e GameObjectManager::get_interaction_e(const std::string& key)
{
	auto value = m_to_interaction_e.find(key);
	if (value == m_to_interaction_e.end())
	{
		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

ai_type_e GameObjectManager::get_ai_type_e(const std::string& key)
{
	auto value = m_to_ai_type_e.find(key);
	if (value == m_to_ai_type_e.end())
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
	case command_e::obj:
	{
		m_object = new GameObject();
		m_object->m_name = args;
		m_items.insert(std::pair<std::string, GameObject*>(args, m_object));
		break;
	}
	case command_e::state:
	{  
		object_state_e state = get_object_state_e(arg[0]);
		switch (state)
		{
		case object_state_e::equip:
		{
			Object_state_equip* obj_state = new Object_state_equip();
			obj_state->m_body_part = get_body_part_e(arg[1]);
			m_object->m_active_state = obj_state;
			break;
		}
		default:
		{
			m_object->m_active_state = new Object_state();
			break;
		}
		}
		m_object->m_active_state->m_state = state;
		m_object->m_state.push_back(m_object->m_active_state);
		m_object->m_active_state->m_ai = nullptr;
		break;
	}
	case command_e::ai:
	{
		ai_type_e type = get_ai_type_e(arg[0]);
		switch (type)
		{
		case ai_type_e::trap:
		{
			AI_trap* ai = new AI_trap();
			ai->m_ai_type = type;
			m_object->m_active_state->m_ai = ai;
			break;
		}
		case ai_type_e::non_humanoid:
		{
			AI_enemy* ai = new AI_enemy();
			ai->m_ai_type = type;
			ai->m_fov_radius = std::stoi(arg[1]);
			ai->m_fov_qualifier = Application::instance().m_ai_manager->m_fov_qualifiers[std::stoi(arg[2])];
			ai->m_path_qualifier = Application::instance().m_ai_manager->m_path_qualifiers[std::stoi(arg[3])];
			m_object->m_active_state->m_ai = ai;
			break;
		}
		}
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
	case command_e::optical:
	{
		m_object->m_active_state->m_optical = new optical_properties_t(RGB_t(std::stof(arg[0]), std::stof(arg[1]), std::stof(arg[2])));
		break;
	}
	case command_e::visibility:
	{
		m_object->m_active_state->m_visibility = new float(std::stof(arg[0]));
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
	case command_e::tile_manager_single_animate:
	{
		if (m_object->m_active_state->m_tile_manager == nullptr)
		{
			m_object->m_active_state->m_tile_manager = new TileManager_Single_animate();
		}
		m_object->m_active_state->m_tile_manager->load_from_file(arg[0], ObjectDirection::ObjectDirection_Down, std::stoi(arg[1]));
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
		action_e action = get_action_e(arg[0]);
		switch (action)
		{
		//case action_e::hit:
		//{
		//	Action_hit* a = new Action_hit();
		//	m_object->m_active_state->m_actions.push_back(a);
		//	break;
		//}
		default:
		{
			m_object->add_effect(interaction_e::action, Application::instance().m_actions[action]);
			break;
		}
		}
		break;
	}
	case command_e::add_slot:
	{
		m_object->add_effect(get_interaction_e(arg[0]), m_slot);
		break;
	}
	case command_e::add_slot_mem:
	{
		m_mem_state->add_effect(get_interaction_e(arg[0]), m_slot);
		break;
	}
	case command_e::mem_action:
	{
		action_e action = get_action_e(arg[0]);
		Action* item = Application::instance().m_actions[action];
		m_slot = item;
		break;
	}
	case command_e::mem_part:
	{
		Object_part* item = new Object_part();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_part_kind = get_body_part_e(arg[0]);
		item->m_name = arg[1];
		m_slot = item;
		m_mem_state = item->m_object_state;
		break;
	}
	case command_e::mem_slot_set_state:
	{
		Slot_set_state* item = new Slot_set_state();
		item->m_value = get_object_state_e(arg[0]);
		m_slot = item;
		break;
	}
	case command_e::mem_slot_select_location:
	{
		Slot_select_cell* item = new Slot_select_cell();
		item->m_value = arg[0];
		m_slot = item;
		break;
	}
	case command_e::mem_slot_mover:
	{
		Slot_mover* item = new Slot_mover();
		item->m_value = static_cast<Slot_allocator*>(m_slot);
		m_slot = item;
		break;
	}
	case command_e::mem_slot_allocator:
	{
		Slot_allocator* item = new Slot_allocator();
		m_slot = item;
		break;
	}
	case command_e::mem_effect:
	{
		Effect* item = new Effect();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_subtype = get_effect_e(arg[0]);
		item->m_value = std::stoi(arg[1]);
		m_slot = item;
		break;
	}
	case command_e::mem_impact_random_value:
	{
		Impact_random_value* item = new Impact_random_value();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_subtype = get_effect_e(arg[0]);
		item->m_value = std::stoi(arg[1]);
		item->m_min_value = std::stoi(arg[2]);
		m_slot = item;
		break;
	}
	case command_e::mem_impact_copy_chance:
	{
		Impact_copy_chance* item = new Impact_copy_chance();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_chance = std::stoi(arg[0]);
		item->m_value = m_slot;
		m_slot = item;
		break;
	}
	case command_e::mem_slot_timer:
	{
		Interaction_timer* item = new Interaction_timer();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_period = std::stoi(arg[0]);
		item->m_value = m_slot;
		item->m_turn = 0;
		m_slot = item;
		break;
	}
	case command_e::mem_slot_time:
	{
		Interaction_time* item = new Interaction_time();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_turn = std::stoi(arg[0]);
		item->m_value = m_slot;
		m_slot = item;
		break;
	}
	case command_e::mem_slot_copyist:
	{
		Interaction_copyist* item = new Interaction_copyist();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_subtype = get_interaction_e(arg[0]);
		item->m_value = m_slot;
		m_slot = item;
		break;
	}
	case command_e::mem_slot_addon:
	{
		Interaction_addon* item = new Interaction_addon();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_subtype = get_interaction_e(arg[0]);
		item->m_value = m_slot;
		m_slot = item;
		break;
	}
	case command_e::mem_slot_prefix:
	{
		Interaction_prefix* item = new Interaction_prefix();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_subtype = get_effect_prefix_e(arg[0]);
		item->m_value = m_slot;
		m_slot = item;
		break;
	}
	case command_e::feature_list:
	{
		feature_list_type_e list_type = get_feature_list_type_e(arg[0]);
		Interaction_list* list = m_object->create_feature_list(list_type, get_interaction_e(arg[1]));
		switch (list_type)
		{
		case feature_list_type_e::parameter:
			{
				Parameter_list* parameter_list = static_cast<Parameter_list*>(list);
				parameter_list->m_basic_value = std::stoi(arg[2]);
				parameter_list->m_basic_limit = std::stoi(arg[3]);
				break;
			}
		}
		break;
	}
	case command_e::feature_list_mem:
	{
		feature_list_type_e list_type = get_feature_list_type_e(arg[0]);
		Interaction_list* list= m_mem_state->create_feature_list(list_type, get_interaction_e(arg[1]));
		switch (list_type)
		{
		case feature_list_type_e::parameter:
		{
			Parameter_list* parameter_list = static_cast<Parameter_list*>(list);
			parameter_list->m_basic_value = std::stoi(arg[2]);
			parameter_list->m_basic_limit = std::stoi(arg[3]);
			break;
		}
		}
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
		default:
		{
			tag = new ObjectTag::Label(get_object_tag_e(arg[0]));
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
	m_commands.insert(std::pair<std::string, command_e>("object", command_e::obj));
	m_commands.insert(std::pair<std::string, command_e>("size", command_e::size));
	m_commands.insert(std::pair<std::string, command_e>("ai", command_e::ai));
	m_commands.insert(std::pair<std::string, command_e>("weight", command_e::weight));
	m_commands["optical"] = command_e::optical;
	m_commands["visibility"] = command_e::visibility;
	m_commands.insert(std::pair<std::string, command_e>("layer", command_e::layer));
	m_commands.insert(std::pair<std::string, command_e>("icon", command_e::icon));
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_single", command_e::tile_manager_single));
	m_commands["tile_manager_single_animate"] = command_e::tile_manager_single_animate;
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_map", command_e::tile_manager_map));
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_rotating", command_e::tile_manager_rotating));
	m_commands.insert(std::pair<std::string, command_e>("light", command_e::light));
	m_commands["add_action"] = command_e::add_action;
	m_commands["add_slot"] = command_e::add_slot;
	m_commands["add_slot_mem"] = command_e::add_slot_mem;
	m_commands["state"] = command_e::state;
	m_commands["mem_effect"] = command_e::mem_effect;
	m_commands["mem_part"] = command_e::mem_part;
	m_commands["mem_slot_timer"] = command_e::mem_slot_timer;
	m_commands["mem_slot_set_state"] = command_e::mem_slot_set_state;
	m_commands["mem_slot_time"] = command_e::mem_slot_time;
	m_commands["mem_slot_prefix"] = command_e::mem_slot_prefix;
	m_commands["mem_slot_copyist"] = command_e::mem_slot_copyist;
	m_commands["mem_slot_addon"] = command_e::mem_slot_addon;
	m_commands["mem_slot_select_location"] = command_e::mem_slot_select_location;
	m_commands["mem_slot_mover"] = command_e::mem_slot_mover;
	m_commands["mem_slot_allocator"] = command_e::mem_slot_allocator;
	m_commands["mem_impact_random_value"] = command_e::mem_impact_random_value;
	m_commands["mem_impact_copy_chance"] = command_e::mem_impact_copy_chance;
	m_commands["mem_action"] = command_e::mem_action;
	m_commands["tag"] = command_e::tag;
	m_commands["feature_list"] = command_e::feature_list;
	m_commands["feature_list_mem"] = command_e::feature_list_mem;

	m_to_object_state_e["alive"] = object_state_e::alive;
	m_to_object_state_e["dead"] = object_state_e::dead;
	m_to_object_state_e["on"] = object_state_e::on;
	m_to_object_state_e["off"] = object_state_e::off;
	m_to_object_state_e["equip"] = object_state_e::equip;
	m_to_object_state_e["growth_01"] = object_state_e::growth_01;
	m_to_object_state_e["growth_02"] = object_state_e::growth_02;
	m_to_object_state_e["growth_03"] = object_state_e::growth_03;
	m_to_object_state_e["growth_04"] = object_state_e::growth_04;
	m_to_object_state_e["growth_05"] = object_state_e::growth_05;
	m_to_object_state_e["growth_06"] = object_state_e::growth_06;

	m_to_interaction_e["total_damage"] = interaction_e::total_damage;
	m_to_interaction_e["damage"] = interaction_e::damage;
	m_to_interaction_e["buff"] = interaction_e::buff;
	m_to_interaction_e["health"] = interaction_e::health;
	m_to_interaction_e["strength"] = interaction_e::strength;
	m_to_interaction_e["hunger"] = interaction_e::hunger;
	m_to_interaction_e["thirst"] = interaction_e::thirst;
	m_to_interaction_e["poison"] = interaction_e::poison;
	m_to_interaction_e["action"] = interaction_e::action;
	m_to_interaction_e["tag"] = interaction_e::tag;
	m_to_interaction_e["body"] = interaction_e::body;
	m_to_interaction_e["weapon_damage"] = interaction_e::weapon_damage;
	m_to_interaction_e["skill_sword"] = interaction_e::skill_sword;
	m_to_interaction_e["strength_bonus"] = interaction_e::strength_bonus;
	m_to_interaction_e["demand_weapon_skill"] = interaction_e::demand_weapon_skill;

	m_effect_string[interaction_e::total_damage] = "общий дополнительный урон";
	m_effect_string[interaction_e::damage] = "урон";
	m_effect_string[interaction_e::buff] = "баффы";
	m_effect_string[interaction_e::health] = "здоровье";
	m_effect_string[interaction_e::strength] = "сила";
	m_effect_string[interaction_e::hunger] = "голод";
	m_effect_string[interaction_e::thirst] = "жажда";
	m_effect_string[interaction_e::tag] = "метки";
	m_effect_string[interaction_e::poison] = "€д";
	m_effect_string[interaction_e::action] = "действи€";
	m_effect_string[interaction_e::body] = "тело";
	m_effect_string[interaction_e::weapon_damage] = "урон оружи€";
	m_effect_string[interaction_e::skill_sword] = "владение мечом";
	m_effect_string[interaction_e::strength_bonus] = "бонус силы";
	m_effect_string[interaction_e::demand_weapon_skill] = "требование к владению оружием";

	m_to_effect_e["value"] = effect_e::value;
	m_to_effect_e["limit"] = effect_e::limit;

	m_effect_subtype_string[effect_e::value] = "модификатор значени€";
	m_effect_subtype_string[effect_e::limit] = "модификатор лимита";

	m_effect_prefix_string[effect_prefix_e::physical_damage] = "физический урон";
	m_effect_prefix_string[effect_prefix_e::poison_damage] = "урон от €да";

	m_to_effect_prefix_e["physical_damage"] = effect_prefix_e::physical_damage;
	m_to_effect_prefix_e["poison_damage"] = effect_prefix_e::poison_damage;

	m_to_object_tag_e["poison_resist"] = object_tag_e::poison_resist;
	m_to_object_tag_e["purification_from_poison"] = object_tag_e::purification_from_poison;
	m_to_object_tag_e["mortal"] = object_tag_e::mortal;
	m_to_object_tag_e["pick_able"] = object_tag_e::pick_able;
	m_to_object_tag_e["pass_able"] = object_tag_e::pass_able;
	m_to_object_tag_e["seethrough_able"] = object_tag_e::seethrough_able;

	m_object_tag_string[object_tag_e::poison_resist] = "сопротивление к €ду";
	m_object_tag_string[object_tag_e::purification_from_poison] = "очищение от €да";
	m_object_tag_string[object_tag_e::mortal] = "смертное существо";
	m_object_tag_string[object_tag_e::pass_able] = "не €вл€етс€ преградой";
	m_object_tag_string[object_tag_e::pick_able] = "можно вз€ть";
	m_object_tag_string[object_tag_e::seethrough_able] = "не загораживает обзор";

	m_to_action_e["equip"] = action_e::equip;
	m_to_action_e["hit"] = action_e::hit;
	m_to_action_e["hit_melee"] = action_e::hit_melee;
	m_to_action_e["move"] = action_e::move;
	m_to_action_e["open"] = action_e::open;
	m_to_action_e["pick"] = action_e::pick;
	m_to_action_e["push"] = action_e::push;
	m_to_action_e["set_motion_path"] = action_e::set_motion_path;
	m_to_action_e["turn"] = action_e::turn;

	m_to_body_part_e["head"] = body_part_e::head;
	m_to_body_part_e["hand"] = body_part_e::hand;
	m_to_body_part_e["foot"] = body_part_e::foot;

	m_to_feature_list_type_e["action"] = feature_list_type_e::action;
	m_to_feature_list_type_e["tag"] = feature_list_type_e::tag;
	m_to_feature_list_type_e["generic"] = feature_list_type_e::generic;
	m_to_feature_list_type_e["parameter"] = feature_list_type_e::parameter;
	m_to_feature_list_type_e["parts"] = feature_list_type_e::parts;

	m_to_ai_type_e["non_humanoid"] = ai_type_e::non_humanoid;
	m_to_ai_type_e["trap"] = ai_type_e::trap;

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
		state = (*item)->clone();
		obj->m_state.push_back(state);
	}
	obj->m_active_state = obj->m_state.front();
	m_objects.push_back(obj);
	obj->update_interaction();
	return obj;
}

void GameObjectManager::update_buff()
{
	Interaction_list* list;
	Object_interaction* e;
	/*Application::instance().console(std::to_string(m_objects.size()));
	Application::instance().console(std::to_string(Application::instance().m_GUI->MapViewer->m_map->m_lights.size()));
	Application::instance().console(std::to_string(Application::instance().m_GUI->MapViewer->m_map->m_ai.size()));*/
	for (auto object = m_objects.begin(); object != m_objects.end(); object++)
	{
		list =static_cast<Interaction_list*>((*object)->get_effect(interaction_e::buff));
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

void GameObjectManager::calculate_ai()
{
	for (auto object = m_objects.begin(); object != m_objects.end(); object++)
	{
		if (((*object)->m_active_state->m_ai) && ((*object)!=Application::instance().m_GUI->MapViewer->m_player->m_object))
		{
			switch ((*object)->m_active_state->m_ai->m_ai_type)
			{
			case ai_type_e::trap:
			{
				AI_trap obj;
				obj = *static_cast<AI_trap*>((*object)->m_active_state->m_ai);
				obj.m_object = (*object);
				obj.create();
				break;
			}
			case ai_type_e::non_humanoid:
			{
				AI_enemy obj;
				obj = *static_cast<AI_enemy*>((*object)->m_active_state->m_ai);
				obj.m_map = Application::instance().m_GUI->MapViewer->m_map;
				obj.m_object = (*object);
				obj.create();
				break;
			}
			}
		}
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

body_part_e GameObjectManager::get_body_part_e(const std::string& key)
{
	auto value = m_to_body_part_e.find(key);
	if (value == m_to_body_part_e.end())
	{
		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

feature_list_type_e GameObjectManager::get_feature_list_type_e(const std::string& key)
{
	auto value = m_to_feature_list_type_e.find(key);
	if (value == m_to_feature_list_type_e.end())
	{
		LOG(FATAL) << "Ёлемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}