#pragma once
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "ActionsDescription.h"
#include <list>
#include "GameMap.h"
#include <algorithm>
#include "GameObjectProperty.h"
#include "Application.h"
#include "Property_Container.h"
#include "TileManager.h"

class MapCell;
class GameObjectProperty;
class TAction;
class Application;
class TileManager;

class GameObject
{
public:

	std::string m_name;
	game_object_size_t m_size;
	light_t* m_light;
	TileManager* m_tile_manager;
	ObjectDirection m_direction;
	bool m_selected;
	MapCell* m_cell;

	std::list<TAction*> m_actions;
	std::list<GameObjectProperty*> m_properties;

	GameObject();

	void Render(int x,int y)
	{
	}

	/*virtual bool ContainAction(TAction* Action);*/
	virtual TAction* find_action(action_e kind);
	virtual GameObjectProperty* find_property(property_e kind);
	void Turn();

};



