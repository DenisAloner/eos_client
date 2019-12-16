#include "mortal.h"
#include "GameObject.h"

ObjectTag::Mortal::Mortal()
    : Object_tag(object_tag_e::mortal) {};

ObjectTag::Mortal* ObjectTag::Mortal::clone()
{
	const auto effect = new ObjectTag::Mortal();
    return effect;
}

void ObjectTag::Mortal::apply_effect(GameObject* unit, Object_interaction* object)
{
	const auto prefix = static_cast<Interaction_prefix*>(object);
    switch (prefix->m_subtype) {
    case effect_prefix_e::parameter_change: {
	    const auto copyist = static_cast<Interaction_copyist*>(prefix->m_value);
        if (copyist->m_subtype == interaction_e::health) {
            const auto feat = unit->get_parameter(interaction_e::health);
            if (feat) {
                if (feat->m_basic_value == 0) {
                    unit->set_state(object_state_e::dead);
                }
            }
        }
        break;
    }
    }
}

void ObjectTag::Mortal::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

iPacker& ObjectTag::Mortal::get_packer()
{
    return Packer<Mortal>::instance();
}

IVISITABLE_IMPL(ObjectTag::Mortal);
