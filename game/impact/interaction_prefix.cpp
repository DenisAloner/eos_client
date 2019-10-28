#include "interaction_prefix.h"
#include "GameObject.h"
#include "tag_list.h"

Interaction_prefix::Interaction_prefix() {};

Interaction_prefix* Interaction_prefix::clone()
{
    auto effect = new Interaction_prefix();
    effect->m_subtype = m_subtype;
    effect->m_value = m_value->clone();
    return effect;
}

void Interaction_prefix::apply_effect(GameObject* unit, Object_interaction* object)
{
    auto i = static_cast<Tag_list*>(unit->get_effect(interaction_e::tag));
    if (i) {
        for (auto& m_item : i->m_items) {
            m_item->apply_effect(unit, this);
        }
    }
    // TODO сделать версию без применения и с применением модифицированного эффекта
    /*if (m_value) { m_value->apply_effect(unit, object); }*/
}

iPacker& Interaction_prefix::get_packer()
{
    return Packer<Interaction_prefix>::instance();
}

interaction_message_type_e Interaction_prefix::get_interaction_message_type()
{
    return interaction_message_type_e::interaction_prefix;
}
