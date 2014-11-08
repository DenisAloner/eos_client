#pragma once
#include "GameObject.h"

class Object_interaction
{
public:

	virtual void apply(GameObject* object) = 0;
	virtual bool on_turn() = 0;
	virtual std::string get_description() = 0;

};
 
class Effect :public Object_interaction
{
public:

	reaction_e m_kind;
	effect_subtype_e m_subtype;
	int m_value;

	Effect();
	virtual void apply(GameObject* object);
	virtual bool on_turn();
	virtual Effect* clone();
	virtual std::string get_description();
};

class Effect_list :public Object_interaction
{
public:

	std::list<Object_interaction*> m_effect;

	virtual void apply(GameObject* object);
	virtual bool on_turn();
	virtual std::string get_description();
};

class Buff : public Effect
{
public:

	int m_duration;

	Buff();
	virtual bool on_turn();
	virtual Effect* clone();
	virtual std::string get_description();
};

class Buff_chance: public Buff
{
public:

	int m_chance;

	Buff_chance();
	virtual Effect* clone();
};

class Reaction
{
public:

	typedef std::function<void(Reaction*, GameObject*, Effect*)> func;
	typedef std::list < func > list;

	int m_value;

	std::map < effect_subtype_e, list > m_items;
	list m_list;

	func m_applicator;
	void apply(int value, GameObject* object, Effect* effect);
	virtual Reaction* clone();
};

