#ifndef INTERACTION_ADDON_H
#define INTERACTION_ADDON_H

#include "interaction_slot.h"

class Interaction_addon : public Interaction_slot {
public:
    interaction_e m_subtype;
    Interaction_addon();
    Object_interaction* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;
    interaction_message_type_e get_interaction_message_type() override;
    IJSONSERIALIZABLE();
};

#endif //INTERACTION_ADDON_H