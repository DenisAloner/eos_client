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
	action_e m_kind;
	std::string m_name;

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
	~Action_CellInfo();

	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler();

};

#endif //TACTION_H