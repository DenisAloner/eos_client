#ifndef IMPACT_COPY_CHANCE_H
#define IMPACT_COPY_CHANCE_H

#include "Effect.h"
class Impact_copy_chance : public Interaction_slot {
public:
    int m_chance;

    Impact_copy_chance();
    ~Impact_copy_chance();

    Object_interaction* clone() override;

    Packer_generic& get_packer() override;
};

#endif // IMPACT_COPY_CHANCE_H