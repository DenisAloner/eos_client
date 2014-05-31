#pragma once
#include "TParameter.h"
#include <algorithm>
#include <iterator>
#include <string>
#include "Application.h"

class TParameter;
class Application;
	
class TAction
{
public:

	ActionKind m_kind;
	std::string m_name;

	TAction(void);
	~TAction(void);

	virtual bool check(TParameter* parameter);
	virtual void perfom(TParameter* parameter);
	virtual char const* get_description(TParameter* parameter) = 0;
	virtual void interaction_handler();
};


class ActionClass_Move :
	public TAction
{
public:

	ActionClass_Move();
	~ActionClass_Move();

	virtual bool check(TParameter* parameter);
	virtual void perfom(TParameter* parameter);
	virtual char const* get_description(TParameter* parameter);
	virtual void interaction_handler();
};


class ActionClass_Push :
	public TAction
{
public:

	ActionClass_Push();
	~ActionClass_Push();

	virtual bool check(TParameter* parameter);
	virtual void perfom(TParameter* parameter);
	virtual char const* get_description(TParameter* parameter);
	virtual void interaction_handler();

};

class ActionClass_Turn :
	public TAction
{
public:

	ActionClass_Turn();
	~ActionClass_Turn();

	virtual bool check(TParameter* parameter);
	virtual void perfom(TParameter* parameter);
	virtual char const* get_description(TParameter* parameter);
	virtual void interaction_handler();

};

class Action_OpenInventory :
	public TAction
{
public:

	Action_OpenInventory();
	~Action_OpenInventory();

	virtual bool check(TParameter* parameter);
	virtual void perfom(TParameter* parameter);
	virtual char const* get_description(TParameter* parameter);
	virtual void interaction_handler();

};

