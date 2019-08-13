#ifndef EFFECT_H
#define	EFFECT_H

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
	virtual bool update();
	Interaction_list* clone() override;
	void apply_effect(GameObject* unit, Object_interaction* object) override;

	virtual void add(Object_interaction* item) { m_items.push_back(item); };
	virtual void remove(Object_interaction* item) { m_items.remove(item); };
	virtual void equip(Object_interaction* item) { m_items.push_back(item); };
	virtual void unequip(Object_interaction* item) { m_items.remove(item); };

	void do_predicate(Visitor& helper) override;
	void apply_visitor(Visitor_generic& visitor) override;

	void reset_serialization_index() override;

	// Для поддержки iJSONSerializable
	Packer_generic& get_packer() override;

	constexpr static auto properties() 
	{
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
	bool update() override;
	Parameter_list* clone() override;
	void apply_visitor(Visitor_generic& visitor) override;

	Packer_generic& get_packer() override;

	constexpr static auto properties() 
	{
		return make_union(
			Interaction_list::properties(),
			std::make_tuple(
				make_property(&Parameter_list::m_subtype, u"subtype"),
				make_property(&Parameter_list::m_basic_value, u"basic_value"),
				make_property(&Parameter_list::m_basic_limit, u"basic_limit")
			));
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
	bool update() override;
	Vision_list* clone() override;
	void apply_visitor(Visitor_generic& visitor) override;

	void equip(Object_interaction* item) override;
	void unequip(Object_interaction* item) override;

	Packer_generic& get_packer() override;

	constexpr static auto properties() { return Interaction_list::properties(); }

private:
	void update_list(Object_interaction* list);
};

class Vision_component :public Interaction_list
{
public:
 
	AI_FOV m_basic_value;
	AI_FOV m_value;
	Vision_component();
	virtual bool update();
	virtual Vision_component* clone();
	void apply_visitor(Visitor_generic& visitor) override;

	Packer_generic& get_packer() override;

	constexpr static auto properties() 
	{
		return make_union(
			Interaction_list::properties(),
			std::make_tuple(make_property(&Vision_component::m_basic_value, u"basic_value")
			));
	}

private:
	void update_list(Object_interaction* list);
};

class Tag_list :public  Interaction_list
{
public:

	Tag_list();

	void apply_visitor(Visitor_generic& visitor) override;

	Tag_list* clone() override;

	bool update() override;

	Packer_generic& get_packer() override;

	constexpr static auto properties() { return Interaction_list::properties(); }

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
	Parts_list* clone() override;
	bool update() override;

	void apply_visitor(Visitor_generic& visitor) override;
	void equip(Object_interaction* item) override;

	Packer_generic& get_packer() override;

	constexpr static auto properties() { return Interaction_list::properties(); }


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
	Interaction_list* clone() override;

	void apply_visitor(Visitor_generic& visitor) override;

	Packer_generic& get_packer() override;

	constexpr static auto properties() { return Interaction_list::properties(); }
};

class Slot_set_state: public Object_interaction
{
public:

	object_state_e m_value;
	Slot_set_state() { m_interaction_message_type = interaction_message_type_e::set_attribute_map; };
	void apply_effect(GameObject* unit, Object_interaction* object) override;
	Slot_set_state* clone() override;
	virtual void do_predicate(predicate func) { func(this); };

	Packer_generic& get_packer() override;
};

class Interaction_slot :public Object_interaction
{
public:

	Object_interaction* m_value;
	Interaction_slot();
	void do_predicate(Visitor& helper) override;
	void reset_serialization_index() override;

	Packer_generic& get_packer() override = 0;
	/*{
		return Packer<Interaction_slot>::Instance();
	}*/

	constexpr static auto properties() 
	{
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
	Object_interaction* clone() override;
	void apply_effect(GameObject* unit, Object_interaction* object) override;

	Packer_generic& get_packer() override;
};


class Interaction_prefix :public Interaction_slot
{
public:

	effect_prefix_e m_subtype;
	Interaction_prefix();
	Interaction_prefix* clone() override;
	void apply_effect(GameObject* unit, Object_interaction* object) override;

	Packer_generic& get_packer() override;
};

class Interaction_addon :public Interaction_slot
{
public:

	interaction_e m_subtype;
	Interaction_addon();
	Object_interaction* clone() override;
	void apply_effect(GameObject* unit, Object_interaction* object) override;

	Packer_generic& get_packer() override;
};

class Interaction_time :public Interaction_slot
{
public:

	int m_turn;
	Interaction_time();
	virtual Object_interaction* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	Packer_generic& get_packer() override;

	constexpr static auto properties() {
		return make_union(
			Interaction_slot::properties(),
			std::make_tuple(
				make_property(&Interaction_time::m_turn, u"m_turn")
			));
	}

};

class Interaction_timer :public Interaction_slot
{
public:

	int m_turn;
	int m_period;
	Interaction_timer();
	virtual Object_interaction* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	Packer_generic& get_packer() override;
};

class Effect :public Object_interaction
{
public:

	int m_value;
	effect_e m_subtype;
	Effect();
	Effect* clone() override;
	void apply_effect(GameObject* unit, Object_interaction* object) override;

	Packer_generic& get_packer() override;

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
	Object_tag* clone() override = 0;

	constexpr static auto properties() { return std::make_tuple(make_property(&Object_tag::m_type, u"type")); }
};

namespace ObjectTag
{
	class Poison_resist :public Object_tag
	{
	public:
		Poison_resist();
		Poison_resist* clone() override;
		void apply_effect(GameObject* unit, Object_interaction* object) override;

		Packer_generic& get_packer() override;
	};

	class Mortal :public Object_tag
	{
	public:
		Mortal();
		Mortal* clone() override;
		void apply_effect(GameObject* unit, Object_interaction* object) override;

		void apply_visitor(Visitor_generic& visitor) override;

		// Для поддержки iSerializable
		Packer_generic& get_packer() override;

		/*constexpr static auto properties() { return Object_tag::properties(); }*/
	};

	class Purification_from_poison : public Object_tag
	{
	public:
		Purification_from_poison();
		virtual Purification_from_poison* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);

		Packer_generic& get_packer() override;
	};

	class Activator : public Object_tag
	{
	public:
		Activator();
		std::list<GameObject*> m_link;
		virtual Activator* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);

		void reset_serialization_index() override;
		
		Packer_generic& get_packer() override;
	};

	class Fast_move : public Object_tag
	{
	public:
		Fast_move();
		virtual Fast_move* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);

		Packer_generic& get_packer() override;
	};

	class Label : public Object_tag
	{
	public:
		Label() :Object_tag(object_tag_e::none) {};
		Label(object_tag_e type);
		virtual Label* clone();
		virtual void apply_effect(GameObject* unit, Object_interaction* object);

		void apply_visitor(Visitor_generic& visitor) override;

		// Для поддержки iSerializable
		Packer_generic& get_packer() override;

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

		// Для поддержки iSerializable
		Packer_generic& get_packer() override;

		constexpr static auto properties()
		{
			return make_union(
				Object_tag::properties(),
				std::make_tuple(
					make_property(&Equippable::m_value, u"value"),
					make_property(&Equippable::m_condition, u"condition")
				));
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

		void reset_serialization_index() override;

		Packer_generic& get_packer() override;

		constexpr static auto properties()
		{
			return make_union(
				Object_tag::properties(),
				std::make_tuple(
					make_property(&Requirements_to_object::m_value, u"value")
				));
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

		Packer_generic& get_packer() override;

		constexpr static auto properties() {
			return make_union(
				Object_tag::properties(),
				std::make_tuple(
					make_property(&Can_transfer_object::m_value, u"value")
				));
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
	virtual Instruction_slot_link* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	Packer_generic& get_packer() override;

	constexpr static auto properties()
	{
		return make_union(
			Interaction_slot::properties(),
			std::make_tuple(
				make_property(&Instruction_slot_link::m_subtype, u"subtype")
			));
	}
};

class Instruction_slot_parameter :public Interaction_slot
{
public:

	Parameter* m_parameter;
	bool m_result;

	Instruction_slot_parameter();
	virtual Object_interaction* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	Packer_generic& get_packer() override;
};

class Instruction_game_owner :public Object_interaction
{
public:

	Game_object_owner* m_value;
	bool m_result;

	Instruction_game_owner();
	virtual Object_interaction* clone() { return nullptr; };
	virtual void apply_effect(GameObject* unit, Object_interaction* object) {};

	Packer_generic& get_packer() override;
};

class Instruction_check_tag :public Object_interaction
{
public:

	object_tag_e m_value;

	Instruction_check_tag();
	virtual Instruction_check_tag* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);


	void apply_visitor(Visitor_generic& visitor) override;

	Packer_generic& get_packer() override;

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
	virtual Instruction_check_part_type* clone();
	virtual void apply_effect(GameObject* unit, Object_interaction* object);

	Packer_generic& get_packer() override;

	constexpr static auto properties() 
	{
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
	virtual void apply_effect(GameObject* unit, Object_interaction* object);
	void reset_serialization_index() override;

	Packer_generic& get_packer() override;

	constexpr static auto properties() 
	{
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
	Instruction_get_owner* clone() override;
	void apply_effect(GameObject* unit, Object_interaction* object) override;
	void reset_serialization_index() override;

	Packer_generic& get_packer() override;

	constexpr static auto properties() 
	{
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

	Packer_generic& get_packer() override;

	constexpr static auto properties() { return Instruction_get_owner::properties(); }
};

class Instruction_check_owner_type :public Object_interaction
{
public:

	entity_e m_value;

	Instruction_check_owner_type();
	Instruction_check_owner_type* clone() override;
	void apply_effect(GameObject* unit, Object_interaction* object) override;

	Packer_generic& get_packer() override;

	constexpr static auto properties()
	{
		return std::make_tuple(
			make_property(&Instruction_check_owner_type::m_value, u"value")
		);
	}

};

#endif //EFFECT_H