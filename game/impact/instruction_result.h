#ifndef INSTRUCTION_RESULT_H
#define INSTRUCTION_RESULT_H

#include "Parameter.h"

class Instruction_result : public Object_interaction {
public:
    Parameter_argument_t m_value;
    bool m_result;

    Instruction_result();
    Instruction_result* clone() override;;
    void apply_effect(GameObject* unit, Object_interaction* object) override {};

    iPacker& get_packer() override;
    interaction_message_type_e get_interaction_message_type() override;
    IJSONSERIALIZABLE();
};

#endif //INSTRUCTION_RESULT_H
