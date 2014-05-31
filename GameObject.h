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

struct TLight
{
	float Power;
	float RGB[3];
};

struct GameObjectSize
{
	int x;
	int y;
	int z;
};

class MapCell;
class GameObjectProperty;
class TAction;
class Application;

enum ActionKind;

class GameObject
{
public:

	GameObjectSize m_size;
	TLight* m_light;
	std::string m_name;
	ObjectDirection m_direction;
	unsigned int m_sprites[4][4];
	bool m_selected;
	MapCell* m_cell;

	std::list<TAction*> m_actions;
	std::list<GameObjectProperty*> m_properties;

	GameObject();

	void Render(int x,int y)
	{
	}

	/*virtual bool ContainAction(TAction* Action);*/
	virtual TAction* find_action(ActionKind kind);
	virtual GameObjectProperty* find_property(PropertyKind kind);
	void Turn();

};


class TFloor :
	public GameObject
{
public:

	TFloor();

};

class TPlayer :
	public GameObject
{
public:

	TPlayer();

};

class TOrc :
	public GameObject
{
public:

	TOrc();

};

class Elf :
	public GameObject
{
public:

	Elf();

};

class TWall :
	public GameObject
{
public:

	TWall();

};

class TBox:
	public GameObject
{
public:

	TBox();

};

class TTorch:
	public GameObject
{
public:

	TTorch();

};