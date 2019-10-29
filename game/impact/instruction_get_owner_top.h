#ifndef INSTRUCTION_GET_OWNER_TOP_H
#define INSTRUCTION_GET_OWNER_TOP_H

#include "instruction_get_owner.h"

class Instruction_get_owner_top : public Instruction_get_owner {
public:
    Instruction_get_owner_top();
    Instruction_get_owner_top* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;

    constexpr static auto properties() { return Instruction_get_owner::properties(); }
};

#endif //INSTRUCTION_GET_OWNER_TOP_H
