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
    IJSONSERIALIZABLE();
};

#endif //INTERACTION_PREFIX_H