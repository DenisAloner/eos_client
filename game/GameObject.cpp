#include "game/GameObject.h"
#include "game/Action.h"


GameObject::GameObject()
{
	m_layer = 1;
	m_direction = ObjectDirection_Down;
	m_selected = false;
	m_light = nullptr;
	m_cell = nullptr;
	m_tile_manager = nullptr;
	m_weight = 0.0;
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
