#include "game/GameObject.h"
#include "game/Action.h"
#include "log.h"


Game_state::Game_state()
{
	m_layer = 1;
	m_light = nullptr;
	m_tile_manager = nullptr;
	m_icon = Application::instance().m_graph->m_no_image;
	m_ai = nullptr;
}


GameObjectProperty* Game_state::find_property(property_e kind)
{
	for (std::list<GameObjectProperty*>::iterator Current = m_properties.begin(); Current != m_properties.end(); Current++)
	{
		if ((*Current)->m_kind == kind)
		{
			return (*Current);
		}
	}
	return nullptr;
}

Action* Game_state::find_action(action_e kind)
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

void Game_state::set_tile_size()
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

GameObjectProperty* MapCell::find_property(property_e kind, GameObject* excluded)
{
	GameObjectProperty* result(nullptr);
	for (std::list<GameObject*>::iterator item = m_items.begin(); item != m_items.end(); item++)
	{
		if (excluded != (*item))
		{
			result = (*item)->m_active_state->find_property(kind);
			if (result != nullptr)
			{
				return result;
			}
		}
	}
	return nullptr;
}

bool MapCell::check_permit(property_e kind, GameObject* excluded)
{
	GameObjectProperty* result(nullptr);
	for (std::list<GameObject*>::iterator item = m_items.begin(); item != m_items.end(); item++)
	{
		if (excluded != (*item))
		{
			result = (*item)->m_active_state->find_property(kind);
			if (result == nullptr)
			{
				return false;
			}
		}
	}
	return true;
}

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
		for (std::list<Game_state*>::iterator item = m_state.begin(); item != m_state.end(); item++)
		{
			(*item)->m_size = game_object_size_t((*item)->m_size.y, (*item)->m_size.x, (*item)->m_size.z);
		}
	}
	for (std::list<Game_state*>::iterator item = m_state.begin(); item != m_state.end(); item++)
	{
		(*item)->set_tile_size();
	}
	m_direction = dir;
}

void GameObject::set_state(state_e state)
{
	for (std::list<Game_state*>::iterator item = m_state.begin(); item != m_state.end(); item++)
	{
		if ((*item)->m_state == state)
		{
			m_active_state = (*item);
			return;
		}
	}
}

void GameObject::add_effect(effect_e kind, Effect* effect)
{
	m_effect[kind].push_back(effect);
}

void GameObject::remove_effect(effect_e kind, Effect* effect)
{
	m_effect[kind].push_back(effect);
	if (m_effect.find(kind)!=m_effect.end())
	{
		m_effect[kind].remove(effect);
	}
}

MapCell* GameObject::cell(){
	return static_cast<MapCell*>(m_owner);
}

Player::Player(GameObject* object, GameMap* map) :m_object(object), m_map(map)
{
	m_fov = new FOV();
	m_fov->calculate(object->m_active_state->m_ai->m_fov_radius, m_object, m_map);
}

GameObjectProperty::GameObjectProperty(property_e _kind = property_e::none) : m_kind(_kind)
{
}

GameObjectProperty* GameObjectProperty::clone()
{
	GameObjectProperty* result = new GameObjectProperty();
	result->m_kind = m_kind;
	return result;
}

GameObjectProperty::~GameObjectProperty(void)
{
}

GameObjectParameter::GameObjectParameter(property_e kind = property_e::none, float value = 0) :GameObjectProperty(kind), m_value(value){}

GameObjectProperty* GameObjectParameter::clone()
{
	GameObjectParameter* result = new GameObjectParameter();
	result->m_kind = m_kind;
	result->m_value = m_value;
	return result;
}

Game_object_feature::Game_object_feature(property_e kind = property_e::none, int value = 0) : GameObjectProperty(kind), m_value(value){}

GameObjectProperty* Game_object_feature::clone()
{
	Game_object_feature* result = new Game_object_feature();
	result->m_kind = m_kind;
	result->m_value = m_value;
	return result;
}

GameObjectLink::GameObjectLink(property_e kind = property_e::none, GameObject* object = nullptr) : GameObjectProperty(kind), m_object(object){}

GameObjectProperty* GameObjectLink::clone()
{
	GameObjectLink* result = new GameObjectLink();
	result->m_kind = m_kind;
	result->m_object = m_object;
	return result;
}

Inventory_cell::Inventory_cell(GameObject* item = nullptr) : m_item(item)
{
	m_kind = entity_e::inventory_cell;
}

Property_Container::Property_Container(int width, int height, std::string name) : GameObjectProperty(property_e::container), m_size(dimension_t(width, height)), m_name(name)
{
	m_items.resize(m_size.w*m_size.h, nullptr);
	for (std::list<Inventory_cell*>::iterator item = m_items.begin(); item != m_items.end(); item++)
	{
		(*item) = new Inventory_cell();
	}
}

GameObjectProperty* Property_Container::clone()
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

	m_path_qualifiers.push_back([](GameObject* object)->bool{return !object->m_active_state->find_property(property_e::permit_move);});
	m_path_qualifiers.push_back([](GameObject* object)->bool{return !object->m_active_state->find_property(property_e::permit_move) && object->m_name != "wall"; });
}

Property_body::Property_body() : GameObjectProperty(property_e::body)
{

}

GameObjectProperty* Property_body::clone()
{
	Property_body* result = new Property_body();
	for (auto part = m_item.begin(); part != m_item.end(); part++)
	{
		result->m_item.push_back((*part));
	}
	return result;
}