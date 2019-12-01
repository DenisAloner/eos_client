#ifndef INTERACTION_TIME_H
#define INTERACTION_TIME_H

#include "interaction_slot.h"

class Interaction_time : public Interaction_slot {
public:
    int m_turn;
    Interaction_time();
    Object_interaction* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            Interaction_slot::properties(),
            std::make_tuple(
                make_property(&Interaction_time::m_turn, u"m_turn")));
    }

    interaction_message_type_e get_interaction_message_type() override;
    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
};

#endif //INTERACTION_TIME_H