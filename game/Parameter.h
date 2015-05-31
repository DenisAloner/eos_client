#ifndef TPARAMETER_H
#define	TPARAMETER_H

#include <vector>
#include <list>
#include "Definiton.h"
#include "GameObject.h"

class Game_object_owner;
class MapCell;
class GameMap;
class Object_part;
class Action;

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

class P_object : public Parameter
{
public:

	GameObject* m_object;

	P_object();
};

class P_object_owner : public Parameter
{
public:

	Game_object_owner* m_cell;
	P_object_owner(ParameterKind kind);
};

class P_unit_interaction : public Parameter
{
public:

	GameObject* m_unit;
	Object_part* m_unit_body_part;
	GameObject* m_object;

	P_unit_interaction();
};

class P_interaction_cell : public Parameter
{
public:

	GameObject* m_unit;
	Object_part* m_unit_body_part;
	GameObject* m_object;
	MapCell* m_cell;

	P_interaction_cell();
};

#endif //TPARAMETER_H
