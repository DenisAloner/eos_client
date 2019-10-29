#include "interaction_copyist.h"
#include "GameObject.h"

Interaction_copyist::Interaction_copyist()
{
}

Object_interaction* Interaction_copyist::clone()
{
    auto effect = new Interaction_copyist();
    effect->m_subtype = m_subtype;
    effect->m_value = m_value->clone();
    return effect;
}

void Interaction_copyist::apply_effect(GameObject* unit, Object_interaction* object)
{
	const auto i = unit->get_effect(m_subtype);
    if (i) {
        m_value->apply_effect(unit, i);
    }
}

iPacker& Interaction_copyist::get_packer()
{
    return Packer<Interaction_copyist>::instance();
}

interaction_message_type_e Interaction_copyist::get_interaction_message_type()
{
    return interaction_message_type_e::interaction_copyist;
}