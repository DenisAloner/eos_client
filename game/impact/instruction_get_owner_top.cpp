#include "instruction_get_owner_top.h"
#include "GameObject.h"

Instruction_get_owner_top::Instruction_get_owner_top()
{
    m_value = nullptr;
    m_result = false;
};

Instruction_get_owner_top* Instruction_get_owner_top::clone()
{
	const auto result = new Instruction_get_owner_top();
    result->m_value = m_value->clone();
    result->m_result = m_result;
    return result;
}

void Instruction_get_owner_top::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {
    case interaction_message_type_e::instruction_result: {
	    auto ir = static_cast<Instruction_result*>(object);
	    auto& a = ir->m_value;
        switch (a.kind) {
        case type_e::gameobject: {
	        auto i = new Instruction_result();
            i->m_value = Parameter_argument_t(type_e::object_owner);
            i->m_value.set(a.m_object->get_owner(entity_e::game_object));
            m_value->apply_effect(unit, i);
            ir->m_result = i->m_result;
            break;
        }
        case type_e::object_part: {
	        auto i = new Instruction_result();
            i->m_value = Parameter_argument_t(type_e::object_owner);
            i->m_value.set(a.m_part->get_owner(entity_e::game_object));
            m_value->apply_effect(unit, i);
            ir->m_result = i->m_result;
            break;
        }
        case type_e::object_owner: {
	        auto i = new Instruction_result();
            i->m_value = Parameter_argument_t(type_e::object_owner);
            i->m_value.set(a.m_part->get_owner(entity_e::game_object));
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
}

iPacker& Instruction_get_owner_top::get_packer()
{
    return Packer<Instruction_get_owner_top>::instance();
};