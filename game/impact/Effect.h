#pragma once
#include "GameObject.h"
#include "Parameter.h"
#include <functional>

class Interaction_list :public Object_interaction
{
public:

	feature_list_type_e m_list_type;
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
	virtual void equip(Object_interaction* item) { m_effect.push_back(item); };
	virtual void unequip(Object_interaction* item) { m_effect.remove(item); };

	virtual void do_predicat(predicat func);

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
	virtual void update();
	virtual Parameter_list* clone();
	virtual void description(std::list<std::string>* info, int level);

	virtual void save();
	virtual void load();

private:
	void update_list(Object_interaction* list);
};

class Vision_list :public Interaction_list
{
public:
	
	Vision_list();
	virtual std::string get_description();
	virtual void update();
	virtual Vision_list* clone();
	virtual void description(std::list<std::string>* info, int level);

	virtual void equip(Object_interaction* item);
	virtual void unequip(Object_interaction* item);

	virtual void save();
	virtual void load();

private:
	void update_list(Object_interaction* list);
};

class Vision_item :public Interaction_list
{
public:
 
	AI_FOV m_basic_value;
	AI_FOV m_value;
	Vision_item();
	virtual std::string get_description();
	virtual void update();
	virtual Vision_item* clone();
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


	virtual void save();
	virtual void load();
};

class Parts_list :public  Interaction_list
{
public:

	Parts_list();
	virtual Parts_list* clone();

	virtual void save();
	virtual void load();
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
	virtual void do_predicat(predicat func);
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
}
