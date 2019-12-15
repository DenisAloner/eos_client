#include "slot_set_state.h"
#include "GameObject.h"

Slot_set_state::Slot_set_state()
{
}

Slot_set_state* Slot_set_state::clone()
{
    auto effect = new Slot_set_state();
    effect->m_value = m_value;
    return effect;
}

void Slot_set_state::do_predicate(predicate fn)
{
    fn(this);
}

void Slot_set_state::apply_effect(GameObject* unit, Object_interaction* object)
{
    unit->set_state(m_value);
}

iPacker& Slot_set_state::get_packer()
{
    return Packer<Slot_set_state>::instance();
}

interaction_message_type_e Slot_set_state::get_interaction_message_type()
{
    return interaction_message_type_e::slot_set_state;
}

IJSONSERIALIZABLE_IMPL(Slot_set_state);
