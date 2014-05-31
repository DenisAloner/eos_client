#include "GameObject.h"

GameObject::GameObject()
{
	m_direction = ObjectDirection_Left;
}

//bool GameObject::ContainAction(TAction* Action)
//{
//	return std::find(Actions.begin(), Actions.end(), 1)==Actions.end();
//}

GameObjectProperty* GameObject::find_property(PropertyKind kind)
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

TAction* GameObject::find_action(ActionKind kind)
{
	for(std::list<TAction*>::iterator Current=m_actions.begin();Current!=m_actions.end();Current++)
	{
		if((*Current)->m_kind==kind)
		{
			return (*Current);
		}
	}
	return nullptr;
}

void GameObject::Turn()
{
	if(m_direction == ObjectDirection_Left)
	{
		m_direction = ObjectDirection_Right;
	} else {
		m_direction = ObjectDirection_Left;
	}
}

TPlayer::TPlayer()
{
	m_cell=nullptr;
	m_sprites[ObjectDirection_Left][0] = 3;
	m_sprites[ObjectDirection_Left][1] = 4;
	m_sprites[ObjectDirection_Left][2] = 5;
	m_sprites[ObjectDirection_Left][3] = 6;
	m_sprites[ObjectDirection_Right][0] = 10;
	m_sprites[ObjectDirection_Right][1] = 11;
	m_sprites[ObjectDirection_Right][2] = 12;
	m_sprites[ObjectDirection_Right][3] = 13;
	m_size.x = 2;
	m_size.y = 1;
	m_size.z = 6;
	m_light = nullptr;
	m_actions.push_back(Application::instance().m_actions[ActionKind_Move]);
	m_properties.push_back(new GameObjectProperty(PropertyKind_Impassable));
	m_properties.push_back(new Property_Container(2, 4, "Мешок"));
	m_name = "Игрок";
	m_selected = false;
}

TOrc::TOrc()
{
	m_cell = nullptr;
	m_sprites[ObjectDirection_Left][0] = 7;
	m_sprites[ObjectDirection_Left][1] = 7;
	m_sprites[ObjectDirection_Left][2] = 7;
	m_sprites[ObjectDirection_Left][3] = 7;
	m_size.x = 3;
	m_size.y = 2;
	m_size.z = 6;
	m_light = nullptr;
	m_properties.push_back(new GameObjectProperty(PropertyKind_Impassable));
	m_name = "Враг";
	m_selected = false;
}

Elf::Elf()
{
	m_cell = nullptr;
	m_sprites[ObjectDirection_Left][0] = 14;
	m_sprites[ObjectDirection_Left][1] = 14;
	m_sprites[ObjectDirection_Left][2] = 14;
	m_sprites[ObjectDirection_Left][3] = 14;
	m_size.x = 2;
	m_size.y = 2;
	m_size.z = 6;
	m_light = nullptr;
	m_properties.push_back(new GameObjectProperty(PropertyKind_Impassable));
	m_name = "Эльф";
	m_selected = false;
}

TFloor::TFloor()
{
	m_cell = nullptr;
	m_sprites[ObjectDirection_Left][0] = 0;
	m_sprites[ObjectDirection_Left][1] = 0;
	m_sprites[ObjectDirection_Left][2] = 0;
	m_sprites[ObjectDirection_Left][3] = 0;
	m_light = nullptr;
	m_size.x = 1;
	m_size.y = 1;
	m_size.z = 1;
	m_actions.push_back(Application::instance().m_actions[ActionKind_Move]);
	m_name = "Пол";
	m_selected = false;
}

TBox::TBox()
{
	m_cell = nullptr;
	m_sprites[ObjectDirection_Left][0] = 9;
	m_sprites[ObjectDirection_Left][1] = 9;
	m_sprites[ObjectDirection_Left][2] = 9;
	m_sprites[ObjectDirection_Left][3] = 9;
	m_light = nullptr;
	m_size.x = 2;
	m_size.y = 2;
	m_size.z = 3;
	m_actions.push_back(Application::instance().m_actions[ActionKind_Push]);
	m_properties.push_back(new GameObjectProperty(PropertyKind_Impassable));
	m_properties.push_back(new Property_Container(5, 5, "Внутри"));
	m_name = "Сундук";
	m_selected = false;
}

TWall::TWall()
{
	m_cell = nullptr;
	m_sprites[ObjectDirection_Left][0] = 1;
	m_sprites[ObjectDirection_Left][1] = 1;
	m_sprites[ObjectDirection_Left][2] = 1;
	m_sprites[ObjectDirection_Left][3] = 1;
	m_light = nullptr;
	m_size.x = 1;
	m_size.y = 1;
	m_size.z = 5;
	m_properties.push_back(new GameObjectProperty(PropertyKind_Impassable));
	m_name = "Стена";
	m_selected = false;
}

TTorch::TTorch()
{
	m_cell = nullptr;
	m_sprites[ObjectDirection_Left][0] = 2;
	m_sprites[ObjectDirection_Left][1] = 2;
	m_sprites[ObjectDirection_Left][2] = 2;
	m_sprites[ObjectDirection_Left][3] = 2;
	m_size.x = 1;
	m_size.y = 1;
	m_size.z = 4;
	m_light = new TLight;
	m_light->Power = 2;
	m_light->RGB[0] = 1;
	m_light->RGB[1] = 1;
	m_light->RGB[2] = 1;
	m_properties.push_back(new GameObjectProperty(PropertyKind_Impassable));
	m_name = "Факел";
	m_selected = false;
}