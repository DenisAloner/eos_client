#ifndef TPARAMETER_H
#define	TPARAMETER_H

#include "Definiton.h"
#include "GameObject.h"

class Game_object_owner;
class MapCell;
class GameMap;
class Object_part;
class Action;

struct Parameter_argument_t
{
	type_e kind;

	union
	{
		GameObject* m_object;
		MapCell* m_cell;
		Game_object_owner* m_owner;
		Object_part* m_part;
		object_direction_e m_direction;
	};

	Parameter_argument_t(){};

	Parameter_argument_t(type_e kind) :kind(kind) {};

	void set(GameObject* value);
	void set(Game_object_owner* value);
	void set(object_direction_e value);

	void init();
	void reset_serialization_index();
	void save();
	void load();

	operator bool();

};

class Parameter : public iSerializable
{
public:

	parameter_type_e m_kind;
	Parameter_argument_t* m_args;
	size_t m_size;

	Parameter(parameter_type_e kind);

	template<typename... T> 
	Parameter(parameter_type_e kind, T... args):Parameter(kind)
	{
		set_helper(0, args...);
	}

	Parameter_argument_t& operator[](std::size_t i)
	{
		return m_args[i];
	}

	const Parameter_argument_t& operator[](std::size_t i) const
	{
		return m_args[i];
	}

	Parameter* clone();

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();

private:

	template<typename T, typename... args>
	void set_helper(int i, T value)
	{
		m_args[i].set(value);
	};

	template<typename T, typename... args>
	void set_helper(int i, T value, args... a)
	{
		m_args[i].set(value);
		if (i < m_size) { set_helper(i + 1, a...); }
	};

};

//class Parameter_MoveObjectByUnit : public Parameter
//{
//public:
//	GameObject* m_unit;
//	GameObject* m_object;
//	MapCell* m_place;
//	GameMap* m_map;
//
//	Parameter_MoveObjectByUnit(void);
//	~Parameter_MoveObjectByUnit(void);
//
//	virtual void reset_serialization_index();
//	virtual void save();
//	virtual void load();
//};

class Instruction_result : public Object_interaction
{
public:

	Parameter_argument_t m_value;
	bool m_result;

	Instruction_result();
	virtual Instruction_result* clone() { return nullptr; };
	std::u16string get_description() override { return nullptr; };
	virtual bool on_turn() { return false; };
	virtual void apply_effect(GameObject* unit, Object_interaction* object) {};

	virtual void save() {};
	virtual void load() {};
};

#endif //TPARAMETER_H
