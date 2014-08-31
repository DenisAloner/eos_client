#ifndef TPARAMETER_H
#define	TPARAMETER_H

#include <vector>
#include <list>
#include "Definiton.h"

class MapCell;
class GameObject;
class Game_object_owner;
class GameMap;
class Inventory_cell;

class Parameter
{
public:

	ParameterKind m_kind;

	Parameter(ParameterKind _kind);
	~Parameter(void);
};

class Parameter_MapCell : public Parameter
{
public:
	MapCell* m_place;

	Parameter_MapCell();
	~Parameter_MapCell();
};

class Parameter_GameObject : public Parameter
{
public:

	GameObject* m_object;

	Parameter_GameObject();
	~Parameter_GameObject();
};

class Parameter_Position : public Parameter
{
public:

	GameObject* m_object;
	MapCell* m_place;
	GameMap* m_map;

	Parameter_Position(GameObject* object, MapCell* place, GameMap* map);

	Parameter_Position(void);
	~Parameter_Position(void);
};

class Parameter_MoveObjectByUnit : public Parameter
{
public:
	GameObject* m_unit;
	GameObject* m_object;
	MapCell* m_place;
	GameMap* m_map;

	Parameter_MoveObjectByUnit(void);
	~Parameter_MoveObjectByUnit(void);
};

class Parameter_destination : public Parameter
{
public:

	GameObject* m_unit;
	GameObject* m_object;
	Game_object_owner* m_owner;

	Parameter_destination();
};

class P_cell : public Parameter
{
public:
	MapCell* m_cell;

	P_cell();
};

class P_object : public Parameter
{
public:

	GameObject* m_object;

	P_object();
};

class P_inventory_cell : public Parameter
{
public:

	Inventory_cell* m_cell;

	P_inventory_cell();

};

#endif //TPARAMETER_H
