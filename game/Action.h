#ifndef TACTION_H
#define	TACTION_H

#include <algorithm>
#include <iterator>
#include <string>
#include "Definiton.h"
#include "MouseController.h"
#include "GameObject.h"
#include "Game_algorithm.h"
#include  "impact\Effect.h"

class Application;
class Parameter;
class P_interaction_cell;
class P_bow_shoot;
class Action;
class Interaction_prefix;

class Action : public Object_interaction
{
public:

	size_t m_index;

	GLuint m_icon;
	action_e m_kind;
	std::string m_name;
	std::string m_error;
	int m_decay;

	Action(void);
	~Action(void);

	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter) = 0;
	virtual void interaction_handler(Parameter* arg);
	virtual bool on_turn(){ return false; };
	virtual std::string get_description() { return "действие"; };
	virtual Object_interaction* clone();
	virtual void description(std::list<std::string>* info, int level) {};

	virtual void reset_serialization_index() { m_serialization_index = 1; };
	virtual void save();
	virtual void load();

	virtual bool get_child(GameTask*& task) { return false; };
};

class Action_wrapper : public Object_interaction
{
public:

	int m_decay;
	Action* m_action;
	Parameter* m_parameter;

	Action_wrapper();

	virtual bool on_turn() { return false; };
	virtual std::string get_description() { return "действие"; };
	virtual void description(std::list<std::string>* info, int level) {};
	virtual Action_wrapper* clone();
	//virtual void perfom(Parameter* parameter);

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();

	void set(GameObject* unit, Action* action, Parameter* parameter);
	void update();

private:
	Interaction_prefix* m_prefix;
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
	virtual void interaction_handler(Parameter* arg);
};

class action_move_step :
	public ActionClass_Move
{
public:

	action_move_step();

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
	virtual void interaction_handler(Parameter* arg);

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
	virtual void interaction_handler(Parameter* arg);

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
	virtual void interaction_handler(Parameter* arg);

};

class Action_CellInfo :
	public Action
{
public:

	Action_CellInfo();

	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler(Parameter* arg);

};

class action_set_motion_path :
	public Action
{
public:

	action_set_motion_path();

	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler(Parameter* arg);
};


class Action_pick :
	public Action
{
public:

	Action_pick();

	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler(Parameter* arg);
	virtual void description(std::list<std::string>* info, int level);

};

class Action_open :
	public Action
{
public:

	Action_open();

	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler(Parameter* arg);

};

class Action_hit :
	public Action
{
public:

	Action_hit();
	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler(Parameter* arg);

};

class action_hit_melee :
	public Action_hit
{
public:

	action_hit_melee();
	virtual void interaction_handler(Parameter* arg);
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
	virtual void interaction_handler(Parameter* arg);

};

class Action_show_parameters :
	public Action
{
public:

	Action_show_parameters();

	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler(Parameter* arg);

};


class Action_use:
	public Action
{
public:

	Action_use();

	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler(Parameter* arg);
};

class Action_save :
	public Action
{
public:

	Action_save();

	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler(Parameter* arg);

};

class Action_autoexplore :
	public Action
{
public:

	Action_autoexplore();

	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler(Parameter* arg);
	virtual bool get_child(GameTask*& task);

};

class Action_shoot :
	public Action
{
public:

	P_bow_shoot* m_arg;

	Action_shoot();
	virtual bool check(Parameter* parameter);
	virtual void perfom(Parameter* parameter);
	virtual std::string get_description(Parameter* parameter);
	virtual void interaction_handler(Parameter* arg);
	bool process_cell(MapCell* a);
private:
	int m_distance;
	bool check_cell(MapCell* a);
};

#endif //TACTION_H