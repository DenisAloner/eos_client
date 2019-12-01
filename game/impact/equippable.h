#ifndef EQUIPPABLE_H
#define EQUIPPABLE_H

#include "interaction_list.h"
#include "object_tag.h"

namespace ObjectTag {

class Equippable : public Object_tag {
public:
    Object_interaction* m_value;
    Interaction_list* m_condition;

    Equippable();
    Equippable* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;
    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            Object_tag::properties(),
            std::make_tuple(
                make_property(&Equippable::m_value, u"value"),
                make_property(&Equippable::m_condition, u"condition")));
    }

    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
};

}

#endif //EQUIPPABLE_H