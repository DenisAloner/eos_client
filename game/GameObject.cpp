#include "game/GameObject.h"
#include "game/Action.h"


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
			result = (*item)->find_property(kind);
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
			result = (*item)->find_property(kind);
			if (result == nullptr)
			{
				return false;
			}
		}
	}
	return true;
}

void MapCell::set_path_info()
{
	m_path_info = 0;
	for (std::list<GameObject*>::iterator item = m_items.begin(); item != m_items.end(); item++)
	{
		if ((*item)->m_name != "floor" && (*item)->m_name != "door")
		{
			m_path_info = 1;
			return;
		}
	}
}

GameObject::GameObject()
{
	m_layer = 1;
	m_direction = ObjectDirection_Down;
	m_selected = false;
	m_light = nullptr;
	m_tile_manager = nullptr;
	m_weight = 0.0;
	m_icon = Application::instance().m_graph->m_no_image;
}


//bool GameObject::ContainAction(Action* Action)
//{
//	return std::find(Actions.begin(), Actions.end(), 1)==Actions.end();
//}


GameObjectProperty* GameObject::find_property(property_e kind)
{
	for(std::list<GameObjectProperty*>::iterator Current=m_properties.begin();Current!=m_properties.end();Current++)
	{
		if((*Current)->m_kind==kind)
		{
			return (*Current);
		}
	}
	return nullptr;
}

Action* GameObject::find_action(action_e kind)
{
	for(std::list<Action*>::iterator Current=m_actions.begin();Current!=m_actions.end();Current++)
	{
		if((*Current)->m_kind == kind)
		{
			return (*Current);
		}
	}
	return nullptr;
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
	set_tile_direction(dir);
}

void GameObject::set_tile_size()
{
	m_tile_size = dimension_t(m_size.x, m_size.y + m_size.z);
}


void GameObject::set_tile_direction(ObjectDirection dir)
{
	if (((m_direction == ObjectDirection_Left || m_direction == ObjectDirection_Right) && (dir == ObjectDirection_Up || dir == ObjectDirection_Down)) || ((dir == ObjectDirection_Left || dir == ObjectDirection_Right) && (m_direction == ObjectDirection_Up || m_direction == ObjectDirection_Down)))
	{
		m_size = game_object_size_t(m_size.y, m_size.x, m_size.z);
	}
	set_tile_size();
	m_direction = dir;
}

MapCell* GameObject::cell(){
	return static_cast<MapCell*>(m_owner);
}

GameObjectProperty::GameObjectProperty(property_e _kind = property_e::none) : m_kind(_kind)
{
}


GameObjectProperty::~GameObjectProperty(void)
{
}

GameObjectParameter::GameObjectParameter(property_e kind = property_e::none, float value = 0) :GameObjectProperty(kind), m_value(value){}

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

Property_Container::~Property_Container()
{
}
