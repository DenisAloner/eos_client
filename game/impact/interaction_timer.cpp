#include "interaction_timer.h"

Interaction_timer::Interaction_timer()
{
}

Object_interaction* Interaction_timer::clone()
{
	const auto effect = new Interaction_timer();
    effect->m_turn = m_turn;
    effect->m_period = m_period;
    effect->m_value = m_value->clone();
    return effect;
}

void Interaction_timer::apply_effect(GameObject* unit, Object_interaction* object)
{
    if (m_turn == m_period - 1) {
        m_value->apply_effect(unit, object);
    }
}

iPacker& Interaction_timer::get_packer()
{
    return Packer<Interaction_timer>::instance();
}

interaction_message_type_e Interaction_timer::get_interaction_message_type()
{
    return interaction_message_type_e::interaction_timer;
}
