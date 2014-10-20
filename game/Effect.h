#pragma once
#include "GameObject.h"

class Effect
{
public:

	reaction_e m_kind;
	effect_subtype_e m_subtype;
	int m_value;
	
	Effect();
	virtual void apply(GameObject* object, Effect* effect);
	virtual bool on_turn();
	virtual Effect* clone();
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

