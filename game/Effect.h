#pragma once
#include "GameObject.h"
#include "Parameter.h"
#include <functional>

class Object_interaction
{
public:
	
	typedef std::function<void(Object_interaction*)> predicat;

	interaction_message_type_e m_interaction_message_type;
	Object_interaction();
	virtual bool on_turn() = 0;
	virtual std::string get_description() = 0;
	virtual Object_interaction* clone() = 0;
	virtual void description(std::list<std::string>* info,int level) = 0;
	virtual void apply_effect(GameObject* unit,Object_interaction* object);
	virtual void do_predicat(predicat func);
};

class Action : public Object_interaction
{
public:

	GLuint m_icon;
	action_e m_kind;
	std::string m_name;
	std::string m_error;

	Action(void);
	~Action(void);

	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter) = 0;
	virtual void interaction_handler();
	virtual bool on_turn(){ return false; };
	virtual std::string get_description() { return "действие"; };
	virtual Object_interaction* clone();
	virtual void description(std::list<std::string>* info, int level) {};
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
	virtual void add(Object_interaction* item) { m_effect.push_back(item); };
	virtual void remove(Object_interaction* item) { m_effect.remove(item); };
	virtual void do_predicat(predicat func);
};

class Action_list :public Interaction_list
{
public:

	Action_list();
	virtual Interaction_list* clone();
};

class Parameter_list :public Interaction_list
{
public:

	interaction_e m_subtype;
	int m_basic_value;
	int m_basic_limit;

	int m_value;
	int m_limit;

	Parameter_list(interaction_e subtype);
	virtual std::string get_description();
	virtual void update();
	virtual Parameter_list* clone();
	virtual void description(std::list<std::string>* info, int level);

private:
	void update_list(Interaction_list* list);
};

class Tag_list :public  Interaction_list
{
public:

	Tag_list();
	virtual Tag_list* clone();
	//virtual void apply_effect(GameObject* unit, Object_interaction* object);
};

class Interaction_slot :public Object_interaction
{
public:

	Object_interaction* m_value;
	Interaction_slot();
	virtual bool on_turn();
	virtual void do_predicat(predicat func);
};

class Interaction_prefix :public Interaction_slot
{
public:

	effect_prefix_e m_subtype;
	Interaction_prefix();
	virtual std::string get_description();
	virtual Interaction_prefix* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
};

class Interaction_prefix_ex :public Interaction_prefix
{
public:
	Object_interaction* m_effect;
	virtual Interaction_prefix_ex* clone();
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

class Interaction_addon :public Interaction_slot
{
public:

	interaction_e m_subtype;
	Interaction_addon();
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

class Object_tag :public Object_interaction
{
public:

	object_tag_e m_type;
	Object_tag(object_tag_e key);
	virtual void description(std::list<std::string>* info, int level);
	virtual std::string get_description();
	virtual Object_tag* clone()=0;
	virtual bool on_turn();
};

namespace ObjectTag
{
	class Poison_resist :public Object_tag
	{
	public:
		Poison_resist();
		virtual Poison_resist* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);
	};

	class Mortal :public Object_tag
	{
	public:
		Mortal();
		virtual Mortal* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);
	};

	class Purification_from_poison: public Object_tag
	{
	public:
		Purification_from_poison();
		virtual Purification_from_poison* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);
	};
}

class ActionClass_Move :
	public Action
{
public:

	ActionClass_Move();
	~ActionClass_Move();

	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();
	virtual Object_interaction* clone(){ return new ActionClass_Move(); };
};

class action_move_step :
	public ActionClass_Move
{
public:
	virtual bool check(Parameter* parameter);
	virtual Object_interaction* clone(){ return new action_move_step(); };
};

class ActionClass_Push :
	public Action
{
public:

	ActionClass_Push();
	~ActionClass_Push();

	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();
	virtual Object_interaction* clone(){ return new ActionClass_Push(); };

};

class ActionClass_Turn :
	public Action
{
public:

	ActionClass_Turn();
	~ActionClass_Turn();

	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();
	virtual Object_interaction* clone(){ return new ActionClass_Turn(); };

};

class Action_OpenInventory :
	public Action
{
public:

	Action_OpenInventory();
	~Action_OpenInventory();

	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();
	virtual Object_interaction* clone(){ return new Action_OpenInventory(); };

};

class Action_CellInfo :
	public Action
{
public:

	Action_CellInfo();

	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();
	virtual Object_interaction* clone(){ return new Action_CellInfo(); };

};

class action_set_motion_path :
	public Action
{
public:

	action_set_motion_path();

	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();
	virtual Object_interaction* clone(){ return new action_set_motion_path(); };

};


class Action_pick :
	public Action
{
public:

	Action_pick();

	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();
	virtual Object_interaction* clone(){ return new Action_pick(); };

};

class Action_open :
	public Action
{
public:

	Action_open();

	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();
	virtual Object_interaction* clone(){ return new Action_open(); };

};

class Action_hit :
	public Action
{
public:

	Action_hit();
	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();
	virtual Object_interaction* clone(){ return new Action_hit(); };

};

class Action_equip :
	public Action
{
public:

	Action_equip();

	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();
	virtual Object_interaction* clone(){ return new Action_equip(); };

};

class Action_show_parameters :
	public Action
{
public:

	Action_show_parameters();

	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();
	virtual Object_interaction* clone(){ return new Action_show_parameters(); };

};
