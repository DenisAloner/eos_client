#pragma once
#include "GameObject.h"
#include "Parameter.h"
#include <functional>

class Game_object_owner;

class Interaction_list :public Object_interaction
{
public:

	feature_list_type_e m_list_type;
	std::list<Object_interaction*> m_items;

	Interaction_list();
	virtual bool on_turn();
	std::u16string get_description() override;
	virtual bool update();
	virtual Interaction_list* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void add(Object_interaction* item) { m_items.push_back(item); };
	virtual void remove(Object_interaction* item) { m_items.remove(item); };
	virtual void equip(Object_interaction* item) { m_items.push_back(item); };
	virtual void unequip(Object_interaction* item) { m_items.remove(item); };

	virtual void do_predicat(Visitor& helper);
	void apply_visitor(Visitor_generic& visitor) override;

	virtual void reset_serialization_index();
	virtual void save();
	virtual void load();

	// ��� ��������� iJSONSerializable
	Packer_generic& get_packer() override
	{
		return Packer<Interaction_list>::Instance();
	}

	constexpr static auto properties() {
		return std::make_tuple(
			make_property(&Interaction_list::m_list_type, u"list_type"),
			make_property(&Interaction_list::m_items, u"items")
		);
	}

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
	std::u16string get_description() override;
	virtual bool update();
	virtual Parameter_list* clone();
	void apply_visitor(Visitor_generic& visitor) override;

	virtual void save();
	virtual void load();

	Packer_generic& get_packer() override
	{
		return Packer<Parameter_list>::Instance();
	}

	constexpr static auto properties() {
		return make_union(std::make_tuple(
			make_property(&Parameter_list::m_subtype, u"subtype"),
			make_property(&Parameter_list::m_basic_value, u"basic_value"),
			make_property(&Parameter_list::m_basic_limit, u"basic_limit")
		), Interaction_list::properties());
	}

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
	std::u16string get_description() override;
	virtual bool update();
	virtual Vision_list* clone();
	void apply_visitor(Visitor_generic& visitor) override;

	virtual void equip(Object_interaction* item);
	virtual void unequip(Object_interaction* item);

	virtual void save();
	virtual void load();

	Packer_generic& get_packer() override
	{
		return Packer<Vision_list>::Instance();
	}

	constexpr static auto properties() {
		return Interaction_list::properties();
	}

private:
	void update_list(Object_interaction* list);
};

class Vision_component :public Interaction_list
{
public:
 
	AI_FOV m_basic_value;
	AI_FOV m_value;
	Vision_component();
	std::u16string get_description() override;
	virtual bool update();
	virtual Vision_component* clone();
	void apply_visitor(Visitor_generic& visitor) override;

	virtual void save();
	virtual void load();

	Packer_generic& get_packer() override
	{
		return Packer<Vision_component>::Instance();
	}

	constexpr static auto properties() {
		return make_union(std::make_tuple(
			make_property(&Vision_component::m_basic_value, u"basic_value")
		), Interaction_list::properties());
	}

private:
	void update_list(Object_interaction* list);
};

class Tag_list :public  Interaction_list
{
public:

	Tag_list();

	void apply_visitor(Visitor_generic& visitor) override;

	virtual Tag_list* clone();

	virtual bool update();
	virtual void save();
	virtual void load();

	Packer_generic& get_packer() override
	{
		return Packer<Tag_list>::Instance();
	}

	constexpr static auto properties() {
		return Interaction_list::properties();
	}

private:
	
	class Update_visitor : public Visitor
	{
	public:
		
		bool was_changed;

		Update_visitor();
		void visit(Object_interaction& value) override;

	};

};

class Parts_list: public Interaction_list
{
public:

	Parts_list();
	virtual Parts_list* clone();
	virtual bool update();
	virtual void save();
	virtual void load();

	void apply_visitor(Visitor_generic& visitor) override;
	virtual void equip(Object_interaction* item);

	Packer_generic& get_packer() override
	{
		return Packer<Parts_list>::Instance();
	}

	constexpr static auto properties() {
		return Interaction_list::properties();
	}

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

	void apply_visitor(Visitor_generic& visitor) override;

	Packer_generic& get_packer() override
	{
		return Packer<Action_list>::Instance();
	}

	constexpr static auto properties() {
		return Interaction_list::properties();
	}
};

class Slot_set_state: public Object_interaction
{
public:

	object_state_e m_value;
	Slot_set_state() { m_interaction_message_type = interaction_message_type_e::set_attribute_map; };
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
	virtual bool on_turn() { return false; };
	std::u16string get_description() override { return u"set_state"; };
	virtual Slot_set_state* clone();
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
	void reset_serialization_index() override;

	Packer_generic& get_packer() override
	{
		return Packer<Interaction_slot>::Instance();
	}

	constexpr static auto properties() {
		return std::make_tuple(
			make_property(&Interaction_slot::m_value, u"value")
		);
	}

};

class Interaction_copyist :public Interaction_slot
{
public:

	interaction_e m_subtype;
	Interaction_copyist();
	std::u16string get_description() override;
	virtual Object_interaction* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void save();
	virtual void load();
};


class Interaction_prefix :public Interaction_slot
{
public:

	effect_prefix_e m_subtype;
	Interaction_prefix();
	std::u16string get_description() override;
	virtual Interaction_prefix* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void save();
	virtual void load();
};

class Interaction_addon :public Interaction_slot
{
public:

	interaction_e m_subtype;
	Interaction_addon();
	std::u16string get_description() override;
	virtual Object_interaction* clone();
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
	std::u16string get_description() override;
	virtual Object_interaction* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void save();
	virtual void load();

	Packer_generic& get_packer() override
	{
		return Packer<Interaction_time>::Instance();
	}

	constexpr static auto properties() {
		return make_union(std::make_tuple(
			make_property(&Interaction_time::m_turn, u"m_turn")
		), Interaction_slot::properties());
	}

};

class Interaction_timer :public Interaction_slot
{
public:

	int m_turn;
	int m_period;
	Interaction_timer();
	virtual bool on_turn();
	std::u16string get_description() override;
	virtual Object_interaction* clone();
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
	Effect* clone() override;
	std::u16string get_description() override;
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void save();
	virtual void load();

	Packer_generic& get_packer() override
	{
		return Packer<Effect>::Instance();
	}

	constexpr static auto properties() {
		return std::make_tuple(
			make_property(&Effect::m_value, u"value"),
			make_property(&Effect::m_subtype, u"subtype")
		);
	}

};

class Object_tag :public Object_interaction
{
public:

	object_tag_e m_type;
	Object_tag(object_tag_e key);
	std::u16string get_description() override;
	virtual Object_tag* clone() = 0;
	virtual bool on_turn();

	constexpr static auto properties()
	{
		return std::make_tuple(make_property(&Object_tag::m_type, u"type"));
	}
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

		void apply_visitor(Visitor_generic& visitor) override;

		// ��� ��������� iSerializable
		Packer_generic& get_packer() override
		{
			return Packer<Mortal>::Instance();
		}

		/*constexpr static auto properties() { return Object_tag::properties(); }*/
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
		Label():Object_tag(object_tag_e::none){};
		Label(object_tag_e type);
		virtual Label* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);

		void apply_visitor(Visitor_generic& visitor) override;

		virtual void save();
		virtual void load();

		// ��� ��������� iSerializable
		Packer_generic& get_packer() override
		{
			return Packer<Label>::Instance();
		}

		constexpr static auto properties() { return Object_tag::properties(); }
	};

	class Equippable : public Object_tag
	{
	public:

		Object_interaction* m_value;
		Interaction_list* m_condition;

		Equippable();
		virtual Equippable* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);
		void reset_serialization_index() override;

		virtual void save();
		virtual void load();

		// ��� ��������� iSerializable
		Packer_generic& get_packer() override
		{
			return Packer<Equippable>::Instance();
		}

		constexpr static auto properties() {
			return make_union(std::make_tuple(
				make_property(&Equippable::m_value, u"value"),
				make_property(&Equippable::m_condition, u"condition")
			), Object_tag::properties());
		}

	};

	class Requirements_to_object : public Object_tag
	{
	public:

		Object_interaction* m_value;
		bool m_result;

		Requirements_to_object();
		virtual Requirements_to_object* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);
		void apply_visitor(Visitor_generic& visitor) override;

		virtual void save();
		virtual void load();
		void reset_serialization_index() override;

		Packer_generic& get_packer() override
		{
			return Packer<Requirements_to_object>::Instance();
		}

		constexpr static auto properties() {
			return make_union(std::make_tuple(
				make_property(&Requirements_to_object::m_value, u"value")
			), Object_tag::properties());
		}

	};

	class Can_transfer_object : public Object_tag
	{
	public:

		Object_interaction* m_value;
		bool m_result;

		Can_transfer_object();
		virtual Can_transfer_object* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);
		void reset_serialization_index() override;

		virtual void save();
		virtual void load();

		Packer_generic& get_packer() override
		{
			return Packer<Can_transfer_object>::Instance();
		}

		constexpr static auto properties() {
			return make_union(std::make_tuple(
				make_property(&Can_transfer_object::m_value, u"value")
			), Object_tag::properties());
		}
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
	std::u16string get_description() override;
	virtual Instruction_slot_link* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void save();
	virtual void load();

	Packer_generic& get_packer() override
	{
		return Packer<Instruction_slot_link>::Instance();
	}

	constexpr static auto properties() {
		return make_union(std::make_tuple(
			make_property(&Instruction_slot_link::m_subtype, u"subtype")
		), Interaction_slot::properties());
	}
};

class Instruction_slot_parameter :public Interaction_slot
{
public:

	Parameter* m_parameter;
	bool m_result;

	Instruction_slot_parameter();
	std::u16string get_description() override;
	virtual Object_interaction* clone();
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
	std::u16string get_description() override { return nullptr; };
	virtual bool on_turn() { return false; };
	virtual Object_interaction* clone() { return nullptr; };
	virtual void apply_effect(GameObject* unit, Object_interaction* object) {};

	virtual void save() {};
	virtual void load() {};

};

class Instruction_check_tag :public Object_interaction
{
public:

	object_tag_e m_value;

	Instruction_check_tag();
	std::u16string get_description() override { return nullptr; };
	virtual bool on_turn() { return false; };
	virtual Instruction_check_tag* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);


	void apply_visitor(Visitor_generic& visitor) override;

	virtual void save() {};
	virtual void load() {};

	Packer_generic& get_packer() override
	{
		return Packer<Instruction_check_tag>::Instance();
	}

	constexpr static auto properties() {
		return std::make_tuple(
			make_property(&Instruction_check_tag::m_value, u"value")
		);
	}

};

class Instruction_check_part_type :public Object_interaction
{
public:

	body_part_e m_value;

	Instruction_check_part_type();
	std::u16string get_description() override { return nullptr; };
	virtual bool on_turn() { return false; };
	virtual Instruction_check_part_type* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void save() {};
	virtual void load() {};

	Packer_generic& get_packer() override
	{
		return Packer<Instruction_check_part_type>::Instance();
	}

	constexpr static auto properties() {
		return std::make_tuple(
			make_property(&Instruction_check_part_type::m_value, u"value")
		);
	}

};

class Instruction_arg_extract : public Object_interaction
{
public:

	Object_interaction* m_value;
	std::size_t m_index;
	bool m_result;

	Instruction_arg_extract();
	virtual Instruction_arg_extract* clone();
	std::u16string get_description() override { return nullptr; };
	virtual bool on_turn() { return false; };
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
	void reset_serialization_index() override;

	virtual void save();
	virtual void load();

	Packer_generic& get_packer() override
	{
		return Packer<Instruction_arg_extract>::Instance();
	}

	constexpr static auto properties() {
		return std::make_tuple(
			make_property(&Instruction_arg_extract::m_value, u"value"),
			make_property(&Instruction_arg_extract::m_index, u"index")
		);
	}
};


class Instruction_get_owner : public Object_interaction
{
public:

	Object_interaction* m_value;
	bool m_result;

	Instruction_get_owner();
	virtual Instruction_get_owner* clone();
	std::u16string get_description() override { return nullptr; };
	virtual bool on_turn() { return false; };
	virtual void apply_effect(GameObject* unit, Object_interaction* object) ;
	void reset_serialization_index() override;

	virtual void save() {};
	virtual void load() {};

	Packer_generic& get_packer() override
	{
		return Packer<Instruction_get_owner>::Instance();
	}

	constexpr static auto properties() {
		return std::make_tuple(
			make_property(&Instruction_get_owner::m_value, u"value")
		);
	}
};

class Instruction_get_owner_top : public Instruction_get_owner
{
public:

	Instruction_get_owner_top();
	Instruction_get_owner_top* clone() override;
	void apply_effect(GameObject* unit, Object_interaction* object) override;

	Packer_generic& get_packer() override
	{
		return Packer<Instruction_get_owner_top>::Instance();
	}

	constexpr static auto properties() {
		return Instruction_get_owner::properties();
	}
};

class Instruction_check_owner_type :public Object_interaction
{
public:

	entity_e m_value;

	Instruction_check_owner_type();
	std::u16string get_description() override { return nullptr; };
	virtual bool on_turn() { return false; };
	virtual Instruction_check_owner_type* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	virtual void save() {};
	virtual void load() {};

	Packer_generic& get_packer() override
	{
		return Packer<Instruction_check_owner_type>::Instance();
	}

	constexpr static auto properties() {
		return std::make_tuple(
			make_property(&Instruction_check_owner_type::m_value, u"value")
		);
	}

};