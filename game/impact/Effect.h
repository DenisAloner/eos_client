#pragma once
#include "GameObject.h"
#include "Parameter.h"
#include <functional>

class Game_object_owner;

class Interaction_list :public Object_interaction
{
public:

	feature_list_type_e m_list_type;
	std::list<Object_interaction*> m_effect;

	Interaction_list();
	virtual bool on_turn();
	virtual std::string get_description();
	virtual bool update();
	virtual Interaction_list* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void add(Object_interaction* item) { m_effect.push_back(item); };
	virtual void remove(Object_interaction* item) { m_effect.remove(item); };
	virtual void equip(Object_interaction* item) { m_effect.push_back(item); };
	virtual void unequip(Object_interaction* item) { m_effect.remove(item); };

	virtual void do_predicat(Visitor& helper);
	virtual void do_predicat_ex(predicat_ex func);

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();

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
	Parameter_list();
	virtual std::string get_description();
	virtual bool update();
	virtual Parameter_list* clone();
	virtual void description(std::list<std::string>* info, int level);

	virtual void save();
	virtual void load();

private:

	class Update_visitor: public Visitor
	{
	public :

		Parameter_list& m_owner;

		Update_visitor(Parameter_list& owner);
		void visit(Object_interaction& value) override;

	};
};

class Vision_list :public Interaction_list
{
public:
	
	int m_max_radius;

	Vision_list();
	virtual std::string get_description();
	virtual bool update();
	virtual Vision_list* clone();
	virtual void description(std::list<std::string>* info, int level);

	virtual void equip(Object_interaction* item);
	virtual void unequip(Object_interaction* item);

	virtual void save();
	virtual void load();

private:
	void update_list(Object_interaction* list);
};

class Vision_component :public Interaction_list
{
public:
 
	AI_FOV m_basic_value;
	AI_FOV m_value;
	Vision_component();
	virtual std::string get_description();
	virtual bool update();
	virtual Vision_component* clone();
	virtual void description(std::list<std::string>* info, int level);

	virtual void save();
	virtual void load();

private:
	void update_list(Object_interaction* list);
};

class Tag_list :public  Interaction_list
{
public:

	Tag_list();
	virtual Tag_list* clone();

	virtual bool update();
	virtual void save();
	virtual void load();

private:
	
	class Update_visitor : public Visitor
	{
	public:
		
		bool was_changed;

		Update_visitor();
		void visit(Object_interaction& value);

	};

};

class Parts_list :public  Interaction_list
{
public:

	Parts_list();
	virtual Parts_list* clone();
	virtual bool update();
	virtual void save();
	virtual void load();

	virtual void equip(Object_interaction* item);


private:

	class Update_visitor : public Visitor
	{
	public:

		Parts_list& m_owner;
		bool was_changed;

		Update_visitor(Parts_list& owner);
		void visit(Object_interaction& value) override;

	};
};

class Action_list :public Interaction_list
{
public:

	Action_list();
	virtual Interaction_list* clone();

	virtual void save();
	virtual void load();
};

class Slot_set_state: public Object_interaction
{
public:

	object_state_e m_value;
	Slot_set_state() { m_interaction_message_type = interaction_message_type_e::set_attribute_map; };
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
	virtual bool on_turn() { return false; };
	virtual std::string get_description() { return "set_state"; };
	virtual Slot_set_state* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void do_predicat(predicat func) { func(this); };

	virtual void save();
	virtual void load();
};

class Slot_select_cell :public Object_interaction
{
public:

	std::string m_value;
	Slot_select_cell() { m_interaction_message_type = interaction_message_type_e::select_location; };
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
	virtual bool on_turn() { return false; };
	virtual std::string get_description() { return "select_location"; };
	virtual Slot_select_cell* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void do_predicat(predicat func) { func(this); };

	virtual void save();
	virtual void load();
};

class Slot_allocator :public Object_interaction
{
public:

	MapCell* m_value;
	Slot_allocator() { m_interaction_message_type = interaction_message_type_e::allocator; };
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
	virtual bool on_turn() { return false; };
	virtual std::string get_description() { return "select_location"; };
	virtual Slot_allocator* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void do_predicat(predicat func) { func(this); };

	virtual void save();
	virtual void load();
};

class Slot_mover :public Object_interaction
{
public:

	Slot_allocator* m_value;
	Slot_mover() { m_interaction_message_type = interaction_message_type_e::mover; };
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
	virtual bool on_turn() { return false; };
	virtual std::string get_description() { return "select_location"; };
	virtual Slot_mover* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void do_predicat(predicat func) { func(this); };

	virtual void save();
	virtual void load();
};

class Interaction_slot :public Object_interaction
{
public:

	Object_interaction* m_value;
	Interaction_slot();
	virtual bool on_turn();
	virtual void do_predicat(Visitor& helper);
	virtual void do_predicat_ex(predicat_ex func);
	virtual void reset_serialization_index();
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

	virtual void save();
	virtual void load();
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

	virtual void save();
	virtual void load();
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

	virtual void save();
	virtual void load();
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

	virtual void save();
	virtual void load();
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

	virtual void save();
	virtual void load();
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

	virtual void save();
	virtual void load();
};

class Object_tag :public Object_interaction
{
public:

	object_tag_e m_type;
	Object_tag(object_tag_e key);
	virtual void description(std::list<std::string>* info, int level);
	virtual std::string get_description();
	virtual Object_tag* clone() = 0;
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

		virtual void save();
		virtual void load();
	};

	class Mortal :public Object_tag
	{
	public:
		Mortal();
		virtual Mortal* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);

		virtual void save();
		virtual void load();
	};

	class Purification_from_poison : public Object_tag
	{
	public:
		Purification_from_poison();
		virtual Purification_from_poison* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);

		virtual void save();
		virtual void load();
	};

	class Activator : public Object_tag
	{
	public:
		Activator();
		std::list<GameObject*> m_link;
		virtual Activator* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);

		virtual void reset_serialization_index();
		virtual void save();
		virtual void load();
	};

	class Fast_move : public Object_tag
	{
	public:
		Fast_move();
		virtual Fast_move* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);

		virtual void save();
		virtual void load();
	};

	class Label : public Object_tag
	{
	public:
		Label(object_tag_e type);
		virtual Label* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);

		virtual void save();
		virtual void load();
	};

	class Equippable : public Object_tag
	{
	public:

		Object_interaction* m_value;
		Interaction_list* m_condition;

		Equippable();
		virtual Equippable* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);

		virtual void save();
		virtual void load();
	};

	class Requirements_to_object : public Object_tag
	{
	public:

		Object_interaction* m_value;
		bool m_result;

		Requirements_to_object();
		virtual Requirements_to_object* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);

		virtual void save();
		virtual void load();
	};

	class Can_transfer_object : public Object_tag
	{
	public:

		Object_interaction* m_value;
		bool m_result;

		Can_transfer_object();
		virtual Can_transfer_object* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);

		virtual void save();
		virtual void load();
	};
}

namespace Effect_functions
{
	Interaction_list* create_feature_list(feature_list_type_e key, interaction_e name);
}

class Instruction_slot_link :public Interaction_slot
{
public:

	interaction_e m_subtype;
	bool m_enable;
	Instruction_slot_link();
	virtual std::string get_description();
	virtual Instruction_slot_link* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void save();
	virtual void load();
};

class Instruction_slot_parameter :public Interaction_slot
{
public:

	Parameter* m_parameter;
	bool m_result;

	Instruction_slot_parameter();
	virtual std::string get_description();
	virtual Object_interaction* clone();
	virtual void description(std::list<std::string>* info, int level);
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void save();
	virtual void load();

};

class Instruction_game_owner :public Object_interaction
{
public:

	Game_object_owner* m_value;
	bool m_result;

	Instruction_game_owner();
	virtual std::string get_description() { return nullptr; };
	virtual bool on_turn() { return false; };
	virtual Object_interaction* clone() { return nullptr; };
	virtual void description(std::list<std::string>* info, int level) {};
	virtual void apply_effect(GameObject* unit, Object_interaction* object) {};

	virtual void save() {};
	virtual void load() {};

};

class Instruction_check_tag :public Object_interaction
{
public:

	object_tag_e m_value;

	Instruction_check_tag();
	virtual std::string get_description() { return nullptr; };
	virtual bool on_turn() { return false; };
	virtual Instruction_check_tag* clone();
	virtual void description(std::list<std::string>* info, int level) {};
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void save() {};
	virtual void load() {};

};

class Instruction_check_part_type :public Object_interaction
{
public:

	body_part_e m_value;

	Instruction_check_part_type();
	virtual std::string get_description() { return nullptr; };
	virtual bool on_turn() { return false; };
	virtual Instruction_check_part_type* clone();
	virtual void description(std::list<std::string>* info, int level) {};
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void save() {};
	virtual void load() {};

};

class Instruction_arg_extract : public Object_interaction
{
public:

	Object_interaction* m_value;
	std::size_t m_index;
	bool m_result;

	Instruction_arg_extract();
	virtual Instruction_arg_extract* clone();
	virtual std::string get_description() { return nullptr; };
	virtual bool on_turn() { return false; };
	virtual void description(std::list<std::string>* info, int level) {};
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void save();
	virtual void load();
};


class Instruction_get_owner : public Object_interaction
{
public:

	Object_interaction* m_value;
	bool m_result;

	Instruction_get_owner();
	virtual Instruction_get_owner* clone();
	virtual std::string get_description() { return nullptr; };
	virtual bool on_turn() { return false; };
	virtual void description(std::list<std::string>* info, int level) {};
	virtual void apply_effect(GameObject* unit, Object_interaction* object) ;

	virtual void save() {};
	virtual void load() {};
};

class Instruction_get_owner_top : public Instruction_get_owner
{
public:

	Instruction_get_owner_top();
	virtual Instruction_get_owner_top* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
};

class Instruction_check_owner_type :public Object_interaction
{
public:

	entity_e m_value;

	Instruction_check_owner_type();
	virtual std::string get_description() { return nullptr; };
	virtual bool on_turn() { return false; };
	virtual Instruction_check_owner_type* clone();
	virtual void description(std::list<std::string>* info, int level) {};
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void save() {};
	virtual void load() {};

};