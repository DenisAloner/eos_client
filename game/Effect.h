#pragma once
#include "GameObject.h"

class Effect
{
public:
	Effect();
	virtual void apply(GameObject* object, Effect* effect) = 0;
};

class Effect_int :public Effect
{
public:

	int m_value;
	
	Effect_int();
	virtual void apply(GameObject* object, Effect* effect) = 0;

};

class Effect_damage :public Effect_int
{
public:

	int m_value;
	reaction_e m_kind;

	Effect_damage();
	virtual void apply(GameObject* object, Effect* effect);

};

class Reaction
{
public:

	typedef std::function<void(Reaction*, GameObject*, Effect*)> func;
	
	std::list < func > m_items;

	int m_value;
	func apply;

	void call_items(int value, GameObject* object, Effect* effect);

};

class Reaction_manager
{
public:

	void check_health(Reaction* reaction, GameObject* object, Effect* effect);
	void get_physical_damage(Reaction* reaction,GameObject* object, Effect* effect);

	std::map<reaction_applicator_e, Reaction::func> m_items;
	Reaction_manager();
};