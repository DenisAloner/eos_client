#include "instruction_game_owner.h"

Instruction_game_owner::Instruction_game_owner()
{
    m_value = nullptr;
    m_result = false;
}

Object_interaction* Instruction_game_owner::clone()
{
	return nullptr;
};

void Instruction_game_owner::apply_effect(GameObject* unit, Object_interaction* object)
{
}

iPacker& Instruction_game_owner::get_packer()
{
    return Packer<Instruction_game_owner>::instance();
}

interaction_message_type_e Instruction_game_owner::get_interaction_message_type()
{
	return interaction_message_type_e::instruction_game_owner;
}

std::u16string Instruction_game_owner::serialize_to_json_reference(JsonWriter& value)
{
    return value.write(*this);
}

std::u16string Instruction_game_owner::serialize_to_json_pointer(JsonWriter& value)
{
    return value.write(this);
}
