#include "fast_move.h"
#include "Action.h"

ObjectTag::Fast_move::Fast_move()
    : Object_tag(object_tag_e::fast_move) {};

ObjectTag::Fast_move* ObjectTag::Fast_move::clone()
{
    const auto effect = new ObjectTag::Fast_move();
    return effect;
}

void ObjectTag::Fast_move::apply_effect(GameObject* unit, Object_interaction* object)
{
	const auto prefix = static_cast<Interaction_prefix*>(object);
    switch (prefix->m_subtype) {
    case effect_prefix_e::action: {
        auto e = static_cast<Action_wrapper*>(prefix->m_value);
        if (e) {
            if (e->m_action->m_kind == action_e::move) {
                if (e->m_decay != 1) {
                    e->m_decay /= 2;
                }
            }
        }
        break;
    }
    }
}

iPacker& ObjectTag::Fast_move::get_packer()
{
    return Packer<Fast_move>::instance();
}