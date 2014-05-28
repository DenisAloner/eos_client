#pragma once
#include "GameEntity.h"
#include <vector>
#include "Definiton.h"
#include "GameMap.h"
#include "GameObject.h"

class MapCell;
class GameObject;
class GameMap;

class TParameter
{
public:

	ParameterKind kind;

	TParameter(ParameterKind _kind);
	~TParameter(void);
};

class Parameter_MapCell :
	public TParameter
{
public:

	MapCell* place;

	Parameter_MapCell();
	~Parameter_MapCell();
};

class Parameter_GameObject :
	public TParameter
{
public:
	GameObject* object;

	Parameter_GameObject();
	~Parameter_GameObject();
};

class Parameter_Position:
	public TParameter
{
public:

	GameObject* object;
	MapCell* place;
	GameMap* map;

	Parameter_Position(void);
	~Parameter_Position(void);
};

class Parameter_MoveObjectByUnit :
	public TParameter
{
public:

	GameObject* unit;
	GameObject* object;
	MapCell* place;
	GameMap* map;

	Parameter_MoveObjectByUnit(void);
	~Parameter_MoveObjectByUnit(void);
};