#pragma once
#include "GameObject.h"

class Object_interaction
{
public:

	reaction_e m_kind;
	Object_interaction();
	virtual bool on_turn() = 0;
	virtual std::string get_description() = 0;
	virtual Object_interaction* clone() = 0;
	virtual void description(std::list<std::string>* info,int level) = 0;
	virtual void apply_effect(GameObject* unit,Object_interaction* object);
};

class Interaction_list :public Object_interaction
{
public:

	std::list<Object_interaction*> m_effect;
	Interaction_list();
	virtual bool on_turn();
	virtual std::string get_description();
	virtual void update();
	virtual Interaction_list* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
};

class Parameter_list :public  Interaction_list
{
public:

	int m_basic_value;
	int m_basic_limit;

	int m_value;
	int m_limit;

	virtual std::string get_description();
	virtual void update();
	virtual void add(Object_interaction* item);
	virtual Parameter_list* clone();
	virtual void description(std::list<std::string>* info, int level);

private:
	void update_list(Interaction_list* list);
};

class Interaction_slot :public Object_interaction
{
public:

	Object_interaction* m_value;
	Interaction_slot();
	virtual bool on_turn();
};

class Interaction_copyist :public Interaction_slot
{
public:

	interaction_e m_subtype;
	Interaction_copyist();
	virtual std::string get_description();
	virtual Object_interaction* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
};

class Interaction_time :public Interaction_slot
{
public:

	int m_turn;
	Interaction_time();
	virtual bool on_turn();
	virtual std::string get_description();
	virtual Object_interaction* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
};

class Interaction_timer :public Interaction_slot
{
public:

	int m_turn;
	int m_period;
	Interaction_timer();
	virtual bool on_turn();
	virtual std::string get_description();
	virtual Object_interaction* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
};
 
class Effect :public Object_interaction
{
public:

	int m_value;
	effect_e m_subtype;
	Effect();
	virtual bool on_turn();
	virtual Object_interaction* clone();
	virtual std::string get_description();
	virtual void description(std::list<std::string>* info, int level);
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
};



