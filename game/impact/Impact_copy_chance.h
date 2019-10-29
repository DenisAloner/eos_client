#ifndef IMPACT_COPY_CHANCE_H
#define IMPACT_COPY_CHANCE_H

#include "interaction_slot.h"

class Impact_copy_chance : public Interaction_slot {
public:
    int m_chance;

    Impact_copy_chance();
    ~Impact_copy_chance();

    Object_interaction* clone() override;

    iPacker& get_packer() override;
    interaction_message_type_e get_interaction_message_type() override;
};

#endif // IMPACT_COPY_CHANCE_H