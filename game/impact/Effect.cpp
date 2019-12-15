#include "effect.h"
#include "writer.h"
#include "parameter_list.h"
#include "interaction_copyist.h"
#include "interaction_prefix.h"

Effect::Effect()
    : m_value(0)
    , m_subtype()
{
}

Effect* Effect::clone()
{
    auto effect = new Effect();
    effect->m_subtype = m_subtype;
    effect->m_value = m_value;
    return effect;
}

void Effect::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (m_subtype) {
    case effect_e::value: {
	    const auto item = static_cast<Parameter_list*>(object);
        item->m_basic_value = item->m_basic_value + m_value;
        if (item->m_basic_value <= 1) {
            item->m_basic_value = 0;
            auto copyist = new Interaction_copyist();
            copyist->m_subtype = item->m_subtype;
            copyist->m_value = clone();
            auto prefix = new Interaction_prefix();
            prefix->m_value = copyist;
            prefix->m_subtype = effect_prefix_e::parameter_change;
            prefix->apply_effect(unit, object);
        }
        break;
    }
    case effect_e::limit: {
	    const auto item = static_cast<Parameter_list*>(object);
        item->m_basic_limit = item->m_basic_limit + m_value;
        break;
    }
    }
}

iPacker& Effect::get_packer()
{
    return Packer<Effect>::instance();
}

interaction_message_type_e Effect::get_interaction_message_type()
{
    return interaction_message_type_e::effect;
}

IJSONSERIALIZABLE_IMPL(Effect);
