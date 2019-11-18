#ifndef OBJECT_STATE_EQUIP_H
#define OBJECT_STATE_EQUIP_H

#include "object_state.h"

class Object_state_equip : public Object_state {
public:
    body_part_e m_body_part;
    Attribute_map m_equip;
    Object_state_equip();
    Object_state* clone() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            Object_state::properties(),
            std::make_tuple(
                make_property(&Object_state_equip::m_body_part, u"body_part"),
                make_property(&Object_state_equip::m_equip, u"equip")));
    }
};

#endif
