#include "activator.h"
#include "GameObject.h"

ObjectTag::Activator::Activator()
    : Object_tag(object_tag_e::activator) {};

ObjectTag::Activator* ObjectTag::Activator::clone()
{
    auto effect = new ObjectTag::Activator();
    effect->m_link = m_link;
    return effect;
}

void ObjectTag::Activator::apply_effect(GameObject* unit, Object_interaction* object)
{
    const auto prefix = static_cast<Interaction_prefix*>(object);
    switch (prefix->m_subtype) {
    case effect_prefix_e::state_change: {
        const auto e = static_cast<Slot_set_state*>(prefix->m_value);
        if (e) {
            for (auto& l : m_link) {
                l->set_state(e->m_value);
            }
        }
        break;
    }
    }
}

void ObjectTag::Activator::reset_serialization_index()
{
    m_serialization_index = 0;
    for (auto& item : m_link) {
        if (item->m_serialization_index > 0) {
            item->reset_serialization_index();
        }
    }
}

iPacker& ObjectTag::Activator::get_packer()
{
    return Packer<Activator>::instance();
}

IJSONSERIALIZABLE_IMPL(ObjectTag::Activator);
