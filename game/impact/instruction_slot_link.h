#ifndef INSTRUCTION_SLOT_LINK_H
#define INSTRUCTION_SLOT_LINK_H

#include "interaction_slot.h"

class Instruction_slot_link : public Interaction_slot {
public:
    interaction_e m_subtype;
    bool m_enable;
    Instruction_slot_link();
    Instruction_slot_link* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            Interaction_slot::properties(),
            std::make_tuple(
                make_property(&Instruction_slot_link::m_subtype, u"subtype")));
    }

    interaction_message_type_e get_interaction_message_type() override;
    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
};

#endif //INSTRUCTION_SLOT_LINK_H
