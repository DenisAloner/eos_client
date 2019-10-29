#include "poison_resist.h"
#include "interaction_prefix.h"
#include "effect.h"

ObjectTag::Poison_resist::Poison_resist()
    : Object_tag(object_tag_e::poison_resist) {};

ObjectTag::Poison_resist* ObjectTag::Poison_resist::clone()
{
    const auto effect = new ObjectTag::Poison_resist();
    return effect;
}

void ObjectTag::Poison_resist::apply_effect(GameObject* unit, Object_interaction* object)
{
	const auto prefix = static_cast<Interaction_prefix*>(object);
    switch (prefix->m_subtype) {
    case effect_prefix_e::poison_damage: {
        static_cast<Effect*>(prefix->m_value)->m_value /= 2;
        break;
    }
    }
}

iPacker& ObjectTag::Poison_resist::get_packer()
{
    return Packer<Poison_resist>::instance();
}