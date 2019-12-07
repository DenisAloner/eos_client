#include "instruction_check_tag.h"

Instruction_check_tag::Instruction_check_tag() {};

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
	    auto i = static_cast<Instruction_game_owner*>(object);
        switch (i->m_value->m_kind) {
        case entity_e::game_object: {
	        auto obj = static_cast<GameObject*>(i->m_value);
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
}

IJSONSERIALIZABLE_IMPL(Instruction_check_tag);

