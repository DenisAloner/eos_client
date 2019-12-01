#include "interaction_time.h"
#include "writer.h"

Interaction_time::Interaction_time()
{
}

Object_interaction* Interaction_time::clone()
{
	const auto effect = new Interaction_time();
    effect->m_turn = m_turn;
    effect->m_value = m_value->clone();
    return effect;
}

void Interaction_time::apply_effect(GameObject* unit, Object_interaction* object)
{
    m_value->apply_effect(unit, object);
}

iPacker& Interaction_time::get_packer()
{
    return Packer<Interaction_time>::instance();
}

interaction_message_type_e Interaction_time::get_interaction_message_type()
{
    return interaction_message_type_e::interaction_time;
}

std::u16string Interaction_time::serialize_to_json_reference(JsonWriter& value)
{
    return value.write(*this);
}

std::u16string Interaction_time::serialize_to_json_pointer(JsonWriter& value)
{
    return value.write(this);
}
