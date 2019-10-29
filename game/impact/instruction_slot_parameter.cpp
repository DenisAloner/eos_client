#include "instruction_slot_parameter.h"

Instruction_slot_parameter::Instruction_slot_parameter()
{
    m_value = nullptr;
}

Object_interaction* Instruction_slot_parameter::clone()
{
	const auto effect = new Instruction_slot_parameter();
    //effect->m_value = m_value->clone();
    return effect;
}

void Instruction_slot_parameter::apply_effect(GameObject* unit, Object_interaction* object)
{
    /*if (m_enable)
	{
		m_value->apply_effect(unit, object);
		m_enable = false;
	}
	else
	{
		Object_part* o = static_cast<Object_part*>(object);
		if (o->m_object_state.get_stat(m_subtype))
		{
			m_value->apply_effect(unit, object);
			m_enable = true;
		}
	}*/
}

iPacker& Instruction_slot_parameter::get_packer()
{
    return Packer<Instruction_slot_parameter>::instance();
}

interaction_message_type_e Instruction_slot_parameter::get_interaction_message_type()
{
    return interaction_message_type_e::instruction_slot_parameter;
}