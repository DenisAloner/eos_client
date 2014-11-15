#pragma once
#include "GameObject.h"

class Object_interaction
{
public:

	reaction_e m_kind;

	virtual void apply(GameObject* object) = 0;
	virtual bool on_turn() = 0;
	virtual std::string get_description() = 0;
};

class Interaction_list :public Object_interaction
{
public:

	std::list<Object_interaction*> m_effect;

	virtual void apply(GameObject* object);
	virtual bool on_turn();
	virtual std::string get_description();
};
 
class Parameter_modification :public Object_interaction
{
public:

	//Object_parameter_modifier* m_modifier;
	Parameter_modification();
	//virtual void apply(GameObject* object) = 0;
	//virtual bool on_turn() = 0;
	//virtual std::string get_description() = 0;
};

class Effect :public Object_interaction
{
public:

	effect_e m_subtype;
	int m_value;

	Effect();
	virtual void apply(GameObject* object);
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

	typedef std::function<void(Reaction*, GameObject*, Object_interaction*)> func;
	typedef std::list < func > list;

	Reaction();
	func handler;
	virtual void apply(GameObject* object, Object_interaction* effect)=0;
	virtual Reaction* clone()=0;
};

class Reaction_effect :public Reaction
{
public:

	int m_value;
	std::map < effect_e, list > m_items;
	list m_list;

	Reaction_effect();
	virtual void apply(GameObject* object, Object_interaction* effect);
	virtual Reaction* clone();
};

class Reaction_effect :public Reaction
{
public:

	int m_value;
	std::map < effect_e, list > m_items;
	list m_list;

	Reaction_effect();
	virtual void apply(GameObject* object, Object_interaction* effect);
	virtual Reaction* clone();
};

class Reaction_parameter :public Reaction
{
public:

	int m_value;
	std::map < object_parameter_e, list > m_items;
	list m_list;

	Reaction_parameter();
	virtual void apply(GameObject* object, Object_interaction* effect);
	virtual Reaction* clone();
};