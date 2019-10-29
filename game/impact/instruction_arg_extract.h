#ifndef INSTRUCTION_ARG_EXTRACT_H
#define INSTRUCTION_ARG_EXTRACT_H

#include "GameObject.h"

class Instruction_arg_extract : public Object_interaction {
public:
    Object_interaction* m_value;
    std::size_t m_index;
    bool m_result;

    Instruction_arg_extract();
    Instruction_arg_extract* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;
    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Instruction_arg_extract::m_value, u"value"),
            make_property(&Instruction_arg_extract::m_index, u"index"));
    }

    interaction_message_type_e get_interaction_message_type() override;
};

#endif //INSTRUCTION_ARG_EXTRACT_H
