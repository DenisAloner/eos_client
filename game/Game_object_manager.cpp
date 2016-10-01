#include "Game_object_manager.h"
#include "game\impact\Impact_random_value.h"
#include "game\impact\Impact_copy_chance.h"

interaction_e GameObjectManager::get_interaction_e(const std::string& key)
{
	auto value = m_to_interaction_e.find(key);
	if (value == m_to_interaction_e.end())
	{
		LOG(FATAL) << "Элемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

ai_type_e GameObjectManager::get_ai_type_e(const std::string& key)
{
	auto value = m_to_ai_type_e.find(key);
	if (value == m_to_ai_type_e.end())
	{
		LOG(FATAL) << "Элемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

effect_e GameObjectManager::get_effect_e(const std::string& key)
{
	auto value = m_to_effect_e.find(key);
	if (value == m_to_effect_e.end())
	{ 
		LOG(FATAL) << "Элемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

object_state_e GameObjectManager::get_object_state_e(const std::string& key)
{
	auto value = m_to_object_state_e.find(key);
	if (value == m_to_object_state_e.end())
	{
		LOG(FATAL) << "Элемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

//object_parameter_e GameObjectManager::get_object_parameter_e(const std::string& key)
//{
//	auto value = m_to_object_parameter_e.find(key);
//	if (value == m_to_object_parameter_e.end())
//	{
//		LOG(FATAL) << "Элемент `" << key << "` отсутствует в m_items";
//	}
//	return value->second;
//}

void GameObjectManager::parser(const std::string& command)
{
	LOG(INFO) << command;
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
		m_object->m_name = arg[0];
		m_items.insert(std::pair<std::string, GameObject*>(arg[0], m_object));
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
			m_stack_attribute_map.push_front(&obj_state->m_equip);
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

		m_stack_attribute_map.push_front(m_object->m_active_state);
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
			m_ai = ai;
			break;
		}
		case ai_type_e::non_humanoid:
		{
			AI_enemy* ai = new AI_enemy();
			ai->m_ai_type = type;
			ai->m_path_qualifier = Application::instance().m_ai_manager->m_path_qualifiers[std::stoi(arg[1])];
			m_object->m_active_state->m_ai = ai;
			m_ai = ai;
			break;
		}
		}
		break;
	}
	case command_e::size:
	{
		game_object_size_t size;
		size.x = std::stoi(arg[0]);
		size.y = std::stoi(arg[1]);
		size.z = std::stoi(arg[2]);
		m_object->m_active_state->m_size = size;
		m_object->set_direction(object_direction_e::down);
		break;
	}
	case command_e::weight:
	{
		m_object->m_active_state->m_weight = std::stof(arg[0]);
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
		m_object->m_active_state->m_layer = std::stoi(arg[0]);
		break;
	}
	case command_e::icon:
	{
		m_object->m_active_state->m_icon = std::stoul(arg[0]);
		break;
	}
	case command_e::tile_manager:
	{
		m_object->m_active_state->m_tile_manager = Application::instance().m_graph->m_tile_managers[std::stoi(arg[0])];
		break;
	}
	case command_e::light:
	{
		light_t* light = new light_t(0, 0, 0);
		light->R = std::stoi(arg[0]);
		light->G = std::stoi(arg[1]);
		light->B = std::stoi(arg[2]);
		m_object->m_active_state->m_light = light;
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
		item->m_interaction_message_type = interaction_message_type_e::slot_time;
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
	case command_e::list:
	{
		feature_list_type_e list_type = get_feature_list_type_e(arg[1]);
		Interaction_list* list;

		if (arg[0][0] == 'y')
		{
			list = m_stack_attribute_map.front()->create_feature_list(list_type, get_interaction_e(arg[2]));
		}
		else
		{
			list = Effect_functions::create_feature_list(list_type, get_interaction_e(arg[2]));
		}
	
		list->m_list_type = list_type;
		switch (list_type)
		{
		case feature_list_type_e::parameter:
		{
			Parameter_list* parameter_list = static_cast<Parameter_list*>(list);
			parameter_list->m_basic_value = std::stoi(arg[3]);
			parameter_list->m_basic_limit = std::stoi(arg[4]);
			break;
		}
		case feature_list_type_e::vision_component:
		{
			Vision_component* v_list = static_cast<Vision_component*>(list);
			v_list->m_basic_value = AI_FOV(std::stoi(arg[3]), Application::instance().m_ai_manager->m_fov_qualifiers[std::stoi(arg[4])], std::stoi(arg[5]), std::stoi(arg[6]));
			break;
		}
		}
		if (arg[0][1] == 'y')
		{
			m_stack_list.push_front(list);
		}
		break;
	}
	case command_e::pop_list:
	{
		m_stack_list.pop_front();
		break;
	}
	case command_e::pop_map:
	{
		m_stack_attribute_map.pop_front();
		break;
	}
	case command_e::new_template_part:
	{
		Object_part* item = new Object_part();
		item->m_interaction_message_type = interaction_message_type_e::part;
		item->m_part_kind = get_body_part_e(arg[0]);
		item->m_name = arg[1];
		m_stack_attribute_map.push_front(&item->m_object_state);
		m_template_part[arg[2]] = item;
		break;
	}
	case command_e::template_part:
	{
		Object_part* item = m_template_part[arg[0]]->clone();
		m_stack_attribute_map.push_front(&item->m_object_state);
		m_stack_list.front()->add(item);
		break;
	}
	case command_e::part:
	{
		Object_part* item = new Object_part();
		item->m_interaction_message_type = interaction_message_type_e::part;
		item->m_part_kind = get_body_part_e(arg[1]);
		item->m_name = arg[2];
		if (arg[0][0] == 'y')
		{
			m_stack_attribute_map.push_front(&item->m_object_state);
		}
		if (arg[0][1] == 'y')
		{
			m_stack_list.front()->add(item);
		}
		if (arg[0][2] == 'y')
		{
			m_slot = item;
		}
		break;
	}
	case command_e::action:
	{
		action_e action = get_action_e(arg[0]);
		Action* item = Application::instance().m_actions[action];
		m_stack_list.front()->add(item);
		break;
	}
	case command_e::mem_instruction_slot_link:
	{
		Instruction_slot_link* item = new Instruction_slot_link();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_subtype = get_interaction_e(arg[0]);
		item->m_value = m_slot;
		m_slot = item;
		break;
	}
	case command_e::tag:
	{
		Object_tag* tag;
		switch (get_object_tag_e(arg[1]))
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
		case object_tag_e::activator:
		{
			tag = new ObjectTag::Activator();
			break;
		}
		case object_tag_e::fast_move:
		{
			tag = new ObjectTag::Fast_move();
			break;
		}
		case object_tag_e::equippable:
		{
			tag = new ObjectTag::Equippable();
			static_cast<ObjectTag::Equippable*>(tag)->m_value = m_slot;
			static_cast<ObjectTag::Equippable*>(tag)->m_condition = Effect_functions::create_feature_list(feature_list_type_e::generic, interaction_e::action);
			break;
		}
		case object_tag_e::requirements_to_object:
		{
			tag = new ObjectTag::Requirements_to_object();
			static_cast<ObjectTag::Requirements_to_object*>(tag)->m_value = m_slot;
			break;
		}
		default:
		{
			tag = new ObjectTag::Label(get_object_tag_e(arg[1]));
			
			break;
		}
		}
		if (arg[0][0] == 'y')
		{
			m_stack_list.front()->add(tag);
		}
		switch (get_object_tag_e(arg[1]))
		{
		case object_tag_e::equippable:
		{
			m_stack_list.push_front(static_cast<ObjectTag::Equippable*>(tag)->m_condition);
			break;
		}
		}
		if (arg[0][1] == 'y')
		{
			m_slot = tag;
		}
		break;
	}
	case command_e::instruction_check_part_type:
	{
		Instruction_check_part_type* item = new Instruction_check_part_type();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_value = get_body_part_e(arg[0]);
		m_slot = item;
		break;
	}
	case command_e::instruction_check_tag:
	{
		Instruction_check_tag* item = new Instruction_check_tag();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_value = get_object_tag_e(arg[0]);
		m_slot = item;
		break;
	}
	case command_e::slot_to_list:
	{
		m_stack_list.front()->add(m_slot);
		break;
	}
	case command_e::list_to_slot:
	{
		m_slot = m_stack_list.front();
		break;
	}
	}
}

void GameObjectManager::init()
{
	m_commands["object"] = command_e::obj;
	m_commands["size"] = command_e::size;
	m_commands["ai"] = command_e::ai;
	m_commands["weight"] = command_e::weight;
	m_commands["optical"] = command_e::optical;
	m_commands["visibility"] = command_e::visibility;
	m_commands["layer"] = command_e::layer;
	m_commands["icon"] = command_e::icon;
	m_commands["tile_manager"] = command_e::tile_manager;
	m_commands["light"] = command_e::light;
	m_commands["state"] = command_e::state;
	m_commands["mem_effect"] = command_e::mem_effect;
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
	
	m_commands["tag"] = command_e::tag;
	m_commands["list"] = command_e::list;
	m_commands["pop_list"] = command_e::pop_list;
	m_commands["pop_map"] = command_e::pop_map;
	m_commands["part"] = command_e::part;
	m_commands["action"] = command_e::action;
	m_commands["new_template_part"] = command_e::new_template_part;
	m_commands["template_part"] = command_e::template_part;
	m_commands["mem_instruction_slot_link"] = command_e::mem_instruction_slot_link;
	m_commands["instruction_check_part_type"] = command_e::instruction_check_part_type;
	m_commands["instruction_check_tag"] = command_e::instruction_check_tag;
	m_commands["slot_to_list"] = command_e::slot_to_list;
	m_commands["list_to_slot"] = command_e::list_to_slot;

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
	m_to_interaction_e["use"] = interaction_e::use;
	m_to_interaction_e["health"] = interaction_e::health;
	m_to_interaction_e["strength"] = interaction_e::strength;
	m_to_interaction_e["intelligence"] = interaction_e::intelligence;
	m_to_interaction_e["dexterity"] = interaction_e::dexterity;
	m_to_interaction_e["hunger"] = interaction_e::hunger;
	m_to_interaction_e["thirst"] = interaction_e::thirst;
	m_to_interaction_e["poison"] = interaction_e::poison;
	m_to_interaction_e["action"] = interaction_e::action;
	m_to_interaction_e["tag"] = interaction_e::tag;
	m_to_interaction_e["body"] = interaction_e::body;
	m_to_interaction_e["weapon_damage"] = interaction_e::weapon_damage;
	m_to_interaction_e["skill_sword"] = interaction_e::skill_sword;
	m_to_interaction_e["skill_bow"] = interaction_e::skill_bow;
	m_to_interaction_e["strength_bonus"] = interaction_e::strength_bonus;
	m_to_interaction_e["demand_weapon_skill"] = interaction_e::demand_weapon_skill;
	m_to_interaction_e["evasion_skill"] = interaction_e::evasion_skill;
	m_to_interaction_e["weapon_range"] = interaction_e::weapon_range;
	m_to_interaction_e["vision"] = interaction_e::vision;
	m_to_interaction_e["vision_component"] = interaction_e::vision_component;
	m_to_interaction_e["skill_unarmed_combat"] = interaction_e::skill_unarmed_combat;
	m_to_interaction_e["equip"] = interaction_e::equip;

	m_effect_string[interaction_e::total_damage] = "общий дополнительный урон";
	m_effect_string[interaction_e::damage] = "урон";
	m_effect_string[interaction_e::buff] = "баффы";
	m_effect_string[interaction_e::use] = "применение";
	m_effect_string[interaction_e::health] = "здоровье";
	m_effect_string[interaction_e::strength] = "сила";
	m_effect_string[interaction_e::intelligence] = "интеллект";
	m_effect_string[interaction_e::dexterity] = "ловкость";
	m_effect_string[interaction_e::hunger] = "голод";
	m_effect_string[interaction_e::thirst] = "жажда";
	m_effect_string[interaction_e::tag] = "метки";
	m_effect_string[interaction_e::poison] = "яд";
	m_effect_string[interaction_e::action] = "действия";
	m_effect_string[interaction_e::body] = "тело";
	m_effect_string[interaction_e::weapon_damage] = "урон оружия";
	m_effect_string[interaction_e::skill_sword] = "владение мечом";
	m_effect_string[interaction_e::skill_bow] = "владение луком";
	m_effect_string[interaction_e::strength_bonus] = "бонус силы";
	m_effect_string[interaction_e::demand_weapon_skill] = "требование к владению оружием";
	m_effect_string[interaction_e::evasion_skill] = "навык уклонения";
	m_effect_string[interaction_e::weapon_range] = "дальность";
	m_effect_string[interaction_e::vision] = "зрение";
	m_effect_string[interaction_e::vision_component] = "компонент зрения";
	m_effect_string[interaction_e::skill_unarmed_combat] = "владение безоружным боем";

	m_to_effect_e["value"] = effect_e::value;
	m_to_effect_e["limit"] = effect_e::limit;
	m_to_effect_e["start_angle"] = effect_e::start_angle;
	m_to_effect_e["end_angle"] = effect_e::end_angle;

	m_effect_subtype_string[effect_e::value] = "модификатор значения";
	m_effect_subtype_string[effect_e::limit] = "модификатор лимита";
	m_effect_subtype_string[effect_e::start_angle] = "модификатор начального угла обзора";
	m_effect_subtype_string[effect_e::end_angle] = "модификатор конечного угла обзора";

	m_effect_prefix_string[effect_prefix_e::physical_damage] = "физический урон";
	m_effect_prefix_string[effect_prefix_e::poison_damage] = "урон от яда";
	m_effect_prefix_string[effect_prefix_e::state_change] = "изменить состояние";

	m_to_effect_prefix_e["physical_damage"] = effect_prefix_e::physical_damage;
	m_to_effect_prefix_e["poison_damage"] = effect_prefix_e::poison_damage;

	m_to_object_tag_e["poison_resist"] = object_tag_e::poison_resist;
	m_to_object_tag_e["purification_from_poison"] = object_tag_e::purification_from_poison;
	m_to_object_tag_e["mortal"] = object_tag_e::mortal;
	m_to_object_tag_e["pick_able"] = object_tag_e::pick_able;
	m_to_object_tag_e["pass_able"] = object_tag_e::pass_able;
	m_to_object_tag_e["seethrough_able"] = object_tag_e::seethrough_able;
	m_to_object_tag_e["activator"] = object_tag_e::activator;
	m_to_object_tag_e["fast_move"] = object_tag_e::fast_move;
	m_to_object_tag_e["equippable"] = object_tag_e::equippable;
	m_to_object_tag_e["ring"] = object_tag_e::ring;
	m_to_object_tag_e["requirements_to_object"] = object_tag_e::requirements_to_object;
	m_to_object_tag_e["cursed"] = object_tag_e::cursed;

	m_object_tag_string[object_tag_e::poison_resist] = "сопротивление к яду";
	m_object_tag_string[object_tag_e::purification_from_poison] = "очищение от яда";
	m_object_tag_string[object_tag_e::mortal] = "смертное существо";
	m_object_tag_string[object_tag_e::pass_able] = "не является преградой";
	m_object_tag_string[object_tag_e::pick_able] = "можно взять";
	m_object_tag_string[object_tag_e::seethrough_able] = "не загораживает обзор";
	m_object_tag_string[object_tag_e::activator] = "активирует/деактивирует механизмы";
	m_object_tag_string[object_tag_e::fast_move] = "быстрое передвижение";
	m_object_tag_string[object_tag_e::equippable] = "можно одеть";
	m_object_tag_string[object_tag_e::ring] = "кольцо";
	m_object_tag_string[object_tag_e::requirements_to_object] = "требования к предмету";
	m_object_tag_string[object_tag_e::cursed] = "наложено проклятье";

	m_to_action_e["equip"] = action_e::equip;
	m_to_action_e["hit"] = action_e::hit;
	m_to_action_e["hit_melee"] = action_e::hit_melee;
	m_to_action_e["move"] = action_e::move;
	m_to_action_e["move_step"] = action_e::move_step;
	m_to_action_e["open"] = action_e::open;
	m_to_action_e["pick"] = action_e::pick;
	m_to_action_e["push"] = action_e::push;
	m_to_action_e["set_motion_path"] = action_e::set_motion_path;
	m_to_action_e["turn"] = action_e::turn;
	m_to_action_e["use"] = action_e::use;
	m_to_action_e["shoot"] = action_e::shoot;

	m_to_body_part_e["wrist"] = body_part_e::wrist;
	m_to_body_part_e["finger"] = body_part_e::finger;
	m_to_body_part_e["head"] = body_part_e::head;
	m_to_body_part_e["hand"] = body_part_e::hand;
	m_to_body_part_e["foot"] = body_part_e::foot;
	m_to_body_part_e["waist"] = body_part_e::waist;
	m_to_body_part_e["container"] = body_part_e::container;

	m_to_feature_list_type_e["action"] = feature_list_type_e::action;
	m_to_feature_list_type_e["tag"] = feature_list_type_e::tag;
	m_to_feature_list_type_e["generic"] = feature_list_type_e::generic;
	m_to_feature_list_type_e["parameter"] = feature_list_type_e::parameter;
	m_to_feature_list_type_e["parts"] = feature_list_type_e::parts;
	m_to_feature_list_type_e["vision"] = feature_list_type_e::vision;
	m_to_feature_list_type_e["vision_component"] = feature_list_type_e::vision_component;

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
	auto it = m_items.find(unit_name);
	if (it == m_items.end())
	{
		LOG(FATAL) << "Элемент `" << unit_name << "` отсутствует в m_items";
	}
	GameObject* config = m_items.find(unit_name)->second;
	obj->m_direction = config->m_direction;
	obj->m_name = config->m_name;
	Object_state* state;
	for (std::list<Object_state*>::iterator item = config->m_state.begin(); item != config->m_state.end(); ++item)
	{
		state = (*item)->clone();
		obj->m_state.push_back(state);
	}
	obj->m_active_state = obj->m_state.front();
	//register_object(obj);
	bind_body(obj);
	obj->update_interaction();
	Application::instance().m_world->m_object_manager.m_items.push_back(obj);
	return obj;
}

//void GameObjectManager::register_object(GameObject*& object)
//{
//	m_objects.push_back(object);
//	if (object->get_effect(interaction_e::buff))
//	{
//		m_update_buff.push_front(object);
//	}
//}

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
		LOG(FATAL) << "Элемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

effect_prefix_e GameObjectManager::get_effect_prefix_e(const std::string& key)
{
	auto value = m_to_effect_prefix_e.find(key);
	if (value == m_to_effect_prefix_e.end())
	{
		LOG(FATAL) << "Элемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

action_e GameObjectManager::get_action_e(const std::string& key)
{
	auto value = m_to_action_e.find(key);
	if (value == m_to_action_e.end())
	{
		LOG(FATAL) << "Элемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

body_part_e GameObjectManager::get_body_part_e(const std::string& key)
{
	auto value = m_to_body_part_e.find(key);
	if (value == m_to_body_part_e.end())
	{
		LOG(FATAL) << "Элемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

feature_list_type_e GameObjectManager::get_feature_list_type_e(const std::string& key)
{
	auto value = m_to_feature_list_type_e.find(key);
	if (value == m_to_feature_list_type_e.end())
	{
		LOG(FATAL) << "Элемент `" << key << "` отсутствует в m_items";
	}
	return value->second;
}

void GameObjectManager::save()
{
	/*for (auto object = m_objects.begin(); object != m_objects.end(); object++)
	{
		(*object)->save(file);
	}*/
}

void GameObjectManager::load()
{

}

void GameObjectManager::bind_body_predicat(Object_interaction* object, bool add_mode)
{
	if (add_mode)
	{
		if (object->m_interaction_message_type == interaction_message_type_e::part)
		{

			Object_part* part = static_cast<Object_part*>(object);
			LOG(INFO) << "parts: " << part->m_name;
			part->m_owner= m_game_object_owner_stack.front();
			m_game_object_owner_stack.push_front(part);
		}
	}
	else
	{
		if (object->m_interaction_message_type == interaction_message_type_e::part)
		{
			m_game_object_owner_stack.pop_front();
		}
	}
}

void GameObjectManager::bind_body(GameObject* object)
{
	Interaction_list* list;
	for (auto item = object->m_state.begin(); item != object->m_state.end(); ++item)
	{
		Interaction_list* list = (*item)->get_list(interaction_e::body);
		if (list)
		{
			m_game_object_owner_stack.clear();
			m_game_object_owner_stack.push_front(object);
			for (auto item = list->m_effect.begin(); item != list->m_effect.end(); ++item)
			{
				(*item)->do_predicat_ex(std::bind(&GameObjectManager::bind_body_predicat, this, std::placeholders::_1, std::placeholders::_2));
			}
			m_game_object_owner_stack.pop_front();
		}
	}
}