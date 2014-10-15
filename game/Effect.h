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
};

class Buff
{
public:

	reaction_e m_kind;
	effect_subtype_e m_subtype;
	int m_value;
	int m_duration;

	Buff();
	virtual void apply(GameObject* object, Effect* effect);
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

