#include "Effect.h"
#include "Application.h"

// Effect

Effect::Effect()
    : m_value(0)
    , m_subtype()
{
}

Effect* Effect::clone()
{
	auto effect = new Effect();
    effect->m_subtype = m_subtype;
    effect->m_value = m_value;
    return effect;
}

void Effect::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (m_subtype) {
    case effect_e::value: {
        Parameter_list* item = static_cast<Parameter_list*>(object);
        item->m_basic_value = item->m_basic_value + m_value;
        if (item->m_basic_value <= 1) {
            item->m_basic_value = 0;
            Interaction_copyist* copyist = new Interaction_copyist();
            copyist->m_subtype = item->m_subtype;
            copyist->m_value = clone();
            Interaction_prefix* prefix = new Interaction_prefix();
            prefix->m_value = copyist;
            prefix->m_subtype = effect_prefix_e::parameter_change;
            prefix->apply_effect(unit, object);
        }
        break;
    }
    case effect_e::limit: {
        Parameter_list* item = static_cast<Parameter_list*>(object);
        item->m_basic_limit = item->m_basic_limit + m_value;
        break;
    }
    }
}

iPacker& Effect::get_packer()
{
    return Packer<Effect>::instance();
}

interaction_message_type_e Effect::get_interaction_message_type()
{
    return interaction_message_type_e::effect;
}

// ObjectTag::Poison_resist

ObjectTag::Poison_resist::Poison_resist()
    : Object_tag(object_tag_e::poison_resist) {};

ObjectTag::Poison_resist* ObjectTag::Poison_resist::clone()
{
	const auto effect = new ObjectTag::Poison_resist();
    return effect;
}

void ObjectTag::Poison_resist::apply_effect(GameObject* unit, Object_interaction* object)
{
	auto prefix = static_cast<Interaction_prefix*>(object);
    switch (prefix->m_subtype) {
    case effect_prefix_e::poison_damage: {
        static_cast<Effect*>(prefix->m_value)->m_value /= 2;
        break;
    }
    }
}

iPacker& ObjectTag::Poison_resist::get_packer()
{
    return Packer<Poison_resist>::instance();
}

// ObjectTag::Mortal

ObjectTag::Mortal::Mortal()
    : Object_tag(object_tag_e::mortal) {};

ObjectTag::Mortal* ObjectTag::Mortal::clone()
{
	auto effect = new ObjectTag::Mortal();
    return effect;
}

void ObjectTag::Mortal::apply_effect(GameObject* unit, Object_interaction* object)
{
    Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
    switch (prefix->m_subtype) {
    case effect_prefix_e::parameter_change: {
        Interaction_copyist* copyist = static_cast<Interaction_copyist*>(prefix->m_value);
        if (copyist->m_subtype == interaction_e::health) {
            auto feat = unit->get_parameter(interaction_e::health);
            if (feat) {
                if (feat->m_basic_value == 0) {
                    unit->set_state(object_state_e::dead);
                }
            }
        }
        break;
    }
    }
}

// ObjectTag::Purification_from_poison

ObjectTag::Purification_from_poison::Purification_from_poison()
    : Object_tag(object_tag_e::purification_from_poison) {};

ObjectTag::Purification_from_poison* ObjectTag::Purification_from_poison::clone()
{
	auto effect = new ObjectTag::Purification_from_poison();
    return effect;
}

void ObjectTag::Purification_from_poison::apply_effect(GameObject* unit, Object_interaction* object)
{
    Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
    switch (prefix->m_subtype) {
    case effect_prefix_e::poison_damage: {
        Parameter_list* p = unit->get_parameter(interaction_e::poison);
        if (p) {
            Effect* e = static_cast<Effect*>(prefix->m_value);
            p->m_basic_value -= e->m_value;
            if (p->m_basic_value > p->m_basic_limit) {
                e->m_value = p->m_basic_limit - p->m_basic_value;
                p->m_basic_value = p->m_basic_limit;
            } else {
                e->m_value = 0;
            }
        }
        break;
    }
    }
}

iPacker& ObjectTag::Purification_from_poison::get_packer()
{
    return Packer<Purification_from_poison>::instance();
}

// ObjectTag::Activator::Activator

ObjectTag::Activator::Activator()
    : Object_tag(object_tag_e::activator) {};

ObjectTag::Activator* ObjectTag::Activator::clone()
{
	auto effect = new ObjectTag::Activator();
    effect->m_link = m_link;
    return effect;
}

void ObjectTag::Activator::apply_effect(GameObject* unit, Object_interaction* object)
{
    Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
    switch (prefix->m_subtype) {
    case effect_prefix_e::state_change: {
        Slot_set_state* e = static_cast<Slot_set_state*>(prefix->m_value);
        if (e) {
            for (auto l = m_link.begin(); l != m_link.end(); ++l) {
                (*l)->set_state(e->m_value);
            }
        }
        break;
    }
    }
}

void ObjectTag::Activator::reset_serialization_index()
{
    m_serialization_index = 0;
    for (auto item = m_link.begin(); item != m_link.end(); ++item) {
        if ((*item)->m_serialization_index > 0) {
            (*item)->reset_serialization_index();
        }
    }
}

iPacker& ObjectTag::Activator::get_packer()
{
    return Packer<Activator>::instance();
}

// ObjectTag::Fast_move

ObjectTag::Fast_move::Fast_move()
    : Object_tag(object_tag_e::fast_move) {};

ObjectTag::Fast_move* ObjectTag::Fast_move::clone()
{
	auto effect = new ObjectTag::Fast_move();
    return effect;
}

void ObjectTag::Fast_move::apply_effect(GameObject* unit, Object_interaction* object)
{
	auto prefix = static_cast<Interaction_prefix*>(object);
    switch (prefix->m_subtype) {
    case effect_prefix_e::action: {
	    auto e = static_cast<Action_wrapper*>(prefix->m_value);
        if (e) {
            if (e->m_action->m_kind == action_e::move) {
                if (e->m_decay != 1) {
                    e->m_decay /= 2;
                }
            }
        }
        break;
    }
    }
}

iPacker& ObjectTag::Fast_move::get_packer()
{
    return Packer<Fast_move>::instance();
}

// ObjectTag::Label

ObjectTag::Label::Label(object_tag_e type)
    : Object_tag(type) {};

ObjectTag::Label* ObjectTag::Label::clone()
{
	auto effect = new ObjectTag::Label(m_type);
    return effect;
}

void ObjectTag::Label::apply_effect(GameObject* unit, Object_interaction* object) {}

void ObjectTag::Label::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
};

iPacker& ObjectTag::Label::get_packer()
{
    return Packer<Label>::instance();
}

Interaction_list* Effect_functions::create_feature_list(feature_list_type_e key, interaction_e name)
{
    Interaction_list* result;
    switch (key) {
    case feature_list_type_e::generic: {
        result = new Interaction_list();
        break;
    }
    case feature_list_type_e::tag: {
        result = new Tag_list();
        break;
    }
    case feature_list_type_e::action: {
        result = new Action_list();
        break;
    }
    case feature_list_type_e::parameter: {
        result = new Parameter_list(name);
        break;
    }
    case feature_list_type_e::parts: {
        result = new Parts_list();
        break;
    }
    case feature_list_type_e::vision: {
        result = new Vision_list();
        break;
    }
    case feature_list_type_e::vision_component: {
        result = new Vision_component();
        break;
    }
    }
    return result;
}

// Interaction_copyist

Instruction_slot_link::Instruction_slot_link()
{
	m_enable = false;
}

Instruction_slot_link* Instruction_slot_link::clone()
{
	auto effect = new Instruction_slot_link();
    effect->m_subtype = m_subtype;
    effect->m_value = m_value->clone();
    effect->m_enable = m_enable;
    return effect;
}

void Instruction_slot_link::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {
    case interaction_message_type_e::instruction_game_owner: {
        Instruction_game_owner* parameter = static_cast<Instruction_game_owner*>(object);
        Object_part* part = static_cast<Object_part*>(parameter->m_value);
        auto i = part->m_attributes.get_list(m_subtype);
        if (i) {
            if (m_enable) {
                i->remove(m_value);
                m_enable = false;
            } else {
                i->add(m_value);
                m_enable = true;
            }
        }
        break;
    }
    case interaction_message_type_e::instruction_result: {
        Instruction_result* ir = static_cast<Instruction_result*>(object);
        Parameter_argument_t& a = ir->m_value;
        switch (a.kind) {
        case type_e::gameobject: {
            auto i = a.m_object->m_active_state->get_list(m_subtype);
            if (i) {
                if (m_enable) {
                    i->remove(m_value);
                    m_enable = false;
                } else {
                    i->add(m_value);
                    m_enable = true;
                }
            }
            break;
        }
        case type_e::object_owner: {
            Interaction_list* i = nullptr;
            switch (a.m_owner->m_kind) {
            case entity_e::body_part: {
                i = static_cast<Object_part*>(a.m_owner)->m_attributes.get_list(m_subtype);
                break;
            }
            case entity_e::game_object: {
                i = static_cast<GameObject*>(a.m_owner)->m_active_state->get_list(m_subtype);
                break;
            }
            }
            if (i) {
                if (m_enable) {
                    i->remove(m_value);
                    m_enable = false;
                } else {
                    i->add(m_value);
                    m_enable = true;
                }
            }
            break;
        }
        }
        break;
    }
    default: {
        Instruction_slot_parameter* parameter = static_cast<Instruction_slot_parameter*>(object);
        Parameter& p(*(parameter->m_parameter));
        Object_part* part = static_cast<Object_part*>(p[1].m_object->m_owner);
        auto i = part->m_attributes.get_list(m_subtype);
        if (i) {
            if (m_enable) {
                i->remove(m_value);
                m_enable = false;
            } else {
                i->add(m_value);
                m_enable = true;
            }
        }
        break;
    }
    }
}

iPacker& Instruction_slot_link::get_packer()
{
    return Packer<Instruction_slot_link>::instance();
}

interaction_message_type_e Instruction_slot_link::get_interaction_message_type()
{
    return interaction_message_type_e::instruction_slot_link;
}

// Instruction_slot_parameter

Instruction_slot_parameter::Instruction_slot_parameter()
{
	m_value = nullptr;

}

Object_interaction* Instruction_slot_parameter::clone()
{
	auto effect = new Instruction_slot_parameter();
    //effect->m_value = m_value->clone();
    return effect;
}

void Instruction_slot_parameter::apply_effect(GameObject* unit, Object_interaction* object)
{
    /*if (m_enable)
	{
		m_value->apply_effect(unit, object);
		m_enable = false;
	}
	else
	{
		Object_part* o = static_cast<Object_part*>(object);
		if (o->m_object_state.get_stat(m_subtype))
		{
			m_value->apply_effect(unit, object);
			m_enable = true;
		}
	}*/
}

iPacker& Instruction_slot_parameter::get_packer()
{
    return Packer<Instruction_slot_parameter>::instance();
}

interaction_message_type_e Instruction_slot_parameter::get_interaction_message_type()
{
    return interaction_message_type_e::instruction_slot_parameter;
}

// ObjectTag::Equippable

ObjectTag::Equippable::Equippable()
    : Object_tag(object_tag_e::equippable)
{
    m_value = nullptr;
    m_condition = nullptr;
};

ObjectTag::Equippable* ObjectTag::Equippable::clone()
{
	auto result = new ObjectTag::Equippable;
    if (m_value) {
        result->m_value = m_value->clone();
    }
    result->m_condition = m_condition->clone();
    return result;
}

void ObjectTag::Equippable::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {
    case interaction_message_type_e::instruction_slot_parameter: {
	    auto parameter = static_cast<Instruction_slot_parameter*>(object);
        Parameter& p(*parameter->m_parameter);

        switch (p[2].m_owner->m_kind) {
        case entity_e::cell: {
            break;
        }
        case entity_e::inventory_cell: {
            break;
        }
        case entity_e::body_part: {
            Instruction_game_owner* i = new Instruction_game_owner();
            i->m_value = p[2].m_owner;
            m_condition->apply_effect(unit, i);
            if (!i->m_result) {
                return;
            };
            if (p[2].m_owner->m_kind == entity_e::body_part) {
                Object_part* part = static_cast<Object_part*>(p[2].m_owner);
                Object_tag* t = part->m_attributes.get_tag(object_tag_e::requirements_to_object);
                if (t) {
                    i->m_result = false;
                    i->m_value = p[1].m_object;
                    t->apply_effect(unit, i);
                    if (!i->m_result) {
                        return;
                    };
                }
            }
            break;
        }
        }

        switch (p[1].m_object->m_owner->m_kind) {
        case entity_e::cell: {
            //static_cast<MapCell*>(p->m_object->m_owner)->m_items.remove(p->m_object);
            static_cast<MapCell*>(p[1].m_object->m_owner)->m_map->remove_object(p[1].m_object);
            break;
        }
        case entity_e::inventory_cell: {
            static_cast<Inventory_cell*>(p[1].m_object->m_owner)->m_item = nullptr;
            break;
        }
        case entity_e::body_part: {
            Object_part* part = static_cast<Object_part*>(p[1].m_object->m_owner);
            part->m_item = nullptr;
            part = nullptr;
            m_value->apply_effect(unit, object);
            break;
        }
        }

        switch (p[2].m_owner->m_kind) {
        case entity_e::cell: {
            //static_cast<MapCell*>(p->m_owner)->m_items.push_back(p->m_object);
            static_cast<MapCell*>(p[2].m_owner)->m_map->add_object(p[1].m_object, *static_cast<MapCell*>(p[2].m_owner));
            p[1].m_object->m_owner = p[2].m_owner;
            break;
        }
        case entity_e::inventory_cell: {
            static_cast<Inventory_cell*>(p[2].m_owner)->m_item = p[1].m_object;
            p[1].m_object->m_owner = p[2].m_owner;
            break;
        }
        case entity_e::body_part: {
            Object_part* part = static_cast<Object_part*>(p[2].m_owner);
            part->m_item = p[1].m_object;
            p[1].m_object->m_owner = part;
            if (m_value) {
                m_value->apply_effect(unit, object);
            }
            break;
        }
        }
        break;
    }
    }
};

void ObjectTag::Equippable::reset_serialization_index()
{

    m_serialization_index = 0;
    if (m_value && m_value->m_serialization_index != 0) {
        m_value->reset_serialization_index();
    }
    if (m_condition && m_condition->m_serialization_index != 0) {
        m_condition->reset_serialization_index();
    }
}

iPacker& ObjectTag::Equippable::get_packer()
{
    return Packer<Equippable>::instance();
}

// ObjectTag::Requirements_to_object

ObjectTag::Requirements_to_object::Requirements_to_object()
    : Object_tag(object_tag_e::requirements_to_object)
{
    m_value = nullptr;
    m_result = false;
};

ObjectTag::Requirements_to_object* ObjectTag::Requirements_to_object::clone()
{
	auto result = new ObjectTag::Requirements_to_object;
    result->m_value = m_value->clone();
    result->m_result = m_result;
    return result;
}

void ObjectTag::Requirements_to_object::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {
    case interaction_message_type_e::instruction_game_owner: {
        m_value->apply_effect(unit, object);
        const auto i = static_cast<Instruction_game_owner*>(object);
        m_result = i->m_result;
        break;
    }
    default: {
        break;
    }
    }
}

void ObjectTag::Requirements_to_object::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
};

void ObjectTag::Requirements_to_object::reset_serialization_index()
{
    m_serialization_index = 0;
    if (m_value) {
        m_value->reset_serialization_index();
    }
}

iPacker& ObjectTag::Requirements_to_object::get_packer()
{
    return Packer<Requirements_to_object>::instance();
}

// ObjectTag::Can_transfer_object

ObjectTag::Can_transfer_object::Can_transfer_object()
    : Object_tag(object_tag_e::can_transfer_object)
{
    m_value = nullptr;
    m_result = false;
};

ObjectTag::Can_transfer_object* ObjectTag::Can_transfer_object::clone()
{
	auto result = new ObjectTag::Can_transfer_object;
    result->m_value = m_value->clone();
    result->m_result = m_result;
    return result;
}

void ObjectTag::Can_transfer_object::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {
    case interaction_message_type_e::instruction_slot_parameter: {
        m_value->apply_effect(unit, object);
        /*Instruction_game_owner* i = static_cast<Instruction_game_owner*>(object);
		m_result = i->m_result;*/
        break;
    }
    default: {
        break;
    }
    }
}

void ObjectTag::Can_transfer_object::reset_serialization_index()
{
    m_serialization_index = 0;
    if (m_value && m_value->m_serialization_index != 0) {
        m_value->reset_serialization_index();
    }
};

iPacker& ObjectTag::Can_transfer_object::get_packer()
{
    return Packer<Can_transfer_object>::instance();
}

// Instruction_game_owner

Instruction_game_owner::Instruction_game_owner()
{
	m_value = nullptr;
	m_result = false;
}
;

// Instruction_check_tag

iPacker& Instruction_game_owner::get_packer()
{
    return Packer<Instruction_game_owner>::instance();
}

Instruction_check_tag::Instruction_check_tag()
{
} ;

Instruction_check_tag* Instruction_check_tag::clone()
{
	auto result = new Instruction_check_tag();
    result->m_value = m_value;
    return result;
}

void Instruction_check_tag::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {

    case interaction_message_type_e::instruction_game_owner: {
        Instruction_game_owner* i = static_cast<Instruction_game_owner*>(object);
        switch (i->m_value->m_kind) {
        case entity_e::game_object: {
            GameObject* obj = static_cast<GameObject*>(i->m_value);
            i->m_result = obj->get_stat(m_value);
            break;
        }
        }
        break;
    }
    }
}

void Instruction_check_tag::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

iPacker& Instruction_check_tag::get_packer()
{
    return Packer<Instruction_check_tag>::instance();
}

interaction_message_type_e Instruction_check_tag::get_interaction_message_type()
{
    return interaction_message_type_e::instruction_check_tag;
};

// Instruction_check_part_type

Instruction_check_part_type::Instruction_check_part_type()
{
};

Instruction_check_part_type* Instruction_check_part_type::clone()
{
	auto result = new Instruction_check_part_type();
    result->m_value = m_value;
    return result;
}

void Instruction_check_part_type::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {
    case interaction_message_type_e::instruction_slot_parameter: {
        break;
    }
    default: {
        Instruction_game_owner* i = static_cast<Instruction_game_owner*>(object);
        Object_part* op = static_cast<Object_part*>(i->m_value);
        i->m_result = (op->m_part_kind == m_value);
        break;
    }
    }
}

iPacker& Instruction_check_part_type::get_packer()
{
    return Packer<Instruction_check_part_type>::instance();
}

interaction_message_type_e Instruction_check_part_type::get_interaction_message_type()
{
    return interaction_message_type_e::instruction_check_part_type;
};

// Instruction_arg_extract

Instruction_arg_extract::Instruction_arg_extract()
{
	m_value = nullptr;
	m_index = 0;
	m_result = false;
}
;

Instruction_arg_extract* Instruction_arg_extract::clone()
{
	auto result = new Instruction_arg_extract();
    result->m_index = m_index;
    result->m_value = m_value->clone();
    result->m_result = m_result;
    return result;
}

void Instruction_arg_extract::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {
    case interaction_message_type_e::instruction_slot_parameter: {
	    auto p = static_cast<Instruction_slot_parameter*>(object);
	    auto& a = p->m_parameter->operator[](m_index);
        switch (a.kind) {
        case type_e::gameobject: {
            Instruction_result* i = new Instruction_result();
            i->m_value = Parameter_argument_t(type_e::gameobject);
            i->m_value.set(a.m_object);
            m_value->apply_effect(unit, i);
            p->m_result = i->m_result;
            break;
        }
        case type_e::object_part: {
            Instruction_result* i = new Instruction_result();
            i->m_value = Parameter_argument_t(type_e::object_part);
            i->m_value.set(a.m_part);
            m_value->apply_effect(unit, i);
            p->m_result = i->m_result;
            break;
        }
        }
        break;
    }
    default: {
        break;
    }
    }
};

void Instruction_arg_extract::reset_serialization_index()
{
    m_serialization_index = 0;
    if (m_value && m_value->m_serialization_index != 0) {
        m_value->reset_serialization_index();
    }
}

iPacker& Instruction_arg_extract::get_packer()
{
    return Packer<Instruction_arg_extract>::instance();
}

interaction_message_type_e Instruction_arg_extract::get_interaction_message_type()
{
    return interaction_message_type_e::instruction_arg_extract;
}

// Instruction_get_owner

Instruction_get_owner::Instruction_get_owner()
{
	m_value = nullptr;
	m_result = false;
}
;

Instruction_get_owner* Instruction_get_owner::clone()
{
	const auto result = new Instruction_get_owner();
    result->m_value = m_value->clone();
    result->m_result = m_result;
    return result;
}

void Instruction_get_owner::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {
    case interaction_message_type_e::instruction_result: {
        Instruction_result* ir = static_cast<Instruction_result*>(object);
        Parameter_argument_t& a = ir->m_value;
        switch (a.kind) {
        case type_e::gameobject: {
            Instruction_result* i = new Instruction_result();
            i->m_value = Parameter_argument_t(type_e::object_owner);
            i->m_value.set(a.m_object->m_owner);
            m_value->apply_effect(unit, i);
            ir->m_result = i->m_result;
            break;
        }
        case type_e::object_part: {
            Instruction_result* i = new Instruction_result();
            i->m_value = Parameter_argument_t(type_e::object_owner);
            i->m_value.set(a.m_part->m_owner);
            m_value->apply_effect(unit, i);
            ir->m_result = i->m_result;
            break;
        }
        }
        break;
    }
    default: {
        break;
    }
    }
};

void Instruction_get_owner::reset_serialization_index()
{
    m_serialization_index = 0;
    if (m_value && m_value->m_serialization_index != 0) {
        m_value->reset_serialization_index();
    }
}

// Instruction_get_owner_top

iPacker& Instruction_get_owner::get_packer()
{
    return Packer<Instruction_get_owner>::instance();
}

interaction_message_type_e Instruction_get_owner::get_interaction_message_type()
{
    return interaction_message_type_e::instruction_get_owner;
}

Instruction_get_owner_top::Instruction_get_owner_top()
{
    m_value = nullptr;
    m_result = false;
};

Instruction_get_owner_top* Instruction_get_owner_top::clone()
{
	auto result = new Instruction_get_owner_top();
    result->m_value = m_value->clone();
    result->m_result = m_result;
    return result;
}

void Instruction_get_owner_top::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {
    case interaction_message_type_e::instruction_result: {
        Instruction_result* ir = static_cast<Instruction_result*>(object);
        Parameter_argument_t& a = ir->m_value;
        switch (a.kind) {
        case type_e::gameobject: {
            Instruction_result* i = new Instruction_result();
            i->m_value = Parameter_argument_t(type_e::object_owner);
            i->m_value.set(a.m_object->get_owner(entity_e::game_object));
            m_value->apply_effect(unit, i);
            ir->m_result = i->m_result;
            break;
        }
        case type_e::object_part: {
            Instruction_result* i = new Instruction_result();
            i->m_value = Parameter_argument_t(type_e::object_owner);
            i->m_value.set(a.m_part->get_owner(entity_e::game_object));
            m_value->apply_effect(unit, i);
            ir->m_result = i->m_result;
            break;
        }
        case type_e::object_owner: {
            Instruction_result* i = new Instruction_result();
            i->m_value = Parameter_argument_t(type_e::object_owner);
            i->m_value.set(a.m_part->get_owner(entity_e::game_object));
            m_value->apply_effect(unit, i);
            ir->m_result = i->m_result;
            break;
        }
        }
        break;
    }
    default: {
        break;
    }
    }
}

iPacker& Instruction_get_owner_top::get_packer()
{
    return Packer<Instruction_get_owner_top>::instance();
};

// Instruction_check_owner_type

Instruction_check_owner_type::Instruction_check_owner_type()
{
} ;

Instruction_check_owner_type* Instruction_check_owner_type::clone()
{
	auto result = new Instruction_check_owner_type();
    result->m_value = m_value;
    return result;
}

void Instruction_check_owner_type::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {
    case interaction_message_type_e::instruction_result: {
        Parameter_argument_t& a = static_cast<Instruction_result*>(object)->m_value;
        switch (a.kind) {
        case type_e::object_owner: {
            static_cast<Instruction_result*>(object)->m_result = (a.m_owner->m_kind == m_value);
            break;
        }
        }
        break;
    }
    default: {
        break;
    }
    }
}

iPacker& Instruction_check_owner_type::get_packer()
{
    return Packer<Instruction_check_owner_type>::instance();
};

void ObjectTag::Mortal::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

iPacker& ObjectTag::Mortal::get_packer()
{
    return Packer<Mortal>::instance();
}
