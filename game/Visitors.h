#ifndef VISITORS_H
#define	VISITORS_H

class Attribute_map;
class Object_state;
class GameObject;
class Interaction_list;
class Parameter_list;
class Vision_list;
class Vision_component;
class Parts_list;
class Object_part;
class Tag_list;
class Action_list;
class Action_pick;
class Instruction_check_tag;
class Instruction_slot_link;
class Slot_set_state;
class Interaction_copyist;
class Interaction_prefix;
class Interaction_addon;
class Interaction_time;
class Interaction_timer;
class Effect;
class Instruction_slot_parameter;

namespace ObjectTag
{
	class Label;
	class Requirements_to_object;
	class Mortal;
}

class Visitor_generic
{
public:

	virtual void visit(GameObject& value) {};
	virtual void visit(Attribute_map& value) {};
	virtual void visit(Object_state& value) {};
	virtual void visit(Interaction_list& value) {};
	virtual void visit(Parameter_list& value) {};
	virtual void visit(Tag_list& value) {};
	virtual void visit(Vision_list& value) {};
	virtual void visit(Vision_component& value) {};
	virtual void visit(Parts_list& value) {};
	virtual void visit(Object_part& value) {};
	virtual void visit(ObjectTag::Label& value) {};
	virtual void visit(ObjectTag::Requirements_to_object& value) {};
	virtual void visit(ObjectTag::Mortal& value) {};
	virtual void visit(Action_list& value) {};
	virtual void visit(Action_pick& value) {};
	virtual void visit(Instruction_check_tag& value) {};
	virtual void visit(Instruction_slot_link& value) {};
	virtual void visit(Slot_set_state& value) {};
	virtual void visit(Interaction_copyist& value) {};
	virtual void visit(Interaction_prefix& value) {};
	virtual void visit(Interaction_addon& value) {};
	virtual void visit(Interaction_time& value) {};
	virtual void visit(Interaction_timer& value) {};
	virtual void visit(Effect& value) {};
	virtual void visit(Instruction_slot_parameter& value) {};
};

class Visitor_simple :public Visitor_generic
{
public:

	void visit(GameObject& value) override;
	void visit(Attribute_map& value) override;
	void visit(Object_state& value) override;
	void visit(Interaction_list& value) override;
	void visit(Parameter_list& value) override;
	void visit(Vision_list& value) override;
	void visit(Vision_component& value) override;
	void visit(Parts_list& value) override;
	void visit(Object_part& value) override;
	void visit(Tag_list& value) override;
	void visit(Action_list& value) override;
	void visit(ObjectTag::Requirements_to_object& value) override;
	void visit(Instruction_slot_link& value) override;
	void visit(Interaction_copyist& value) override;
	void visit(Interaction_prefix& value) override;
	void visit(Interaction_addon& value) override;
	void visit(Interaction_time& value) override;
	void visit(Interaction_timer& value) override;
	void visit(Instruction_slot_parameter& value) override;

};

#endif VISITORS_H