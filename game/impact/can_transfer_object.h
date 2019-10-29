#ifndef CAN_TRANSFER_OBJECT_H
#define CAN_TRANSFER_OBJECT_H

#include "object_tag.h"

namespace ObjectTag {

class Can_transfer_object : public Object_tag {
public:
    Object_interaction* m_value;
    bool m_result;

    Can_transfer_object();
    virtual Can_transfer_object* clone();
    void apply_effect(GameObject* unit, Object_interaction* object) override;
    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            Object_tag::properties(),
            std::make_tuple(
                make_property(&Can_transfer_object::m_value, u"value")));
    }
};

}

#endif //CAN_TRANSFER_OBJECT
