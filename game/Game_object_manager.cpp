#include "Game_object_manager.h"
#include "game\impact\Impact_random_value.h"
#include "game\impact\Impact_copy_chance.h"


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
	case command_e::state:
	{  
		object_state_e state = Parser::m_json_object_state_e.get_enum(arg[0]);
		switch (state)
		{
		case object_state_e::equip:
		{
			Object_state_equip* obj_state = new Object_state_equip();
			obj_state->m_body_part = Parser::m_json_body_part_e.get_enum(arg[1]);
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
		ai_type_e type = Parser::m_json_ai_type_e.get_enum(arg[0]);
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
		//m_object->m_active_state->m_icon = std::stoul(arg[0]);
		break;
	}
	case command_e::tile_manager:
	{
		//m_object->m_active_state->m_tile_manager = Application::instance().m_graph->m_tile_managers[std::stoi(arg[0])];
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
		item->m_value = Parser::m_json_object_state_e.get_enum(arg[0]);
		m_slot = item;
		break;
	}
	case command_e::mem_effect:
	{
		Effect* item = new Effect();
		item->m_subtype = Parser::m_json_effect_e.get_enum(arg[0]);
		item->m_value = std::stoi(arg[1]);
		m_slot = item;
		break;
	}
	case command_e::mem_impact_random_value:
	{
		Impact_random_value* item = new Impact_random_value();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_subtype = Parser::m_json_effect_e.get_enum(arg[0]);
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
		item->m_subtype = Parser::m_json_interaction_e.get_enum(arg[0]);
		item->m_value = m_slot;
		m_slot = item;
		break;
	}
	case command_e::mem_slot_addon:
	{
		Interaction_addon* item = new Interaction_addon();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_subtype = Parser::m_json_interaction_e.get_enum(arg[0]);
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
		feature_list_type_e list_type = Parser::m_json_feature_list_type_e.get_enum(arg[1]);
		Interaction_list* list;

		if (arg[0][0] == 'y')
		{
			list = m_stack_attribute_map.front()->create_feature_list(list_type, Parser::m_json_interaction_e.get_enum(arg[2]));
		}
		else
		{
			list = Effect_functions::create_feature_list(list_type, Parser::m_json_interaction_e.get_enum(arg[2]));
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
	case command_e::template_part:
	{
		Object_part* item = m_template_part[arg[0]]->clone();
		m_stack_attribute_map.push_front(&item->m_attributes);
		m_stack_list.front()->add(item);
		break;
	}
	case command_e::action:
	{
		action_e::type action = Parser::m_json_action_e.get_enum(arg[0]);
		Action* item = Application::instance().m_actions[action];
		m_stack_list.front()->add(item);
		break;
	}
	case command_e::mem_instruction_slot_link:
	{
		Instruction_slot_link* item = new Instruction_slot_link();
		item->m_subtype = Parser::m_json_interaction_e.get_enum(arg[0]);
		item->m_value = m_slot;
		m_slot = item;
		break;
	}
	case command_e::tag:
	{
		Object_tag* tag;
		switch (Parser::m_json_object_tag.get_enum(arg[1]))
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
		case object_tag_e::can_transfer_object:
		{
			tag = new ObjectTag::Can_transfer_object();
			static_cast<ObjectTag::Can_transfer_object*>(tag)->m_value = m_slot;
			break;
		}
		default:
		{
			tag = new ObjectTag::Label(Parser::m_json_object_tag.get_enum(arg[1]));
			
			break;
		}
		}
		if (arg[0][0] == 'y')
		{
			m_stack_list.front()->add(tag);
		}
		switch (Parser::m_json_object_tag.get_enum(arg[1]))
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
		item->m_value = Parser::m_json_body_part_e.get_enum(arg[0]);
		m_slot = item;
		break;
	}
	case command_e::instruction_check_tag:
	{
		Instruction_check_tag* item = new Instruction_check_tag();
		item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_value = Parser::m_json_object_tag.get_enum(arg[0]);
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
	case command_e::instruction_arg_extract:
	{
		Instruction_arg_extract* item = new Instruction_arg_extract();
		//item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_value = m_slot;
		item->m_index = std::stoi(arg[0]);
		m_slot = item;
		break;
	}
	case command_e::instruction_get_owner:
	{
		Instruction_get_owner* item;
		if(arg[0]=="top")
		{
			item = new Instruction_get_owner_top();
		}
		else
		{
			item = new Instruction_get_owner();
			
		}
		item->m_value = m_slot;
		m_slot = item;
		break;
	}
	case command_e::instruction_check_owner_type:
	{
		Instruction_check_owner_type* item = new Instruction_check_owner_type();
		//item->m_interaction_message_type = interaction_message_type_e::single;
		item->m_value = Parser::m_json_entity_e.get_enum(arg[0]);
		m_slot = item;
		break;
	}
	}
}

Config GameObjectManager::m_config = {};

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
	m_commands["instruction_arg_extract"] = command_e::instruction_arg_extract;
	m_commands["instruction_get_owner"] = command_e::instruction_get_owner;
	m_commands["instruction_check_owner_type"] = command_e::instruction_check_owner_type;


	m_effect_prefix_string[effect_prefix_e::physical_damage] = "физический урон";
	m_effect_prefix_string[effect_prefix_e::poison_damage] = "урон от яда";
	m_effect_prefix_string[effect_prefix_e::state_change] = "изменить состояние";

	m_to_effect_prefix_e["physical_damage"] = effect_prefix_e::physical_damage;
	m_to_effect_prefix_e["poison_damage"] = effect_prefix_e::poison_damage;

	
	bytearray json;
	FileSystem::instance().load_from_file(FileSystem::instance().m_resource_path + "Configs\\Objects.json", json);
	std::u16string json_config(json);


	std::u16string temp(json_config);
	scheme_map_t* s = Parser::read_object(temp);
	//Config* tempcfg = new Config;
	m_config.from_json(s);
	delete s;

	m_items.insert(m_config.m_items.begin(),m_config.m_items.end());

	/*bytearray buffer;
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
	}*/

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
	obj->set_direction(obj->m_direction);
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

std::string GameObjectManager::get_effect_prefix_string(effect_prefix_e key)
{
	auto value = m_effect_prefix_string.find(key);
	if (value != m_effect_prefix_string.end())
	{
		return value->second;
	}
	return "неизвестный тип";
}

void Config::instancedictonary_icon_from_json(std::u16string value, InstanceDictonary<Icon*>& prop)
{
	std::u16string temp = value;
	scheme_list_t* s = Parser::read_array(temp);
	if (s)
	{
		for (auto element : (*s))
		{
			std::string&& name = Parser::UTF16_to_CP866(Parser::get_value(element));
			Icon* icon = new Icon;
			icon->m_value = Application::instance().m_graph->load_texture(FileSystem::instance().m_resource_path + "Tiles\\" + name + ".bmp");
			prop.add(icon, name);
		}
		delete s;
	}
}

void Config::instancedictonary_icon_from_binary(const std::string& value, InstanceDictonary<Icon*>& prop, std::size_t& pos)
{
}

void Config::instancedictonary_tilemanager_from_json(std::u16string value, InstanceDictonary<TileManager*>& prop)
{
	std::u16string temp = value;
	scheme_list_t* s = Parser::read_array(temp);
	std::string k;
	TileManager* v = nullptr;
	if (s)
	{
		for (auto element : (*s))
		{
			scheme_vector_t* p = Parser::read_pair(element);
			Parser::from_json<std::string>((*p)[0], k);
			Parser::from_json<TileManager*>((*p)[1], v);
			prop.add(v, k);
		}
		delete s;
	}
}

void Config::instancedictonary_tilemanager_from_binary(const std::string& value, InstanceDictonary<TileManager*>& prop, std::size_t& pos)
{
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


void GameObjectManager::bind_body(GameObject* object)
{
	Interaction_list* list;
	for (auto item = object->m_state.begin(); item != object->m_state.end(); ++item)
	{
		Interaction_list* list = (*item)->get_list(interaction_e::body);
		if (list)
		{
			Visitor_part_hierarchy_setter setter;
			setter.m_game_object_owner_stack.push_front(object);
			list->apply_visitor(setter);
		}
	}
}

void Visitor_part_hierarchy_setter::visit(Object_part& value)
{
	value.m_owner = m_game_object_owner_stack.front();
	m_game_object_owner_stack.push_front(&value);
	Visitor_simple::visit(value);
	m_game_object_owner_stack.pop_front();
}
