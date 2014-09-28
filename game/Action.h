#ifndef TACTION_H
#define	TACTION_H

#include <algorithm>
#include <iterator>
#include <string>
#include "Definiton.h"


class Parameter;
class Application;
	
class Action
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
};

class action_move_step :
	public ActionClass_Move
{
public:
	virtual bool check(Parameter* parameter);
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

};

class action_set_motion_path :
	public Action
{
public:

	action_set_motion_path();

	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();

};


class Action_pick :
	public Action
{
public:

	Action_pick();

	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();

};

class Action_open :
	public Action
{
public:

	Action_open();

	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();

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

};

#endif //TACTION_H