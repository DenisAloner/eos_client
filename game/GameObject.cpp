#include "game/GameObject.h"
#include "game/Action.h"
#include "log.h"
#include "AI.h"

Attribute_map::Attribute_map(){};

Interaction_list*  Attribute_map::create_feature_list(feature_list_type_e key, interaction_e name)
{
	Interaction_list* result=nullptr;
	switch (key)
		{
		case feature_list_type_e::generic:
		{
			result = new Interaction_list();
			break;
		}
		case feature_list_type_e::tag:
		{
			result = new Tag_list();
			break;
		}
		case feature_list_type_e::action:
		{
			result = new Action_list();
			break;
		}
		case feature_list_type_e::parameter:
		{
			result = new Parameter_list(name);
			break;
		}
		case feature_list_type_e::parts:
		{
			result = new Parts_list();
			break;
		}
		}
	m_item[name] = result;	
	return result;
}

void Attribute_map::add_effect(interaction_e key, Object_interaction* item)
{
	if (m_item.find(key) != m_item.end())
	{
		m_item[key]->add(item);
	}
}

Attribute_map* Attribute_map::clone()
{
	Attribute_map* result = new Attribute_map();
	for (auto item = m_item.begin(); item != m_item.end(); item++)
	{
		result->m_item[item->first] = item->second->clone();
	}
	return result;
}

Object_state::Object_state()
{
	m_layer = 1;
	m_light = nullptr;
	m_tile_manager = nullptr;
	m_icon = Application::instance().m_graph->m_no_image;
	m_ai = nullptr;
	m_optical = nullptr;
	m_visibility = nullptr;
	m_weight = 0.0F;
}

Object_state* Object_state::clone()
{
	Object_state* state = new Object_state();
	state->m_state = m_state;
	state->m_layer = m_layer;
	state->m_size = m_size;
	state->m_tile_size = m_tile_size;
	state->m_tile_manager = m_tile_manager;
	state->m_light = m_light;
	state->m_icon = m_icon;
	state->m_weight = m_weight;
	if (m_ai)
	{
		state->m_ai = m_ai->clone();
	}
	else state->m_ai = nullptr;
	if (m_optical)
	{
		state->m_optical = new optical_properties_t(*m_optical);
	}
	if (m_visibility)
	{
		state->m_visibility = new float;
		*state->m_visibility = *m_visibility;
	}
	for (auto item = m_item.begin(); item != m_item.end(); item++)
	{
		state->m_item[item->first] = item->second->clone();
	}
	return state;
}

Object_state_equip::Object_state_equip()
{
	m_state = object_state_e::equip;
}

Object_state* Object_state_equip::clone()
{
	Object_state_equip* state = new Object_state_equip();
	state->m_body_part = m_body_part;
	state->m_state = m_state;
	state->m_layer = m_layer;
	state->m_size = m_size;
	state->m_tile_size = m_tile_size;
	state->m_tile_manager = m_tile_manager;
	state->m_light = m_light;
	state->m_icon = m_icon;
	if (m_ai)
	{
		state->m_ai = m_ai->clone();
	}
	else state->m_ai = nullptr;
	
	for (auto item = m_item.begin(); item != m_item.end(); item++)
	{
		state->m_item[item->first] = item->second->clone();
	}
	return state;
}

//Action* Object_state::find_action(action_e kind)
//{
//	for (std::list<Action*>::iterator Current = m_actions.begin(); Current != m_actions.end(); Current++)
//	{
//		if ((*Current)->m_kind == kind)
//		{
//			return (*Current);
//		}
//	}
//	return nullptr;
//}

void Object_state::set_tile_size()
{
	m_tile_size = dimension_t(tile_size_x_half*(m_size.x + m_size.y), tile_size_y_half*(m_size.x + m_size.y) + m_size.z * 18);
}

void Object_state::save(FILE* file)
{
	size_t s = m_item.size();
	fwrite(&s, sizeof(size_t), 1, file);
	for (auto item = m_item.begin(); item != m_item.end(); item++)
	{
		fwrite(&((*item).first), sizeof(interaction_e), 1, file);
		(*item).second->save(file);
	}
	fwrite(&m_state, sizeof(object_state_e), 1, file);
	fwrite(&m_layer, sizeof(int), 1, file);
	fwrite(&m_size, sizeof(game_object_size_t), 1, file);
	fwrite(&m_weight, sizeof(float), 1, file);
	FileSystem::instance().serialize_pointer(m_light, type_e::light_t, file);
	FileSystem::instance().serialize_pointer(m_optical, type_e::optical_properties_t, file);
	fwrite(&m_tile_manager->m_index, sizeof(size_t), 1, file);
	FileSystem::instance().serialize_AI(m_ai, file);
}

void Object_state::load(FILE* file)
{
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	m_item.clear();
	interaction_e ie;
	for (size_t i = 0; i < s; i++)
	{
		fread(&ie, sizeof(interaction_e), 1, file);
		m_item[ie] = static_cast<Interaction_list*>(FileSystem::instance().deserialize_impact(file));
	}
	fread(&m_state, sizeof(object_state_e), 1, file);
	fread(&m_layer, sizeof(int), 1, file);
	fread(&m_size, sizeof(game_object_size_t), 1, file);
	fread(&m_weight, sizeof(float), 1, file);
	m_light = static_cast<light_t*>(FileSystem::instance().deserialize_pointer(file));
	m_optical = static_cast<optical_properties_t*>(FileSystem::instance().deserialize_pointer(file));
	fread(&s, sizeof(size_t), 1, file);
	m_tile_manager = Application::instance().m_graph->m_tile_managers[s];
	m_ai=FileSystem::instance().deserialize_AI(file);
}


Game_object_owner::Game_object_owner(){}

MapCell::MapCell(int x, int y, GameMap* map) :x(x), y(y), m_map(map)
{
	m_kind = entity_e::cell;
}

void MapCell::add_object(GameObject* Object)
{
	m_items.push_back(Object);
}

//Object_feature* MapCell::find_property(property_e kind, GameObject* excluded)
//{
//	Object_feature* result(nullptr);
//	for (std::list<GameObject*>::iterator item = m_items.begin(); item != m_items.end(); item++)
//	{
//		if (excluded != (*item))
//		{
//			result = (*item)->m_active_state->find_property(kind);
//			if (result != nullptr)
//			{
//				return result;
//			}
//		}
//	}
//	return nullptr;
//}

GameObject::GameObject()
{
	m_direction = object_direction_e::down;
	m_selected = false;
	m_active_state = nullptr;
	//rendering_necessary = false;
}

bool GameObject::on_turn()
{
	LOG(FATAL)<<"Метод не описан.";
	return false;
}

std::string GameObject::get_description()
{
	LOG(FATAL) << "Метод не описан.";
	return "";
}

Object_interaction* GameObject::clone()
{
	LOG(FATAL) << "Метод не описан.";
	return nullptr;
}

void GameObject::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<" + m_name + ">");
}

void GameObject::do_predicat(predicat func)
{ 
	func(this); 
	LOG(FATAL) << "Метод не описан.";
};

void GameObject::turn()
{
	/*object_direction_e dir;
	if (m_direction == object_direction_e_Down)
	{
		dir = object_direction_e_Left;
	}
	if (m_direction == object_direction_e_Left)
	{
		dir = object_direction_e_Up;
	}
	if (m_direction == object_direction_e_Up)
	{
		dir = object_direction_e_Right;
	}
	if (m_direction == object_direction_e_Right)
	{
		dir = object_direction_e_Down;
	}
	set_direction(dir);*/
}

void GameObject::set_direction(object_direction_e dir)
{
	/*if (((m_direction == object_direction_e_Left || m_direction == object_direction_e_Right) && (dir == object_direction_e_Up || dir == object_direction_e_Down)) || ((dir == object_direction_e_Left || dir == object_direction_e_Right) && (m_direction == object_direction_e_Up || m_direction == object_direction_e_Down)))
	{
		for (std::list<Object_state*>::iterator item = m_state.begin(); item != m_state.end(); item++)
		{
			(*item)->m_size = game_object_size_t((*item)->m_size.y, (*item)->m_size.x, (*item)->m_size.z);
		}
	}*/
	for (std::list<Object_state*>::iterator item = m_state.begin(); item != m_state.end(); item++)
	{
		(*item)->set_tile_size();
	}
	m_direction = dir;
}

void GameObject::set_state(object_state_e state)
{
	for (std::list<Object_state*>::iterator item = m_state.begin(); item != m_state.end(); item++)
	{
		if ((*item)->m_state == state)
		{
			m_active_state = (*item);
			return;
		}
	}
}

Object_state* GameObject::get_state(object_state_e state)
{
	for (std::list<Object_state*>::iterator item = m_state.begin(); item != m_state.end(); item++)
	{
		if ((*item)->m_state == state)
		{
			return (*item);
		}
	}
	return nullptr;
}

Interaction_list* GameObject::create_feature_list(feature_list_type_e key, interaction_e name)
{
	Interaction_list* result;

	switch (key)
	{
	case feature_list_type_e::generic:
	{
		result = new Interaction_list();
		break;
	}
	case feature_list_type_e::tag:
	{
		result = new Tag_list();
		break;
	}
	case feature_list_type_e::action:
	{
		result = new Action_list();
		break;
	}
	case feature_list_type_e::parameter:
	{
		result = new Parameter_list(name);
		break;
	}
	case feature_list_type_e::parts:
	{
		result = new Parts_list();
		break;
	}
	}
	m_active_state->m_item[name] = result;
	return result;
}

void GameObject::add_effect(interaction_e key, Object_interaction* item)
{
	if (m_active_state->m_item.find(key) != m_active_state->m_item.end())
	{
		m_active_state->m_item[key]->add(item);
	}
}

void GameObject::add_from(interaction_e key, Interaction_list* feature)
{
	if (m_active_state)
	{
		if (m_active_state->m_item.find(key) == m_active_state->m_item.end())
		{
			m_active_state->m_item[key] = feature;
		}
		else
		{
			m_active_state->m_item[key]->add(feature);
		}
	}
}

void GameObject::remove_effect(interaction_e key, Object_interaction* item)
{
	if (m_active_state)
	{
		if (m_active_state->m_item.find(key) != m_active_state->m_item.end())
		{
			m_active_state->m_item[key]->remove(item);
		}
	}
}

void GameObject::remove_from(interaction_e key, Interaction_list* feature)
{
	if (m_active_state)
	{
		auto item = m_active_state->m_item.find(key);
		if (item != m_active_state->m_item.end())
		{
			if (item->second == feature)
			{
				m_active_state->m_item.erase(key);
			}
			else
			{
				m_active_state->m_item[key]->remove(feature);
			}
		}
	}
}

Interaction_list* GameObject::get_effect(interaction_e key)
{
	if (m_active_state)
	{
		auto value = m_active_state->m_item.find(key);
		if (value != m_active_state->m_item.end())
		{
			return value->second;
		}
	}
	return nullptr;
}

Parameter_list* GameObject::get_parameter(interaction_e key)
{
	if (m_active_state)
	{
		auto value = m_active_state->m_item.find(key);
		if (value != m_active_state->m_item.end())
		{
			return static_cast<Parameter_list*>(value->second);
		}
	}
	return nullptr;
}

bool GameObject::get_stat(object_tag_e key)
{
	if (m_active_state)
	{
		auto list = m_active_state->m_item.find(interaction_e::tag);
		if (list != m_active_state->m_item.end())
		{
			Tag_list* taglist = static_cast<Tag_list*>(list->second);
			for (auto item = taglist->m_effect.begin(); item != taglist->m_effect.end(); item++)
			{
				if (static_cast<Object_tag*>(*item)->m_type == key){ return true; }
			}
		}
	}
	return false;
}

Object_tag* GameObject::get_tag(object_tag_e key)
{
	if (m_active_state)
	{
		auto list = m_active_state->m_item.find(interaction_e::tag);
		if (list != m_active_state->m_item.end())
		{
			Tag_list* taglist = static_cast<Tag_list*>(list->second);
			for (auto item = taglist->m_effect.begin(); item != taglist->m_effect.end(); item++)
			{
				if (static_cast<Object_tag*>(*item)->m_type == key){ return static_cast<Object_tag*>(*item); }
			}
		}
	}
	return nullptr;
}

MapCell* GameObject::cell(){
	return static_cast<MapCell*>(m_owner);
}

void GameObject::update_interaction()
{
	if (m_active_state)
	{
		for (auto item = m_active_state->m_item.begin(); item != m_active_state->m_item.end(); item++)
		{
			item->second->update();
		}
	}
}


void GameObject::save(FILE* file)
{
	FileSystem::instance().serialize_string(m_name, file);
	fwrite(&m_direction, sizeof(object_direction_e), 1, file);
	size_t s = m_state.size();
	fwrite(&s, sizeof(size_t), 1, file);
	for (auto item = m_state.begin(); item != m_state.end(); item++)
	{
		(*item)->save(file);
	}
}

void GameObject::load(FILE* file)
{
	FileSystem::instance().deserialize_string(m_name, file);
	fread(&m_direction, sizeof(object_direction_e), 1, file);
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	Object_state* value;
	for (size_t i = 0; i < s; i++)
	{
		value = new Object_state();
		value->load(file);
		m_state.push_back(value);
	}
	m_active_state = m_state.front();
	set_direction(m_direction);
	update_interaction();

}

Player::Player(GameObject* object, GameMap* map) :m_object(object), m_map(map)
{
	m_fov = new FOV();
	m_fov->calculate(static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_fov_radius, m_object, m_map);
	m_actions.push_front(Application::instance().m_actions[action_e::open_inventory]);
	m_actions.push_front(Application::instance().m_actions[action_e::cell_info]);
	m_actions.push_front(Application::instance().m_actions[action_e::show_parameters]);
}

Inventory_cell::Inventory_cell(GameObject* item = nullptr) : m_item(item)
{
	m_kind = entity_e::inventory_cell;
}

Object_part::Object_part(GameObject* item) :Inventory_cell(item)
{
	m_interaction_message_type = interaction_message_type_e::part;
	m_kind = entity_e::body_part;
	m_object_state = new Attribute_map();
};

Object_part* Object_part::clone()
{
	Object_part* effect = new Object_part();
	//effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_kind = m_kind;
	effect->m_name = m_name;
	effect->m_part_kind = m_part_kind;
	effect->m_item = nullptr;
	effect->m_object_state = m_object_state->clone();
	return effect;
}

bool Object_part::on_turn()
{
	return false;
}

std::string Object_part::get_description()
{
	return "slot";
}

void Object_part::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<" + m_name + ">:");
	info->push_back(std::string(level, '.') + "<эффекты>:");
	for (auto current = m_object_state->m_item.begin(); current != m_object_state->m_item.end(); current++)
	{
		info->push_back(std::string(level + 1, '.') + Application::instance().m_game_object_manager->get_effect_string(current->first) + ":");
		current->second->description(info, level + 2);
	}
}

void Object_part::do_predicat(predicat func)
{ 
	func(this);
	for (auto item = m_object_state->m_item.begin(); item != m_object_state->m_item.end(); item++)
	{
		item->second->do_predicat(func);
	}
}

void Object_part::save(FILE* file)
{
}

void Object_part::load(FILE* file)
{
}
