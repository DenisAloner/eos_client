#include "instruction_slot_link.h"
#include "GameObject.h"

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
	    const auto parameter = static_cast<Instruction_game_owner*>(object);
	    auto part = static_cast<ObjectPart*>(parameter->m_value);
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
	    auto ir = static_cast<Instruction_result*>(object);
	    auto& a = ir->m_value;
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
                i = static_cast<ObjectPart*>(a.m_owner)->m_attributes.get_list(m_subtype);
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
	    const auto parameter = static_cast<Instruction_slot_parameter*>(object);
	    auto& p(*(parameter->m_parameter));
	    auto part = static_cast<ObjectPart*>(p[1].m_object->m_owner);
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