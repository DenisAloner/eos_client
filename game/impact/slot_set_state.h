#ifndef SLOT_SET_STATE_H
#define SLOT_SET_STATE_H

#include "Definiton.h"

class Slot_set_state : public Object_interaction {
public:
    object_state_e m_value;
    Slot_set_state();
    void apply_effect(GameObject* unit, Object_interaction* object) override;
    Slot_set_state* clone() override;
    virtual void do_predicate(predicate fn);

    iPacker& get_packer() override;
    interaction_message_type_e get_interaction_message_type() override;
    IVISITABLE();
};

#endif //SLOT_SET_STATE_H
