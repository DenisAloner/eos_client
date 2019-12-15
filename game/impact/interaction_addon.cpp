#include "interaction_addon.h"
#include "GameObject.h"

Interaction_addon::Interaction_addon() {};

Object_interaction* Interaction_addon::clone()
{
    auto effect = new Interaction_addon();
    effect->m_subtype = m_subtype;
    effect->m_value = m_value->clone();
    return effect;
}

void Interaction_addon::apply_effect(GameObject* unit, Object_interaction* object)
{
    unit->add_from(m_subtype, m_value->clone());
}

iPacker& Interaction_addon::get_packer()
{
    return Packer<Interaction_addon>::instance();
}

interaction_message_type_e Interaction_addon::get_interaction_message_type()
{
    return interaction_message_type_e::interaction_addon;
}

 IJSONSERIALIZABLE_IMPL(Interaction_addon);
