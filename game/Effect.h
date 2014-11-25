#pragma once
#include "GameObject.h"

class Object_interaction
{
public:

	reaction_e m_kind;
	Object_interaction();
	virtual void apply(GameObject* object) = 0;
	virtual bool on_turn() = 0;
	virtual std::string get_description() = 0;
	virtual Object_interaction* clone() = 0;
};



class Interaction_list :public Object_interaction
{
public:

	std::list<Object_interaction*> m_effect;
	Interaction_list();
	virtual void apply(GameObject* object);
	virtual bool on_turn();
	virtual std::string get_description();
	virtual void add(Object_interaction* item);
	virtual void update();
	virtual Object_interaction* clone();

};

class Parameter_list :public Interaction_list
{
public:

	int m_basic_value;
	int m_basic_limit;

	int m_value;
	int m_limit;


	virtual std::string get_description();
	virtual void update();
	virtual void add(Object_interaction* item);

private:
	void update_list(Interaction_list* list);
};

class Interaction_slot :public Object_interaction
{
public:

	Object_interaction* m_value;
	interaction_e m_subtype;
	Interaction_slot();
	virtual void apply(GameObject* object);
	virtual bool on_turn();
	virtual std::string get_description();
	virtual Object_interaction* clone();
};
 
class Effect :public Object_interaction
{
public:

	int m_value;
	effect_e m_subtype;
	Effect();
	virtual void apply(GameObject* object);
	virtual bool on_turn();
	virtual Object_interaction* clone();
	virtual std::string get_description();
};

class Buff : public Effect
{
public:

	int m_duration;

	Buff();
	virtual bool on_turn();
	virtual Object_interaction* clone();
	virtual std::string get_description();
};

class Buff_chance: public Buff
{
public:

	int m_chance;

	Buff_chance();
	virtual Object_interaction* clone();
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

//class Reaction_parameter :public Reaction
//{
//public:
//
//	int m_value;
//	std::map < object_parameter_e, list > m_items;
//	list m_list;
//
//	Reaction_parameter();
//	virtual void apply(GameObject* object, Object_interaction* effect);
//	virtual Reaction* clone();
//};

