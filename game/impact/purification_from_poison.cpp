#include "purification_from_poison.h"
#include "GameObject.h"

ObjectTag::Purification_from_poison::Purification_from_poison()
    : Object_tag(object_tag_e::purification_from_poison) {};

ObjectTag::Purification_from_poison* ObjectTag::Purification_from_poison::clone()
{
	const auto effect = new ObjectTag::Purification_from_poison();
    return effect;
}

void ObjectTag::Purification_from_poison::apply_effect(GameObject* unit, Object_interaction* object)
{
	const auto prefix = static_cast<Interaction_prefix*>(object);
    switch (prefix->m_subtype) {
    case effect_prefix_e::poison_damage: {
	    auto p = unit->get_parameter(interaction_e::poison);
        if (p) {
	        const auto e = static_cast<Effect*>(prefix->m_value);
            p->m_basic_value -= e->m_value;
            if (p->m_basic_value > p->m_basic_limit) {
                e->m_value = p->m_basic_limit - p->m_basic_value;
                p->m_basic_value = p->m_basic_limit;
            } else {
                e->m_value = 0;
            }
        }
        break;
    }
    }
}

iPacker& ObjectTag::Purification_from_poison::get_packer()
{
    return Packer<Purification_from_poison>::instance();
}

IVISITABLE_IMPL(ObjectTag::Purification_from_poison);
