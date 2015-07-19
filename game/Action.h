#ifndef TACTION_H
#define	TACTION_H

#include <algorithm>
#include <iterator>
#include <string>
#include "Definiton.h"
#include "MouseController.h"
#include "GameObject.h"
#include "Game_algorithm.h"

class Application;
class Parameter;
class Action;

class Action : public Object_interaction
{
public:

	GLuint m_icon;
	action_e m_kind;
	std::string m_name;
	std::string m_error;
	float m_decay;

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
	virtual void description(std::list<std::string>* info, int level);

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

class action_hit_melee :
	public Action_hit
{
public:

	action_hit_melee();
	virtual Object_interaction* clone(){ return new action_hit_melee(); };
	virtual void interaction_handler();
	virtual void perfom(Parameter* parameter);
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

#endif //TACTION_H