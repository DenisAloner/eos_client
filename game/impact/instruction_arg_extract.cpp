#include "instruction_arg_extract.h"

Instruction_arg_extract::Instruction_arg_extract()
{
    m_value = nullptr;
    m_index = 0;
    m_result = false;
};

Instruction_arg_extract* Instruction_arg_extract::clone()
{
    const auto result = new Instruction_arg_extract();
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
            auto i = new Instruction_result();
            i->m_value = Parameter_argument_t(type_e::gameobject);
            i->m_value.set(a.m_object);
            m_value->apply_effect(unit, i);
            p->m_result = i->m_result;
            break;
        }
        case type_e::object_part: {
            auto i = new Instruction_result();
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

 IVISITABLE_IMPL(Instruction_arg_extract);
