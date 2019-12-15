#include "instruction_get_owner.h"
#include "GameObject.h"

Instruction_get_owner::Instruction_get_owner()
{
    m_value = nullptr;
    m_result = false;
};

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
        auto ir = static_cast<Instruction_result*>(object);
        auto& a = ir->m_value;
        switch (a.kind) {
        case type_e::gameobject: {
            auto i = new Instruction_result();
            i->m_value = Parameter_argument_t(type_e::object_owner);
            i->m_value.set(a.m_object->m_owner);
            m_value->apply_effect(unit, i);
            ir->m_result = i->m_result;
            break;
        }
        case type_e::object_part: {
            auto i = new Instruction_result();
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

iPacker& Instruction_get_owner::get_packer()
{
    return Packer<Instruction_get_owner>::instance();
}

interaction_message_type_e Instruction_get_owner::get_interaction_message_type()
{
    return interaction_message_type_e::instruction_get_owner;
}

IJSONSERIALIZABLE_IMPL(Instruction_get_owner);
