#include "instruction_result.h"
#include "writer.h"

Instruction_result::Instruction_result()
{
    m_result = false;
}

iPacker& Instruction_result::get_packer()
{
    return Packer<Instruction_result>::instance();
}

interaction_message_type_e Instruction_result::get_interaction_message_type()
{
    return interaction_message_type_e::instruction_result;
}

std::u16string Instruction_result::serialize_to_json_reference(JsonWriter& value)
{
    return value.write(*this);
}

std::u16string Instruction_result::serialize_to_json_pointer(JsonWriter& value)
{
    return value.write(this);
};
