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

class Reaction
{
public:

	typedef std::function<void(Reaction*, GameObject*, Effect*)> func;
	typedef std::list < func > list;

	int m_value;

	func m_applicator;
	virtual void apply(int value, GameObject* object, Effect* effect) = 0;
	virtual Reaction* clone() = 0;

};

class Reaction_mods :public Reaction
{
public:

	list m_list;
	void apply(int value, GameObject* object, Effect* effect);
	virtual Reaction* clone();

};

class Reaction_subtype :public Reaction
{
public:

	
	std::map < effect_subtype_e, list > m_items;

	func m_applicator;
	void apply(int value, GameObject* object, Effect* effect);
	virtual Reaction* clone();

};

class Reaction_manager
{
public:

	void change_health(Reaction* reaction, GameObject* object, Effect* effect);
	void get_damage(Reaction* reaction,GameObject* object, Effect* effect);

	std::map<reaction_applicator_e, Reaction::func> m_items;
	Reaction_manager();
};