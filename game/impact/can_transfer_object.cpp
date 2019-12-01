#include "can_transfer_object.h"
#include "writer.h"

ObjectTag::Can_transfer_object::Can_transfer_object()
    : Object_tag(object_tag_e::can_transfer_object)
{
    m_value = nullptr;
    m_result = false;
};

ObjectTag::Can_transfer_object* ObjectTag::Can_transfer_object::clone()
{
	const auto result = new ObjectTag::Can_transfer_object;
    result->m_value = m_value->clone();
    result->m_result = m_result;
    return result;
}

void ObjectTag::Can_transfer_object::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {
    case interaction_message_type_e::instruction_slot_parameter: {
        m_value->apply_effect(unit, object);
        /*Instruction_game_owner* i = static_cast<Instruction_game_owner*>(object);
		m_result = i->m_result;*/
        break;
    }
    default: {
        break;
    }
    }
}

void ObjectTag::Can_transfer_object::reset_serialization_index()
{
    m_serialization_index = 0;
    if (m_value && m_value->m_serialization_index != 0) {
        m_value->reset_serialization_index();
    }
};

iPacker& ObjectTag::Can_transfer_object::get_packer()
{
    return Packer<Can_transfer_object>::instance();
}

std::u16string ObjectTag::Can_transfer_object::serialize_to_json_reference(JsonWriter& value)
{
    return value.write(*this);
}

std::u16string ObjectTag::Can_transfer_object::serialize_to_json_pointer(JsonWriter& value)
{
    return value.write(this);
}
