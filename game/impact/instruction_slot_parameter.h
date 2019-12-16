#ifndef INSTRUCTION_SLOT_PARAMETER_H
#define INSTRUCTION_SLOT_PARAMETER_H

#include "interaction_slot.h"

class Instruction_slot_parameter : public Interaction_slot {
public:
    Parameter* m_parameter;
    bool m_result;

    Instruction_slot_parameter();
    Object_interaction* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;
    interaction_message_type_e get_interaction_message_type() override;
    IVISITABLE();
};

#endif //INSTRUCTION_SLOT_PARAMETER_H
