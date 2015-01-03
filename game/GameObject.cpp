#include "game/GameObject.h"
#include "game/Action.h"
#include "log.h"


Object_state::Object_state()
{
	m_layer = 1;
	m_light = nullptr;
	m_tile_manager = nullptr;
	m_icon = Application::instance().m_graph->m_no_image;
	m_ai = nullptr;
}

Action* Object_state::find_action(action_e kind)
{
	for (std::list<Action*>::iterator Current = m_actions.begin(); Current != m_actions.end(); Current++)
	{
		if ((*Current)->m_kind == kind)
		{
			return (*Current);
		}
	}
	return nullptr;
}

void Object_state::set_tile_size()
{
	m_tile_size = dimension_t(m_size.x, m_size.y + m_size.z);
}

Game_object_owner::Game_object_owner(){}

MapCell::MapCell(int x, int y) :x(x), y(y){
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
	m_direction = ObjectDirection_Down;
	m_selected = false;
	m_active_state = nullptr;
	m_weight = 0.0;
}

void GameObject::turn()
{
	ObjectDirection dir;
	if (m_direction == ObjectDirection_Down)
	{
		dir = ObjectDirection_Left;
	}
	if (m_direction == ObjectDirection_Left)
	{
		dir = ObjectDirection_Up;
	}
	if (m_direction == ObjectDirection_Up)
	{
		dir = ObjectDirection_Right;
	}
	if (m_direction == ObjectDirection_Right)
	{
		dir = ObjectDirection_Down;
	}
	set_direction(dir);
}

void GameObject::set_direction(ObjectDirection dir)
{
	if (((m_direction == ObjectDirection_Left || m_direction == ObjectDirection_Right) && (dir == ObjectDirection_Up || dir == ObjectDirection_Down)) || ((dir == ObjectDirection_Left || dir == ObjectDirection_Right) && (m_direction == ObjectDirection_Up || m_direction == ObjectDirection_Down)))
	{
		for (std::list<Object_state*>::iterator item = m_state.begin(); item != m_state.end(); item++)
		{
			(*item)->m_size = game_object_size_t((*item)->m_size.y, (*item)->m_size.x, (*item)->m_size.z);
		}
	}
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

Object_feature* GameObject::get_feature(object_feature_e key)
{
	if (m_active_state)
	{
		auto value = m_active_state->m_feature.find(key);
		if (value != m_active_state->m_feature.end())
		{
			return value->second;
		}
	}
	return nullptr;
}

void GameObject::add_attribute(object_attribute_e key)
{
	Attribute_feature* list = static_cast<Attribute_feature*>(get_feature(object_feature_e::attribute_feature));
	if (list)
	{
		list->m_stat.push_back(key);
	}
	else
	{
		if (m_active_state)
		{
			list = new Attribute_feature();
			list->m_stat.push_back(key);
			m_active_state->m_feature[object_feature_e::attribute_feature] = list;
		}
	}
}

void GameObject::add_label(const std::string& key)
{
	Attribute_feature* list = static_cast<Attribute_feature*>(get_feature(object_feature_e::attribute_feature));
	if (list)
	{
		auto value = Application::instance().m_game_object_manager->m_labels.find(key);
		if (value != Application::instance().m_game_object_manager->m_labels.end())
		{
			list->m_label.push_back(value->second);
		}
	}
	else
	{
		if (m_active_state)
		{
			auto value = Application::instance().m_game_object_manager->m_labels.find(key);
			if (value != Application::instance().m_game_object_manager->m_labels.end())
			{
				list = new Attribute_feature();
				list->m_label.push_back(value->second);
				m_active_state->m_feature[object_feature_e::attribute_feature] = list;
			}
		}
	}
}

void GameObject::add_effect(interaction_e key, Object_interaction* item)
{
	Interaction_feature* list = static_cast<Interaction_feature*>(get_feature(object_feature_e::interaction_feature));
	if (list)
	{
		if (list->m_effect.find(key) == list->m_effect.end())
		{
			list->m_effect[key] = new Interaction_list;
		}
		static_cast<Interaction_list*>(list->m_effect[key])->m_effect.push_back(item);
	}
	else
	{
		if (m_active_state)
		{
			list = new Interaction_feature();
			list->m_effect[key] = new Interaction_list();
			static_cast<Interaction_list*>(list->m_effect[key])->m_effect.push_back(item);
			m_active_state->m_feature[object_feature_e::interaction_feature] = list;
		}
	}
}

void GameObject::add_tag(Object_interaction* item)
{
	Interaction_feature* list = static_cast<Interaction_feature*>(get_feature(object_feature_e::interaction_feature));
	if (list)
	{
		if (list->m_effect.find(interaction_e::tag) == list->m_effect.end())
		{
			list->m_effect[interaction_e::tag] = new Tag_list;
		}
		static_cast<Interaction_list*>(list->m_effect[interaction_e::tag])->m_effect.push_back(item);
	}
	else
	{
		if (m_active_state)
		{
			list = new Interaction_feature();
			list->m_effect[interaction_e::tag] = new Interaction_list();
			static_cast<Interaction_list*>(list->m_effect[interaction_e::tag])->m_effect.push_back(item);
			m_active_state->m_feature[object_feature_e::interaction_feature] = list;
		}
	}
}

void GameObject::add_parameter(interaction_e key, int value, int limit)
{
	Interaction_feature* list = static_cast<Interaction_feature*>(get_feature(object_feature_e::interaction_feature));
	if (list)
	{
		Parameter_list* pl = new Parameter_list(key);
		pl->m_basic_value = value;
		pl->m_basic_limit = limit;
		list->m_effect[key] = pl;
	}
	else
	{
		if (m_active_state)
		{
			list = new Interaction_feature();
			Parameter_list* pl = new Parameter_list(key);
			pl->m_basic_value = value;
			pl->m_basic_limit = limit;
			list->m_effect[key] = pl;
			m_active_state->m_feature[object_feature_e::interaction_feature] = list;
		}
	}
}

void GameObject::add_to_parameter(interaction_e key, Object_interaction* item)
{
	Interaction_feature* list = static_cast<Interaction_feature*>(get_feature(object_feature_e::interaction_feature));
	if (list)
	{
		if (list->m_effect.find(key) == list->m_effect.end())
		{
			list->m_effect[key] = new Parameter_list(key);
		}
		static_cast<Parameter_list*>(list->m_effect[key])->m_effect.push_back(item);
	}
	else
	{
		if (m_active_state)
		{
			list = new Interaction_feature();
			list->m_effect[key] = new Parameter_list(key);
			static_cast<Parameter_list*>(list->m_effect[key])->m_effect.push_back(item);
			m_active_state->m_feature[object_feature_e::interaction_feature] = list;
		}
	}
}

void GameObject::remove_effect(interaction_e key, Object_interaction* item)
{
	Interaction_feature* list = static_cast<Interaction_feature*>(get_feature(object_feature_e::interaction_feature));
	if (list)
	{
		if (list->m_effect.find(key) != list->m_effect.end())
		{
			static_cast<Interaction_list*>(list->m_effect[key])->m_effect.remove(item);
		}
	}
}

Interaction_list* GameObject::get_effect(interaction_e key)
{
	Interaction_feature* list = static_cast<Interaction_feature*>(get_feature(object_feature_e::interaction_feature));
	if (list)
	{
		auto value = list->m_effect.find(key);
		if (value != list->m_effect.end())
		{
			return value->second;
		}
	}
	return nullptr;
}

Parameter_list* GameObject::get_parameter(interaction_e key)
{
	Interaction_feature* list = static_cast<Interaction_feature*>(get_feature(object_feature_e::interaction_feature));
	if (list)
	{
		auto value = list->m_effect.find(key);
		if (value != list->m_effect.end())
		{
			return static_cast<Parameter_list*>(value->second);
		}
	}
	return nullptr;
}

bool GameObject::get_stat(object_attribute_e key)
{
	Attribute_feature* list = static_cast<Attribute_feature*>(get_feature(object_feature_e::attribute_feature));
	if (list)
	{
		for (auto item = list->m_stat.begin(); item != list->m_stat.end();item++)
		{
			if (*item == key)
			{
				return true;
			}
		}
		for (auto label = list->m_label.begin(); label != list->m_label.end(); label++)
		{
			for (auto stat = (*label)->m_stat.begin(); stat != (*label)->m_stat.end(); stat++)
			{
				if (*stat == key)
				{
					return true;
				}
			}
		}
	}
	return false;
}

MapCell* GameObject::cell(){
	return static_cast<MapCell*>(m_owner);
}

void GameObject::update_interaction()
{
	Interaction_feature* list = static_cast<Interaction_feature*>(get_feature(object_feature_e::interaction_feature));
	if (list)
	{
		for (auto item = list->m_effect.begin(); item != list->m_effect.end(); item++)
		{
			item->second->update();
		}
	}
}

Player::Player(GameObject* object, GameMap* map) :m_object(object), m_map(map)
{
	m_fov = new FOV();
	m_fov->calculate(object->m_active_state->m_ai->m_fov_radius, m_object, m_map);
}

Object_feature::Object_feature(object_feature_e kind) : m_kind(kind)
{
}

Object_feature* Object_feature::clone()
{
	Object_feature* result = new Object_feature(object_feature_e::none);
	result->m_kind = m_kind;
	return result;
}

Inventory_cell::Inventory_cell(GameObject* item = nullptr) : m_item(item)
{
	m_kind = entity_e::inventory_cell;
}

Property_Container::Property_Container(int width, int height, std::string name) : Object_feature(object_feature_e::container), m_size(dimension_t(width, height)), m_name(name)
{
	m_items.resize(m_size.w*m_size.h, nullptr);
	for (std::list<Inventory_cell*>::iterator item = m_items.begin(); item != m_items.end(); item++)
	{
		(*item) = new Inventory_cell();
	}
}

Object_feature* Property_Container::clone()
{
	Property_Container* result = new Property_Container(m_size.w,m_size.h,m_name);
	return result;
}

Property_Container::~Property_Container()
{
}

AI_manager::AI_manager()
{
	m_fov_qualifiers.push_back([](GameObject* object)->bool{return object->m_name == "wall"; });
	m_fov_qualifiers.push_back([](GameObject* object)->bool{return false; });

	m_path_qualifiers.push_back([](GameObject* object)->bool{return !object->get_stat(object_attribute_e::pass_able);});
	m_path_qualifiers.push_back([](GameObject* object)->bool{return !object->get_stat(object_attribute_e::pass_able) && object->m_name != "wall"; });
}

Property_body::Property_body() : Object_feature(object_feature_e::body)
{

}

Object_feature* Property_body::clone()
{
	Property_body* result = new Property_body();
	for (auto part = m_item.begin(); part != m_item.end(); part++)
	{
		result->m_item.push_back((*part));
	}
	return result;
}

Attribute_feature::Attribute_feature() : Object_feature(object_feature_e::attribute_feature)
{
}

Object_feature* Attribute_feature::clone()
{
	Attribute_feature* result = new Attribute_feature();
	std::copy(m_label.begin(), m_label.end(), std::back_inserter(result->m_label));
	std::copy(m_stat.begin(), m_stat.end(), std::back_inserter(result->m_stat));
	return result;
}

//Parameter_feature::Parameter_feature() : Object_feature(object_feature_e::parameter_feature)
//{
//}
//
//Object_feature* Parameter_feature::clone()
//{
//	Parameter_feature* result = new Parameter_feature();
//	result->m_parameter.insert(m_parameter.begin(), m_parameter.end());
//	return result;
//}

Interaction_feature::Interaction_feature() : Object_feature(object_feature_e::interaction_feature)
{
}

Object_feature* Interaction_feature::clone()
{
	Interaction_feature* result = new Interaction_feature();
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		result->m_effect[item->first] = item->second->clone();
	}
	return result;
}

Object_parameter_modifier::Object_parameter_modifier()
{
}

Object_parameter_modifier_single::Object_parameter_modifier_single(int value = 0, int duration = 0) :m_value(value), m_duration(duration)
{
}

Object_parameter_modifier_list::Object_parameter_modifier_list()
{
}

object_parameter_t::object_parameter_t(int value = 0, int limit = 0) : m_value(value), m_limit(limit)
{
}
