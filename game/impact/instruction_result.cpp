#include "instruction_result.h"

Instruction_result::Instruction_result()
{
    m_result = false;
}

Instruction_result* Instruction_result::clone()
{
	return nullptr;
}

iPacker& Instruction_result::get_packer()
{
    return Packer<Instruction_result>::instance();
}

interaction_message_type_e Instruction_result::get_interaction_message_type()
{
    return interaction_message_type_e::instruction_result;
}

IVISITABLE_IMPL(Instruction_result);
