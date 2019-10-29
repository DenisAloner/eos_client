#ifndef INSTRUCTION_CHECK_OWNER_TYPE_H
#define INSTRUCTION_CHECK_OWNER_TYPE_H

#include "GameObject.h"

class Instruction_check_owner_type : public Object_interaction {
public:
    entity_e m_value;

    Instruction_check_owner_type();
    Instruction_check_owner_type* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Instruction_check_owner_type::m_value, u"value"));
    }

    interaction_message_type_e get_interaction_message_type() override { return interaction_message_type_e::instruction_check_owner_type; };
};

#endif //INSTRUCTION_CHECK_OWNER_TYPE_H
