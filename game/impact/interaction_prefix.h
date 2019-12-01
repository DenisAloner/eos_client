#ifndef INTERACTION_PREFIX_H
#define INTERACTION_PREFIX_H

#include "interaction_slot.h"

class Interaction_prefix : public Interaction_slot {
public:
    effect_prefix_e m_subtype;
    Interaction_prefix();
    Interaction_prefix* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;
    interaction_message_type_e get_interaction_message_type() override;
    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
};

#endif //INTERACTION_PREFIX_H