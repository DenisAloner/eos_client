#include "requirements_to_object.h"
#include "instruction_game_owner.h"

ObjectTag::Requirements_to_object::Requirements_to_object()
    : Object_tag(object_tag_e::requirements_to_object)
{
    m_value = nullptr;
    m_result = false;
};

ObjectTag::Requirements_to_object* ObjectTag::Requirements_to_object::clone()
{
    auto result = new ObjectTag::Requirements_to_object;
    result->m_value = m_value->clone();
    result->m_result = m_result;
    return result;
}

void ObjectTag::Requirements_to_object::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {
    case interaction_message_type_e::instruction_game_owner: {
        m_value->apply_effect(unit, object);
        const auto i = static_cast<Instruction_game_owner*>(object);
        m_result = i->m_result;
        break;
    }
    default: {
        break;
    }
    }
}

void ObjectTag::Requirements_to_object::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
};

void ObjectTag::Requirements_to_object::reset_serialization_index()
{
    m_serialization_index = 0;
    if (m_value) {
        m_value->reset_serialization_index();
    }
}

iPacker& ObjectTag::Requirements_to_object::get_packer()
{
    return Packer<Requirements_to_object>::instance();
}