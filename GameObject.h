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
#include "GameEntity.h"
#include "Property_Container.h"

struct TLight
{
	float Power;
	float RGB[3];
};

struct TArea
{
	int x;
	int y;
};

class MapCell;
class GameObjectProperty;
class TAction;
class Application;

enum ActionKind;

class GameObject: public GameEntity
{
public:

	int Width;
	int Height;
	TArea Area;
	TLight* Light;
	std::string Name;

	ObjectDirection direction;

	unsigned int Sprites[4][4];
	bool selected;

	MapCell* Cell;

	std::list<TAction*> Actions;
	std::list<GameObjectProperty*> Properties;

	GameObject(Application* app);

	void Render(int x,int y)
	{
	}

	/*virtual bool ContainAction(TAction* Action);*/
	virtual TAction* FindAction(ActionKind kind);
	virtual GameObjectProperty* FindProperty(PropertyKind kind);
	void Turn();

};


class TFloor :
	public GameObject
{
public:

	TFloor(Application* app);

};

class TPlayer :
	public GameObject
{
public:

	TPlayer(Application* app);

};

class TOrc :
	public GameObject
{
public:

	TOrc(Application* app);

};

class Elf :
	public GameObject
{
public:

	Elf(Application* app);

};

class TWall :
	public GameObject
{
public:

	TWall(Application* app);

};

class TBox:
	public GameObject
{
public:

	TBox(Application* app);

};

class TTorch:
	public GameObject
{
public:

	TTorch(Application* app);

};