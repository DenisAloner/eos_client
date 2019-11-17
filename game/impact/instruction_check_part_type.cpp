#include "instruction_check_part_type.h"

Instruction_check_part_type::Instruction_check_part_type() {};

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
	    const auto i = static_cast<Instruction_game_owner*>(object);
	    const auto op = static_cast<ObjectPart*>(i->m_value);
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

