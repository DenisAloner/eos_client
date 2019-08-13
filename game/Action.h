#ifndef ACTION_H
#define	ACTION_H

#include <algorithm>
#include <iterator>
#include <string>
#include "Definiton.h"
#include "MouseController.h"
#include "GameObject.h"
#include "Game_algorithm.h"
#include  "impact/Effect.h"

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
	action_e::type m_kind;
	std::u16string m_name;
	std::u16string m_error;
	int m_decay;
	parameter_type_e m_parameter_kind;
	animation_e m_animation;

	Action(void);
	~Action(void);

	virtual bool check(Parameter* parameter);
	virtual char perform(Parameter* parameter);
	virtual std::u16string get_description(Parameter* parameter) = 0;
	virtual void interaction_handler(Parameter* parameter);
	Object_interaction* clone() override;

	void reset_serialization_index() override { m_serialization_index = 0; };

	virtual bool get_child(GameTask*& task) { return false; };

	Packer_generic& get_packer() final;

	constexpr static auto properties() { return std::make_tuple(make_property(&Action::m_kind, u"value")); }

};

class Action_wrapper : public Object_interaction
{
public:

	int m_decay;
	Action* m_action;
	Parameter* m_parameter;
	bool done;

	Action_wrapper();

	virtual std::u16string get_description() { return u"действие"; };
	Action_wrapper* clone() override;
	//virtual void perfom(Parameter* parameter);

	void reset_serialization_index() override;

	void set(GameObject* unit, Action* action, Parameter* parameter);
	void update();

	Packer_generic& get_packer() override;

private:
	Interaction_prefix* m_prefix;
};

class ActionClass_Move :
	public Action
{
public:

	ActionClass_Move();
	~ActionClass_Move();

	bool check(Parameter* parameter) override;
	char perform(Parameter* parameter) override;
	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* parameter) override;
};

class action_move_step :
	public ActionClass_Move
{
public:

	action_move_step();

	bool check(Parameter* parameter) override;
};

class ActionClass_Push :
	public Action
{
public:

	ActionClass_Push();
	~ActionClass_Push();

	bool check(Parameter* parameter) override;
	char perform(Parameter* parameter) override;
	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* parameter) override;

};

class ActionClass_Turn :
	public Action
{
public:

	ActionClass_Turn();
	~ActionClass_Turn();

	bool check(Parameter* parameter) override;
	char perform(Parameter* parameter) override;
	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* parameter) override;

};

class Action_OpenInventory :
	public Action
{
public:

	Action_OpenInventory();
	~Action_OpenInventory();

	bool check(Parameter* parameter) override;
	char perform(Parameter* parameter) override;
	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* parameter) override;

};

class Action_CellInfo :
	public Action
{
public:

	Action_CellInfo();

	bool check(Parameter* parameter) override;
	char perform(Parameter* parameter) override;
	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* arg) override;

};

class action_set_motion_path :
	public Action
{
public:

	action_set_motion_path();

	char perform(Parameter* parameter) override;
	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* parameter) override;
};


class Action_pick :
	public Action
{
public:

	Action_pick();

	char perform(Parameter* parameter) override;
	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* parameter) override;
	void apply_visitor(Visitor_generic& visitor) override;

};

class Action_move_out :
	public Action_pick
{
public:

	Action_move_out();

	std::u16string get_description(Parameter* parameter) override;

};

class Action_open :
	public Action
{
public:

	Action_open();

	char perform(Parameter* parameter) override;
	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* arg) override;

};

class Action_hit :
	public Action
{
public:

	Action_hit();
	bool check(Parameter* parameter) override;
	char perform(Parameter* parameter) override;
	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* arg) override;

};

class action_hit_melee :
	public Action_hit
{
public:

	action_hit_melee();
	void interaction_handler(Parameter* arg) override;
	char perform(Parameter* parameter) override;
};

class Action_equip :
	public Action
{
public:

	Action_equip();

	bool check(Parameter* parameter) override;
	char perform(Parameter* parameter) override;
	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* arg) override;

};

class Action_show_parameters :
	public Action
{
public:

	Action_show_parameters();

	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* arg) override;

};


class Action_use:
	public Action
{
public:

	Action_use();

	bool check(Parameter* parameter) override;
	char perform(Parameter* parameter) override;
	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* parameter) override;
};

class Action_save :
	public Action
{
public:

	Action_save();

	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* arg) override;

};

class Action_load :
	public Action
{
public:

	Action_load();

	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* arg) override;

};

class Action_rotate_view :public Action {
public:

	Action_rotate_view();

	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* arg) override;

};

class Action_change_z_level :public Action {
public:

	Action_change_z_level();

	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* arg) override;

};

class Action_autoexplore :
	public Action
{
public:

	Action_autoexplore();

	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* arg) override;
	bool get_child(GameTask*& task) override;

};

class Action_shoot :
	public Action
{
public:

	Action_shoot();
	bool check(Parameter* parameter) override;
	char perform(Parameter* parameter) override;
	std::u16string get_description(Parameter* parameter) override;
	void interaction_handler(Parameter* parameter) override;
	bool process_cell(Parameter* parameter,MapCell* cell);
private:
	int m_distance;
	bool check_cell(Parameter* parameter);
};

#endif //ACTION_H