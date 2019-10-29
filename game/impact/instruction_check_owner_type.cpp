#include "instruction_check_owner_type.h"

Instruction_check_owner_type::Instruction_check_owner_type() {};

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
        auto& a = static_cast<Instruction_result*>(object)->m_value;
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
