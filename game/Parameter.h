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

class Parameter: public iSerializable
{
public:

	ParameterKind m_kind;

	Parameter(ParameterKind _kind);
	~Parameter(void);

	virtual void save() {};
	virtual void load() {};
};

class Parameter_MapCell : public Parameter
{
public:
	MapCell* m_place;

	Parameter_MapCell();
	~Parameter_MapCell();

	virtual void save();
	virtual void load();
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

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();
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

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();
};

class Parameter_destination : public Parameter
{
public:

	GameObject* m_unit;
	GameObject* m_object;
	Game_object_owner* m_owner;

	Parameter_destination();

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();
};

class P_object : public Parameter
{
public:

	GameObject* m_object;

	P_object();

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();
};

class P_object_owner : public Parameter
{
public:

	Game_object_owner* m_cell;

	P_object_owner();

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();
};

class P_unit_interaction : public Parameter
{
public:

	GameObject* m_unit;
	GameObject* m_object;
	Object_part* m_unit_body_part;
	
	P_unit_interaction();

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();
};


class P_interaction_cell : public Parameter
{
public:

	GameObject* m_unit;
	GameObject* m_object;
	Object_part* m_unit_body_part;
	MapCell* m_cell;

	P_interaction_cell();

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();
};

class P_bow_shoot : public Parameter
{
public:

	GameObject* m_unit;
	GameObject* m_object;
	Object_part* m_unit_body_part;
	std::vector<Object_part*> m_ammo_owner;
	MapCell* m_cell;

	P_bow_shoot();

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();
};

class Parameter_direction : public Parameter
{
public:

	GameObject* m_object;
	object_direction_e m_direction;

	Parameter_direction();

	Parameter_direction(GameObject* object, object_direction_e direction);

	virtual void reset_serialization_index() {};
	virtual void save() {};
	virtual void load() {};
};

#endif //TPARAMETER_H
