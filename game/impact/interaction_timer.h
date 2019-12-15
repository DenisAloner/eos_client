#ifndef INTERACTION_TIMER_H
#define INTERACTION_TIMER_H

#include "interaction_slot.h"

class Interaction_timer : public Interaction_slot {
public:
    int m_turn;
    int m_period;
    Interaction_timer();
    Object_interaction* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;
    interaction_message_type_e get_interaction_message_type() override;
    IJSONSERIALIZABLE();
};

#endif //INTERACTION_TIMER_H