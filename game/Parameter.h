#ifndef TPARAMETER_H
#define	TPARAMETER_H

#include <vector>
#include "Definiton.h"


class MapCell;
class GameObject;
class GameMap;

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

#endif //TPARAMETER_H
