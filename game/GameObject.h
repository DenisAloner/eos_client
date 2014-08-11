#ifndef GAMEOBJECT_H
#define	GAMEOBJECT_H

#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <list>
#include <algorithm>
#include "Property_Container.h"


class MapCell;
class GameObjectProperty;
class Action;
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

	std::list<Action*> m_actions;
	std::list<GameObjectProperty*> m_properties;

	GameObject();

	void Render(int x,int y)
	{
	}

	/*virtual bool ContainAction(Action* Action);*/
	virtual Action* find_action(action_e kind);
	virtual GameObjectProperty* find_property(property_e kind);
	void Turn();

};

#endif //GAMEOBJECT_H
